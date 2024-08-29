#version 410 core

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

out vec3 normal;
out vec3 FragPos;
out vec3 vertexColor;

uniform mat4 projection;
uniform mat4 model;
uniform mat4 view;

in VS_OUT {
    vec3 fragmentPos;
    vec3 color;
} gs_in[];

const float MAGNITUDE = 0.3;

vec3 GetNormal()
{
    vec3 a = vec3(gl_in[0].gl_Position) - vec3(gl_in[1].gl_Position);
    vec3 b = vec3(gl_in[2].gl_Position) - vec3(gl_in[1].gl_Position);
    return normalize(cross(b, a));
}

void main()
{
    normal = mat3(transpose(inverse(model))) * GetNormal();

    for (int i = 0; i < 3; i++)
    {
        gl_Position = projection * view * model * vec4(gs_in[i].fragmentPos, 1.0);
        FragPos = vec3(model * vec4(gs_in[i].fragmentPos, 1.0));
        vertexColor = gs_in[i].color;
        EmitVertex();
    }
    
    EndPrimitive();
}
