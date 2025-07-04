#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec4 starColor;

layout(location = 0) out vec4 outColor;

void main()
{
    float magnitude = starColor.w;
    float radius = length(gl_PointCoord - vec2(.5, .5));
    // Make mag 6.0 be barely visible.
    float irradiance = pow(2.512, 6.0 - magnitude) * 0.0625;
    float value = irradiance * exp(-radius * radius);
    outColor = vec4(value * starColor.xyz, 1.0);
}
