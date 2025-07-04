/* <editor-fold desc="MIT License">

Copyright(c) 2025 Timothy Moore

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

</editor-fold> */

#include "StarField.h"

#include "taal/sky/StarCatalog.h"
#include "taal/shading/shaderDefs.h"
#include "taal/util/math.h"

#include <vsg/app/RecordTraversal.h>
#include <vsg/core/Array.h>
#include <vsg/io/read.h>
#include <vsg/nodes/StateGroup.h>
#include <vsg/nodes/VertexDraw.h>
#include <vsg/state/ColorBlendState.h>
#include <vsg/state/InputAssemblyState.h>
#include <vsg/state/RasterizationState.h>
#include <vsg/utils/GraphicsPipelineConfigurator.h>
#include <vsg/utils/ShaderSet.h>
#include <vulkan/vulkan_core.h>

namespace taal
{
    namespace
    {
        vsg::ref_ptr<vsg::ShaderSet> makeShaderSet(const vsg::ref_ptr<vsg::Options>& options)
        {
            auto vertexShader = vsg::read_cast<vsg::ShaderStage>("shaders/stars.vert", options);
            auto fragmentShader = vsg::read_cast<vsg::ShaderStage>("shaders/stars.frag", options);

            if (!vertexShader || !fragmentShader)
            {
                vsg::fatal("makeShaderSet(...) could not find shaders.");
                return {};
            }

            auto result = vsg::ShaderSet::create(vsg::ShaderStages{vertexShader, fragmentShader});
            result->addAttributeBinding("taal_starData", "", 0,
                                        VK_FORMAT_R32G32B32A32_SFLOAT, vsg::vec4Array::create(1));
            result->addAttributeBinding("taal_starColor", "", 1,
                                        VK_FORMAT_R32G32B32_SFLOAT, vsg::vec3Array::create(1));
            result->addDescriptorBinding("lightData", "", shading::VIEW_DESCRIPTOR_SET, 0,
                                         VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, {});
            result->addDescriptorBinding("viewData", "", shading::VIEW_DESCRIPTOR_SET, 1,
                                         VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1 ,VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, {});
            result->addPushConstantRange("pc", "", VK_SHADER_STAGE_VERTEX_BIT, 0, 128);
            result->customDescriptorSetBindings.push_back(vsg::ViewDependentStateBinding::create(shading::VIEW_DESCRIPTOR_SET));
            return result;
        }

        struct SetPipelineStates : public vsg::Visitor
        {
            VkPrimitiveTopology topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
            bool blending = false;
            bool two_sided = false;
            bool depthClamp = false;

            SetPipelineStates(VkPrimitiveTopology in_topology, bool in_blending, bool in_two_sided,
                              bool in_depth_clamp)
                : topology(in_topology), blending(in_blending), two_sided(in_two_sided),
                  depthClamp(in_depth_clamp)
            {
            }
            void apply(vsg::Object& object) override
            {
                object.traverse(*this);
            }
            void apply(vsg::RasterizationState& rs) override
            {
                if (two_sided)
                {
                    rs.cullMode = VK_CULL_MODE_NONE;
                }
                if (depthClamp)
                {
                    rs.depthClampEnable = VK_TRUE;
                }
            }
            void apply(vsg::InputAssemblyState& ias) override
            {
                ias.topology = topology;
            }
            void apply(vsg::ColorBlendState& cbs) override
            {
                cbs.configureAttachments(blending);
            }
            void apply(vsg::DepthStencilState& dss) override
            {
                dss.depthCompareOp = VK_COMPARE_OP_GREATER_OR_EQUAL;
            }
        };
    }

    StarField::StarField(const vsg::ref_ptr<vsg::Options>& options)
    {
        auto starArray = vsg::vec4Array::create(StarCatalog::catalog.size());
        auto colorArray = vsg::vec3Array::create(StarCatalog::catalog.size());
        for (unsigned i = 0; i < StarCatalog::catalog.size(); ++i)
        {
            const auto& entry = StarCatalog::catalog[i];
            auto [sd, cd] = sincos(entry.declination);
            auto [sra, cra] = sincos(entry.rightAscensionRad);
            (*starArray)[i] = vsg::vec4(cra * cd, sra * cd, sd, entry.magnitude);
            (*colorArray)[i] = vsg::vec3(1.0, 1.0, 1.0);
#if 0
            // Debugging; highlight Ursa Major
            auto auxEntry = StarCatalog::auxData.find(entry.hr);
            if (auxEntry != StarCatalog::auxData.end())
            {
                if (auxEntry->second.constellation.compare("UMa") == 0)
                {
                    (*colorArray)[i] = vsg::vec3(1.0, 0.0, 0.0);
                }
            }
#endif
        }
        auto starShaderSet = makeShaderSet(options);
        auto pipelineConf = vsg::GraphicsPipelineConfigurator::create(starShaderSet);
        SetPipelineStates sps(VK_PRIMITIVE_TOPOLOGY_POINT_LIST, false, false, false);
        pipelineConf->accept(sps);
        vsg::DataList vertexArrays;
        pipelineConf->assignArray(vertexArrays, "taal_starData", VK_VERTEX_INPUT_RATE_VERTEX, starArray);
        pipelineConf->assignArray(vertexArrays, "taal_starColor", VK_VERTEX_INPUT_RATE_VERTEX, colorArray);
        auto vd = vsg::VertexDraw::create();
        vd->assignArrays(vertexArrays);
        vd->vertexCount = starArray->size();
        vd->instanceCount = 1;
        pipelineConf->init();
        auto stateGroup = vsg::StateGroup::create();
        stateGroup->add(pipelineConf->bindGraphicsPipeline);
        stateGroup->addChild(vd);
        addChild(stateGroup);
    }

}
