#version 410 core

layout (location = 0) in vec3 aPos;

uniform mat4 view;
uniform mat4 model;

void main()
{
    mat3 normalMatrix = mat3(transpose(inverse(view * model)));
    gl_Position = view * model * vec4(aPos, 1.0);
}