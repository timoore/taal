#pragma once

#include <vsg/utils/ShaderSet.h>

namespace taal
{
class ComputeShaderSet : public vsg::Inherit<vsg::ShaderSet, ComputeShaderSet>
{
public:
    ComputeShaderSet() = default;
    ComputeShaderSet(vsg::ref_ptr<vsg::ShaderStage> computeStage,
                     vsg::ref_ptr<vsg::ShaderCompileSettings> in_hints = {});
    vsg::ref_ptr<vsg::ShaderStage> getShaderStage(const vsg::ref_ptr<vsg::ShaderCompileSettings>& scs = {});
private:
    using ShaderSet::addAttributeBinding;
    using ShaderSet::getAttributeBinding;
    using ShaderSet::getShaderStages;
    
};
}
