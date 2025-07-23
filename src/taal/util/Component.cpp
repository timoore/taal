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

#include "Component.h"

#include <vsg/io/Logger.h>

namespace taal
{
    void Component::init(const vsg::ref_ptr<vsg::PhysicalDevice>&,
                         const vsg::ref_ptr<vsg::Options>&)
    {
    }

    void Component::addDeviceFeatures(const vsg::ref_ptr<vsg::PhysicalDevice> &,
                                      const vsg::ref_ptr<vsg::DeviceFeatures> &)
    {
    }

    Taal::Taal(vsg::ref_ptr<vsg::Options> options)
        : _options(std::move(options))
    {
        _options->fileCache = vsg::getEnv("VSG_FILE_CACHE");
        _options->paths = vsg::getEnvPaths("VSG_FILE_PATH");
    }

    void Taal::addComponent(vsg::ref_ptr<Component> component)
    {
        _components.push_back(std::move(component));
    }

    void Taal::init(const vsg::ref_ptr<vsg::Window> &window)
    {
        if (window->getDevice())
        {
            vsg::warn("The Vulkan Device has already been created; features may or may not be enabled.");
        }
        auto physDevice = window->getOrCreatePhysicalDevice();
        init(physDevice);
    }

    void Taal::init(const vsg::ref_ptr<vsg::PhysicalDevice> &physDevice)
    {
        for (auto &component : _components)
        {
            component->init(physDevice, _options);
        }
    }

    void Taal::addDeviceFeatures(
        const vsg::ref_ptr<vsg::PhysicalDevice> &physDevice,
        const vsg::ref_ptr<vsg::DeviceFeatures> &deviceFeatures)
    {
        for (auto &component : _components) {
            component->addDeviceFeatures(physDevice, deviceFeatures);
        }
    }

    void Taal::addDeviceFeatures(const vsg::ref_ptr<vsg::Window> &window)
    {
        auto traits = window->traits();
        if (!traits->deviceFeatures)
        {
            traits->deviceFeatures = vsg::DeviceFeatures::create();
        }
        addDeviceFeatures(window->getOrCreatePhysicalDevice(), traits->deviceFeatures);
    }
} // namespace taal
