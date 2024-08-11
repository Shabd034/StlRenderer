#include "Renderer.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"

float gFov = 45.0f;
float gCameraX = 0.0f;
float gCameraY = 0.0f;
float gAngleX = 0.0f;
float gAngleY = 0.0f;
bool gIsDragging = false;
double gLastX = 0;
double gLastY = 0;
Shader* pShaderProgram = nullptr;

// Mouse button callback
static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods) 
{
    if (button == GLFW_MOUSE_BUTTON_LEFT) 
    {
        if (action == GLFW_PRESS) 
        {
            gIsDragging = true;
            glfwGetCursorPos(window, &gLastX, &gLastY);
        }
        else if (action == GLFW_RELEASE)
        {
            gIsDragging = false;
        }
    }
}

// Cursor position callback
static void CursorPositionCallback(GLFWwindow* window, double xpos, double ypos) 
{
    // TODO
}

// Window resize callback
static void Reshape(GLFWwindow* window, int width, int height) 
{
   glViewport(0, 0, width, height);
}

Renderer::Renderer()
{
    if (!glfwInit()) 
    {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        exit(EXIT_FAILURE);
    }
}

void Renderer::SetupBuffers() 
{
    float vertices[] = {
    // positions          // colors (RGB)
    -0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,  // Red
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,  // Red
     0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 0.0f,  // Red
     0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 0.0f,  // Red
    -0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 0.0f,  // Red
    -0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,  // Red

    -0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 0.0f,  // Green
     0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 0.0f,  // Green
     0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0f,  // Green
     0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0f,  // Green
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0f,  // Green
    -0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 0.0f,  // Green

    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1.0f,  // Blue
    -0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 1.0f,  // Blue
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 1.0f,  // Blue
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 1.0f,  // Blue
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1.0f,  // Blue
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1.0f,  // Blue

     0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.0f,  // Yellow
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f,  // Yellow
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 0.0f,  // Yellow
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 0.0f,  // Yellow
     0.5f, -0.5f,  0.5f,  1.0f, 1.0f, 0.0f,  // Yellow
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.0f,  // Yellow

    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 1.0f,  // Cyan
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 1.0f,  // Cyan
     0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 1.0f,  // Cyan
     0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 1.0f,  // Cyan
    -0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 1.0f,  // Cyan
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 1.0f,  // Cyan

    -0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 1.0f,  // Magenta
     0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 1.0f,  // Magenta
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 1.0f,  // Magenta
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 1.0f,  // Magenta
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 1.0f,  // Magenta
    -0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 1.0f,  // Magenta
};


   // Generate and bind the Vertex Array Object (VAO)
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);

    // Bind and set the vertex buffer data
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Unbind the buffer and the vertex array
    glBindBuffer(GL_ARRAY_BUFFER, 0); 
    glBindVertexArray(0); 
}

void Renderer::RenderScene() 
{
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    int width, height;
    glfwGetFramebufferSize(gWindow, &width, &height);

    Shader shaderProgram = *pShaderProgram;
    shaderProgram.Use();
    
    // create transformations
    glm::mat4 model         = glm::mat4(1.0f);
    glm::mat4 view          = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
    glm::mat4 projection    = glm::mat4(1.0f);

    model      = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));
    projection = glm::perspective(glm::radians(gFov), ((float)width / (float)height), 0.1f, 100.0f);

    // Might be a better way to do movement using camera
    view       = glm::translate(view, glm::vec3(gCameraX, gCameraY, -3.0f));

    // pass them to the shaders
    shaderProgram.setMat4("model", model);
    shaderProgram.setMat4("view", view);
    shaderProgram.setMat4("projection", projection);

    // draw our first triangle
    glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
    glDrawArrays(GL_TRIANGLES, 0, 36);

    glfwSwapBuffers(gWindow);
}

void Renderer::ProcessInput() 
{
    int width, height;
    glfwGetFramebufferSize(gWindow, &width, &height);

    if (glfwGetKey(gWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(gWindow, true);

    if (glfwGetKey(gWindow, GLFW_KEY_I) == GLFW_PRESS && gFov > 10.0f) 
    {
        gFov -= 0.5f;
        Reshape(gWindow, width, height);
    }

    if (glfwGetKey(gWindow, GLFW_KEY_O) == GLFW_PRESS && gFov < 100.0f) 
    {
        gFov += 0.5f;
        Reshape(gWindow, width, height);
    }

    if (glfwGetKey(gWindow, GLFW_KEY_LEFT) == GLFW_PRESS) 
    {
        gCameraX += 0.05f;
        Reshape(gWindow, width, height);
    }

    if (glfwGetKey(gWindow, GLFW_KEY_RIGHT) == GLFW_PRESS) 
    {
        gCameraX -= 0.05f;
        Reshape(gWindow, width, height);
    }

    if (glfwGetKey(gWindow, GLFW_KEY_UP) == GLFW_PRESS) 
    {
        gCameraY -= 0.05f;
        Reshape(gWindow, width, height);
    }

    if (glfwGetKey(gWindow, GLFW_KEY_DOWN) == GLFW_PRESS) 
    {
        gCameraY += 0.05f;
        Reshape(gWindow, width, height);
    }

    if (glfwGetKey(gWindow, GLFW_KEY_R) == GLFW_PRESS) 
    {
        gCameraX = 0.0f;
        gCameraY = 0.0f;
        gAngleX = 0.0f;
        gAngleY = 0.0f;
        gFov = 45.0f;
        Reshape(gWindow, width, height);
    }
}

void Renderer::StartRender(int width, int height) 
{
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    gWindow = glfwCreateWindow(width, height, "OpenGL 4.1 Cube", NULL, NULL);
    if (!gWindow) 
    {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return;
    }

    glfwMakeContextCurrent(gWindow);
    glfwSetFramebufferSizeCallback(gWindow, Reshape);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        glfwTerminate();
        return;
    }

    // Set the mouse button callback
    glfwSetMouseButtonCallback(gWindow, MouseButtonCallback);

    // Set the cursor position callback
    glfwSetCursorPosCallback(gWindow, CursorPositionCallback);

    Shader shaderProgram = Shader("vertex_shader.glsl", "fragment_shader.glsl");
    pShaderProgram = &shaderProgram;
    SetupBuffers();

    glEnable(GL_DEPTH_TEST);

    while (!glfwWindowShouldClose(gWindow)) 
    {
        ProcessInput();
        RenderScene();
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    shaderProgram.Delete();

    const GLubyte* renderer = glGetString(GL_RENDERER); // Get renderer string
    const GLubyte* version = glGetString(GL_VERSION); // Get OpenGL version string
    const GLubyte* glslVersion = glGetString(GL_SHADING_LANGUAGE_VERSION); // Get GLSL version string

    std::cout << "Renderer: " << renderer << std::endl;
    std::cout << "OpenGL version supported: " << version << std::endl;
    std::cout << "GLSL version supported: " << glslVersion << std::endl;

    glfwTerminate();
}

GLFWwindow* Renderer::GetWindow() 
{
    return gWindow;
}