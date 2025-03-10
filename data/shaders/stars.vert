#version 450
#extension GL_ARB_separate_shader_objects : enable

#define VIEW_DESCRIPTOR_SET 0

layout(push_constant) uniform PushConstants {
    mat4 projection;
    mat4 modelView;
} pc;


layout(location = 0) in vec4 taal_starData;

layout(location = 0) out vec4 starColor;

layout(set = VIEW_DESCRIPTOR_SET, binding = 1) uniform ViewportParams
{
    vec4 viewportExtent;     // x, y, width, height
#if 0
    ivec4 scissorExtent;     // offset x, offset y, width, height
    vec2 depthExtent;        // minDepth, maxDepth
#endif
} viewportParams;

out gl_PerVertex{ vec4 gl_Position; float gl_PointSize; };


void main()
{
    vec4 starDirection = vec4(taal_starData.xyz, 0.0);
    vec4 starProjection = (pc.projection * pc.modelView) * starDirection;
    starProjection.z = 0.0;
    gl_Position = starProjection;
    float magnitude = taal_starData.w;
    gl_PointSize = max((8.0 - magnitude) / 2.0, 1.0);
    starColor = vec4(1.0, 1.0, 1.0, 1.0);
}
