#include "Renderer.h"

int main()
{
    float vertices[] = 
    {
        // Back face
        -0.5f, -0.5f, -0.5f, // Bottom-left
        0.5f,  0.5f, -0.5f, // top-right
        0.5f, -0.5f, -0.5f, // bottom-right         
        0.5f,  0.5f, -0.5f, // top-right
        -0.5f, -0.5f, -0.5f, // bottom-left
        -0.5f,  0.5f, -0.5f, // top-left
        // Front face
        -0.5f, -0.5f,  0.5f, // bottom-left
        0.5f, -0.5f,  0.5f, // bottom-right
        0.5f,  0.5f,  0.5f, // top-right
        0.5f,  0.5f,  0.5f, // top-right
        -0.5f,  0.5f,  0.5f, // top-left
        -0.5f, -0.5f,  0.5f, // bottom-left
        // Left face
        -0.5f,  0.5f,  0.5f, // top-right
        -0.5f,  0.5f, -0.5f, // top-left
        -0.5f, -0.5f, -0.5f, // bottom-left
        -0.5f, -0.5f, -0.5f, // bottom-left
        -0.5f, -0.5f,  0.5f, // bottom-right
        -0.5f,  0.5f,  0.5f, // top-right
        // Right face
        0.5f,  0.5f,  0.5f, // top-left
        0.5f, -0.5f, -0.5f, // bottom-right
        0.5f,  0.5f, -0.5f, // top-right         
        0.5f, -0.5f, -0.5f, // bottom-right
        0.5f,  0.5f,  0.5f, // top-left
        0.5f, -0.5f,  0.5f, // bottom-left     
        // Bottom face
        -0.5f, -0.5f, -0.5f, // top-right
        0.5f, -0.5f, -0.5f, // top-left
        0.5f, -0.5f,  0.5f, // bottom-left
        0.5f, -0.5f,  0.5f, // bottom-left
        -0.5f, -0.5f,  0.5f, // bottom-right
        -0.5f, -0.5f, -0.5f, // top-right
        // Top face
        -0.5f,  0.5f, -0.5f, // top-left
        0.5f,  0.5f,  0.5f, // bottom-right
        0.5f,  0.5f, -0.5f, // top-right     
        0.5f,  0.5f,  0.5f, // bottom-right
        -0.5f,  0.5f, -0.5f, // top-left
        -0.5f,  0.5f,  0.5f // bottom-left
    };


    Renderer renderer;
    renderer.StartRender(800, 600, vertices, 36);

    return 0;
}