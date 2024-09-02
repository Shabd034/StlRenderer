#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Renderer 
{
public:
    Renderer();
    void StartRender(int width, int height, const float vertices[], const int vertexCount);
    GLFWwindow* GetWindow();

private:
    void SetupBuffers(const float vertices[]);
    void RenderScene();
    void ProcessInput();
    
    unsigned int VAO, VBO;
    GLFWwindow* gWindow;
};