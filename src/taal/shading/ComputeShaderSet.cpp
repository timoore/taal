#include "ComputeShaderSet.h"

namespace taal
{
    ComputeShaderSet::ComputeShaderSet(vsg::ref_ptr<vsg::ShaderStage> computeStage,
                                       vsg::ref_ptr<vsg::ShaderCompileSettings> in_hints)
        : Inherit(vsg::ShaderStages{computeStage}, in_hints)
    {
    }

    vsg::ref_ptr<vsg::ShaderStage> ComputeShaderSet::getShaderStage(const vsg::ref_ptr<vsg::ShaderCompileSettings>& scs)
    {
        auto shaderStages = ShaderSet::getShaderStages(scs);
        return shaderStages[0];
    }
}
