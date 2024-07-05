#include "Renderer.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Renderer::Renderer() : gFov(45.0f), gCameraX(0.0f), gCameraY(0.0f), gAngleX(0.0f), gAngleY(0.0f), gIsDragging(false), gLastX(0.0), gLastY(0.0) 
{
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        exit(EXIT_FAILURE);
    }
}

// Utility function to read shader source code from a file
std::string ReadShaderSource(const std::string& filePath) 
{
    std::ifstream file(filePath);
    if (!file.is_open()) {
        std::cerr << "Failed to open shader file: " << filePath << std::endl;
        exit(EXIT_FAILURE);
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

void Renderer::LoadShaders() 
{
    // Read vertex shader source code
    std::string vertexShaderSource = ReadShaderSource("vertex_shader.glsl");
    const char* vertexShaderCode = vertexShaderSource.c_str();

    // Read fragment shader source code
    std::string fragmentShaderSource = ReadShaderSource("fragment_shader.glsl");
    const char* fragmentShaderCode = fragmentShaderSource.c_str();

    // Compile vertex shader
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderCode, NULL);
    glCompileShader(vertexShader);

    // Check for vertex shader compile errors
    GLint success;
    GLchar infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // Compile fragment shader
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderCode, NULL);
    glCompileShader(fragmentShader);

    // Check for fragment shader compile errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // Link shaders into a shader program
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // Check for linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    // Delete the shaders as they're linked into our program now and no longer necessary
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

void Renderer::SetupBuffers() {
    // Define vertices and colors
    GLfloat vertices[] = {
        // Positions          // Colors
         0.0f,  1.0f,  0.0f, 1.0f, 1.0f, 1.0f, // Top point
        -1.0f, -1.0f,  1.0f, 1.0f, 1.0f, 1.0f, // Front left
         1.0f, -1.0f,  1.0f, 1.0f, 1.0f, 1.0f, // Front right
         1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 1.0f, // Back right
        -1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 1.0f// Back left
    };

    GLuint indices[] = {
        // Front face
        0, 1, 2,
        // Right face
        0, 2, 3,
        // Back face
        0, 3, 4,
        // Left face
        0, 4, 1,
        // Bottom face
        1, 2, 3,
        1, 3, 4
    };

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
}

void Renderer::RenderScene() {
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 projection = glm::perspective(glm::radians(gFov), 800.0f / 600.0f, 0.1f, 100.0f);

    glUseProgram(shaderProgram);

    GLint modelLoc = glGetUniformLocation(shaderProgram, "model");
    GLint viewLoc = glGetUniformLocation(shaderProgram, "view");
    GLint projLoc = glGetUniformLocation(shaderProgram, "projection");

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 18, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void Renderer::ProcessInput() {
    int width, height;
    glfwGetFramebufferSize(gWindow, &width, &height);

    if (glfwGetKey(gWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(gWindow, true);

    if (glfwGetKey(gWindow, GLFW_KEY_I) == GLFW_PRESS && gFov > 10.0f) {
        gFov -= 0.5f;
        Reshape(width, height);
    }

    if (glfwGetKey(gWindow, GLFW_KEY_O) == GLFW_PRESS && gFov < 100.0f) {
        gFov += 0.5f;
        Reshape(width, height);
    }

    if (glfwGetKey(gWindow, GLFW_KEY_LEFT) == GLFW_PRESS) {
        gCameraX += 0.05f;
        Reshape(width, height);
    }

    if (glfwGetKey(gWindow, GLFW_KEY_RIGHT) == GLFW_PRESS) {
        gCameraX -= 0.05f;
        Reshape(width, height);
    }

    if (glfwGetKey(gWindow, GLFW_KEY_UP) == GLFW_PRESS) {
        gCameraY -= 0.05f;
        Reshape(width, height);
    }

    if (glfwGetKey(gWindow, GLFW_KEY_DOWN) == GLFW_PRESS) {
        gCameraY += 0.05f;
        Reshape(width, height);
    }

    if (glfwGetKey(gWindow, GLFW_KEY_R) == GLFW_PRESS) {
        gCameraX = 0.0f;
        gCameraY = 0.0f;
        gAngleX = 0.0f;
        gAngleY = 0.0f;
        gFov = 45.0f;
        Reshape(width, height);
    }
}

void Renderer::Display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Apply transformations for camera movement
    glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(gCameraX, gCameraY, -5.0f));
    view = glm::rotate(view, glm::radians(gAngleX), glm::vec3(1.0f, 0.0f, 0.0f));
    view = glm::rotate(view, glm::radians(gAngleY), glm::vec3(0.0f, 1.0f, 0.0f));

    GLint viewLoc = glGetUniformLocation(shaderProgram, "view");
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

    RenderScene();

    glfwSwapBuffers(gWindow);
}

void Renderer::Reshape(int width, int height) {
    glViewport(0, 0, width, height);
    glm::mat4 projection = glm::perspective(glm::radians(gFov), (float)width / (float)height, 0.1f, 100.0f);

    GLint projLoc = glGetUniformLocation(shaderProgram, "projection");
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
}

void Renderer::StartRender(int width, int height) {

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    gWindow = glfwCreateWindow(width, height, "OpenGL 4.1 Pyramid", NULL, NULL);
    if (!gWindow) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return;
    }

    glfwMakeContextCurrent(gWindow);
    
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        glfwTerminate();
        return;
    }

    LoadShaders();
    SetupBuffers();

    glEnable(GL_DEPTH_TEST);

    Reshape(width, height);

    while (!glfwWindowShouldClose(gWindow)) {
        ProcessInput();
        Display();
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteProgram(shaderProgram);

    const GLubyte* renderer = glGetString(GL_RENDERER); // Get renderer string
    const GLubyte* version = glGetString(GL_VERSION); // Get OpenGL version string
    const GLubyte* glslVersion = glGetString(GL_SHADING_LANGUAGE_VERSION); // Get GLSL version string

    std::cout << "Renderer: " << renderer << std::endl;
    std::cout << "OpenGL version supported: " << version << std::endl;
    std::cout << "GLSL version supported: " << glslVersion << std::endl;

    glfwTerminate();
}

GLFWwindow* Renderer::GetWindow() {
    return gWindow;
}
