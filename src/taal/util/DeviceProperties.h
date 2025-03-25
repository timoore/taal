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

#include <vsg/app/Window.h>
#include <vsg/core/Inherit.h>
#include <vsg/core/Object.h>
#include <vsg/vk/DeviceFeatures.h>

namespace taal
{
    class DeviceProperties : public vsg::Inherit<vsg::Object, DeviceProperties>
    {
    public:
        DeviceProperties();
        vsg::ref_ptr<vsg::DeviceFeatures> features;
        vsg::Names extensions;
    };

    void setProperties(const vsg::ref_ptr<vsg::Window>& window,
                       const vsg::ref_ptr<DeviceProperties>& properties);
    
    void setProperties(const vsg::ref_ptr<vsg::WindowTraits>& windowTraits,
                       const vsg::ref_ptr<DeviceProperties>& properties);
}
