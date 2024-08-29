#version 410 core

layout (location = 0) in vec3 aPos;

uniform vec3 color;

out VS_OUT {
    vec3 fragmentPos;
    vec3 color;
} vs_out;

void main()
{
    gl_Position = vec4(aPos, 1.0);
    vs_out.color = color;
    vs_out.fragmentPos = aPos;
}