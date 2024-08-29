#version 410 core

layout (triangles) in;
layout (line_strip, max_vertices = 2) out;

const float MAGNITUDE = 0.3;

uniform mat4 projection;

vec3 GetNormal()
{
    vec3 a = vec3(gl_in[0].gl_Position) - vec3(gl_in[1].gl_Position);
    vec3 b = vec3(gl_in[2].gl_Position) - vec3(gl_in[1].gl_Position);
    return normalize(cross(b, a));
}

void main()
{
    vec3 normal = GetNormal();
    vec3 center = (vec3(gl_in[0].gl_Position.xyz)+ vec3(gl_in[1].gl_Position.xyz) + vec3(gl_in[2].gl_Position.xyz)) / 3.0;
    // Emit the center
    gl_Position = projection * vec4(center, 1.0);
    EmitVertex();
    
    vec3 end = center + (normal * MAGNITUDE);
    // Emit the end of the normal line
    gl_Position = projection * vec4(end, 1.0);
    EmitVertex();
    
    EndPrimitive();
}
