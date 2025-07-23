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

#pragma once

#include <taal/util/Component.h>

#include <vsg/app/RenderGraph.h>
#include <vsg/vk/Device.h>
#include <vulkan/vulkan_core.h>

#include <cstdint>

namespace taal
{
    class HDR : public vsg::Inherit<Component, HDR>
    {
    public:
        HDR(uint32_t sampleBits = VK_SAMPLE_COUNT_1_BIT);
        void init(const vsg::ref_ptr<vsg::PhysicalDevice>& physDevice) override;
        vsg::ref_ptr<vsg::RenderPass> createHdrRenderPass(vsg::ref_ptr<vsg::Device> device);
        void buildImages(const vsg::ref_ptr<vsg::Device>& device, const VkExtent2D& extent);
        uint32_t preferredSampleBits;
        uint32_t actualSampleBits;
        VkFormat imageFormat;
        VkFormat depthFormat;

        vsg::ref_ptr<vsg::Image> _colorImage;
        vsg::ref_ptr<vsg::Image> _depthImage;
        vsg::ref_ptr<vsg::Image> _outputImage;
        vsg::ref_ptr<vsg::ImageView> _colorImageView;
        vsg::ref_ptr<vsg::ImageView> _depthImageView;
        vsg::ref_ptr<vsg::ImageView> _outputImageView;
    };
}
