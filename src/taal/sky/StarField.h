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

#include <vsg/io/FileSystem.h>
#include <vsg/nodes/Group.h>
#include <vsg/utils/ShaderSet.h>

namespace taal
{
    class StarField : public vsg::Inherit<Component, StarField>
    {
    public:
        class StarFieldGroup : public vsg::Inherit<vsg::Group, StarFieldGroup>
        {
        public:
            explicit StarFieldGroup(const vsg::ref_ptr<vsg::Options>& options,
                                    StarField* starfield);
        };
        friend class StarFieldGroup;
        void init(const vsg::ref_ptr<vsg::PhysicalDevice>& physDevice,
                  const vsg::ref_ptr<vsg::Options>& options) override;
        void addDeviceFeatures(
            const vsg::ref_ptr<vsg::PhysicalDevice> &physDevice,
            const vsg::ref_ptr<vsg::DeviceFeatures> &deviceFeatures) override;
        vsg::ref_ptr<StarFieldGroup> createGroup(const vsg::ref_ptr<vsg::Options>& options);
    protected:
        vsg::ref_ptr<vsg::ShaderSet> _starShaderSet;
    };
}
