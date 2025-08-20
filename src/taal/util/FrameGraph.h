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

// A go at what everyone else calls a "render graph." The frame graph
// connects "processors," which transform input and output, via those
// inputs and outputs which are called "resources". It constructs
// command graphs that include the processor nodes and any barrier
// commands that are needed between the processors.
//
// Heavily influenced by Granite
// (https://github.com/Themaister/Granite) and the accompanying blog
// post at https://themaister.net/blog/2017/08/.

#pragma once

#include <vsg/app/CommandGraph.h>
#include <vsg/core/observer_ptr.h>
#include <vsg/nodes/Group.h>
#include <vsg/vk/vulkan.h>

#include <unordered_map>
#include <vector>

namespace taal
{

    struct Resource;
    struct NodeTransitions;
    
    // A node that eventually will contribute a subgraph to a command
    // graph. 
    struct FrameGraphNode : public vsg::Inherit<vsg::Object, FrameGraphNode>
    {
        virtual std::vector<vsg::ref_ptr<Resource>> inputResources() = 0;
        virtual std::vector<vsg::ref_ptr<Resource>> outputResources() = 0;
        virtual vsg::ref_ptr<vsg::Node> nodeForCommandGraph() = 0;
        virtual NodeTransitions getNodeTransitions() = 0;
        
    };

    struct Resource : public vsg::Inherit<vsg::Object, Resource>
    {
        vsg::observer_ptr<FrameGraphNode> writer;
        std::vector<vsg::observer_ptr<FrameGraphNode>> readers;
    };

    // Straight outta Granite
    enum SizeClass
    {
        Absolute,
        SwapchainRelative,
        InputRelative
    };

    struct ImageResource : public vsg::Inherit<Resource, ImageResource>
    {
        SizeClass sizeClass = Absolute;
        vsg::ref_ptr<vsg::Image> image;
    };

    struct ResourceUse
    {
        vsg::ref_ptr<Resource> resource;
        // stage produced or stages consumed
        VkPipelineStageFlags stages = {};
        VkAccessFlags dstAccessMask = {};
        VkImageLayout newLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    };
    
    struct NodeTransitions
    {
        std::vector<ResourceUse> invalidate;
        std::vector<ResourceUse> flush;
    };
    

    // A node that does "something" with inputs and outputs.
    struct Processor : vsg::Inherit<FrameGraphNode, Processor>
    {
        std::vector <vsg::ref_ptr<ImageResource>> inputs;
        std::vector<vsg::ref_ptr<ImageResource>> outputs;
        std::vector<vsg::ref_ptr<Resource>> inputResources() override;
        std::vector<vsg::ref_ptr<Resource>> outputResources() override;
        vsg::ref_ptr<vsg::Node> nodeForCommandGraph() override;
        vsg::ref_ptr<vsg::Node> vsgNode;
    };


    struct ImageMemoryInput : public vsg::Inherit<InputResource, ImageMemoryInput>
    {

        VkAccessFlags dstAccessMask = VK_ACCESS_SHADER_WRITE_BIT;
        VkImageLayout newLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        uint32_t dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        vsg::ref_ptr<vsg::Image> image;
        VkImageSubresourceRange subresourceRange = {0, 0, 0, 0, 0};
    };

    struct ImageMemoryOutput : public vsg::Inherit<OutputResource, ImageMemoryOutput>
    {
    public:
        VkAccessFlags srcAccessMask = 0;
        VkImageLayout oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        uint32_t srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    };

    struct PresentableSurface : public vsg::Inherit<OutputResource, PresentableSurface>
    {
    };
    
    struct vsgProcessor : public vsg::Inherit<Processor, vsgProcessor>
    {
        vsg::ref_ptr<vsg::Group> vsgObj;
    };

    struct RenderProcessor : vsg::Inherit<Processor, RenderProcessor>
    {
    };
    
    // A collection of frame graph nodes
    struct FrameGraph : vsg::Inherit<vsg::Object, FrameGraph>
    {
        FrameGraph();

        resizeSwapchain(const VkExtent2D& swapchainExtent);
        std::unordered_map<std::string, Resource> resources;
        // A map? A set?
        std::vector<vsg::ref_ptr<FrameGraphNode>> nodes;
        std::vector<vsg::ref_ptr<FrameGraphNode>> orderGraph(const vsg::ref_ptr<PresentableSurface>& presentableSurface);
        vsg::CommandGraphs createCommandGraphs();

        vsg::ref_ptr<PresentableSurface> presentable;
    };
    

}
