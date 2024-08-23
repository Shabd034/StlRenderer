#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Renderer 
{
public:
    Renderer();
    void StartRender(int width, int height);
    GLFWwindow* GetWindow();

private:
    void SetupBuffers();
    void RenderScene();
    void ProcessInput();
    
    unsigned int VAO, VBO, lightVAO;
    GLFWwindow* gWindow;
};