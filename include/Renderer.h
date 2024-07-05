#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Renderer {
public:
    Renderer();
    void StartRender(int width, int height);
    GLFWwindow* GetWindow();

private:
    void LoadShaders();
    void SetupBuffers();
    void RenderScene();
    void ProcessInput();
    void Display();
    void Reshape(int width, int height);
    
    GLuint shaderProgram;
    unsigned int VAO, VBO, EBO;
    GLFWwindow* gWindow;
    float gFov;
    float gCameraX, gCameraY;
    float gAngleX, gAngleY;
    bool gIsDragging;
    double gLastX, gLastY;
};