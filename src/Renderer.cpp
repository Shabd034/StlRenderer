#include "Renderer.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"
#include "Materials.h"

//#define TESTING
//#define BLINN_PHONG

int gNumTriangles = 0;

float gFov = 45.0f;
float gCameraX = 0.0f;
float gCameraY = 0.0f;
float gCameraZ = -3.0f;

float gAngleX = 0.0f;
float gAngleY = 0.0f;

glm::mat4 gRotationMatrix = glm::mat4(1.0f); // Start with identity matrix

bool gIsDragging = false;
double gLastX = 0;
double gLastY = 0;

Shader* pShaderProgram = nullptr;
#ifdef TESTING
Shader* pNormalShaderProgram = nullptr;
#endif

// positions of the point lights
glm::vec3 gPointLightPositions[] = 
{
    glm::vec3(-2.5f,  -2.0f,  -1.0f),
    glm::vec3(2.5f, -2.0f, -1.0f),
    glm::vec3(0.0f,  3.0f, 2.0f)
};

#ifdef BLINN_PHONG
int gMaterialType = ALUMINUM;
#else
int gMaterialType = RED;
#endif
bool gButtonHeld = false; // Used to prevent multiple key presses for material change

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

void UpdateRotation(float xOffset, float yOffset) 
{
    glm::vec3 xAxis = glm::vec3(1.0f, 0.0f, 0.0f);
    glm::vec3 yAxis = glm::vec3(0.0f, 1.0f, 0.0f);

    // Create rotation matrices for x and y offsets
    glm::mat4 rotX = glm::rotate(glm::mat4(1.0f), glm::radians(yOffset), xAxis);
    glm::mat4 rotY = glm::rotate(glm::mat4(1.0f), glm::radians(xOffset), yAxis);

    // Combine the rotations by multiplying them in the correct order
    gRotationMatrix = rotX * rotY * gRotationMatrix;
}

//Cursor position callback
static void CursorPositionCallback(GLFWwindow* window, double xpos, double ypos) 
{
    static double lastX = xpos, lastY = ypos;
    double xOffset = xpos - lastX;
    double yOffset = ypos - lastY;
    lastX = xpos;
    lastY = ypos;

    // Sensitivity factor
    float sensitivity = 0.3f;
    xOffset *= sensitivity;
    yOffset *= sensitivity;

    // Update rotation angles
    if (gIsDragging) 
    {
        UpdateRotation(xOffset, yOffset);
    }
}

static void MouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
    float sensitivity = 0.5f;
    gFov -= (float)sensitivity * yoffset;
    if (gFov < 1.0f)
        gFov = 1.0f;
    if (gFov > 90.0f)
        gFov = 90.0f;
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

void Renderer::SetupBuffers(const float vertices[]) 
{
    // Generate and bind the Vertex Array Object (VAO)
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);

    // Bind and set the vertex buffer data
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, gNumTriangles * 3 * sizeof(float), vertices, GL_STATIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Unbind the buffer and the vertex array
    glBindBuffer(GL_ARRAY_BUFFER, 0); 
    glBindVertexArray(0); 
}

static void SetLighting()
{
    Shader shaderProgram = *pShaderProgram;

#ifdef BLINN_PHONG

    // directional light
    shaderProgram.setVec3("dirLight.direction", -1.0f, -1.0f, -1.0f);
    shaderProgram.setVec3("dirLight.ambient", 0.05f, 0.05f, 0.05f);
    shaderProgram.setVec3("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
    shaderProgram.setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);
    // point light 1
    shaderProgram.setVec3("pointLights[0].position", gPointLightPositions[0]);
    shaderProgram.setVec3("pointLights[0].ambient", 0.05f, 0.05f, 0.05f);
    shaderProgram.setVec3("pointLights[0].diffuse", 0.8f, 0.8f, 0.8f);
    shaderProgram.setVec3("pointLights[0].specular", 1.0f, 1.0f, 1.0f);
    shaderProgram.setFloat("pointLights[0].constant", 1.0f);
    shaderProgram.setFloat("pointLights[0].linear", 0.22f);
    shaderProgram.setFloat("pointLights[0].quadratic", 0.20f);
    // point light 2
    shaderProgram.setVec3("pointLights[1].position", gPointLightPositions[1]);
    shaderProgram.setVec3("pointLights[1].ambient", 0.05f, 0.05f, 0.05f);
    shaderProgram.setVec3("pointLights[1].diffuse", 0.8f, 0.8f, 0.8f);
    shaderProgram.setVec3("pointLights[1].specular", 1.0f, 1.0f, 1.0f);
    shaderProgram.setFloat("pointLights[1].constant", 1.0f);
    shaderProgram.setFloat("pointLights[1].linear", 0.22f);
    shaderProgram.setFloat("pointLights[1].quadratic", 0.20f);
    // point light 3
    shaderProgram.setVec3("pointLights[2].position", gPointLightPositions[2]);
    shaderProgram.setVec3("pointLights[2].ambient", 0.05f, 0.05f, 0.05f);
    shaderProgram.setVec3("pointLights[2].diffuse", 0.8f, 0.8f, 0.8f);
    shaderProgram.setVec3("pointLights[2].specular", 1.0f, 1.0f, 1.0f);
    shaderProgram.setFloat("pointLights[2].constant", 1.0f);
    shaderProgram.setFloat("pointLights[2].linear", 0.09f);
    shaderProgram.setFloat("pointLights[2].quadratic", 0.032f);

#else
    shaderProgram.setVec3("lightColors[0]", 300.0f, 300.0f, 300.0f);
    shaderProgram.setVec3("lightPositions[0]", -10.0f,  10.0f, 10.0f);

    shaderProgram.setVec3("lightColors[1]", 300.0f, 300.0f, 300.0f);
    shaderProgram.setVec3("lightPositions[1]", 10.0f,  10.0f, 10.0f);

    shaderProgram.setVec3("lightColors[2]", 300.0f, 300.0f, 300.0f);
    shaderProgram.setVec3("lightPositions[2]", -10.0f, -10.0f, 10.0f);

    shaderProgram.setVec3("lightColors[3]", 300.0f, 300.0f, 300.0f);
    shaderProgram.setVec3("lightPositions[3]", 10.0f, -10.0f, 10.0f);
#endif
}

void Renderer::RenderScene() 
{
    glClearColor(0.53f, 0.81f, 0.92f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    int width, height;
    glfwGetFramebufferSize(gWindow, &width, &height);

    Shader shaderProgram = *pShaderProgram;
    // create transformations
    glm::mat4 model         = gRotationMatrix;
    glm::mat4 view          = glm::mat4(1.0f);
    glm::mat4 projection    = glm::mat4(1.0f);

    projection = glm::perspective(glm::radians(gFov), ((float)width / (float)height), 0.1f, 100.0f);
    view       = glm::translate(view, glm::vec3(gCameraX, gCameraY, gCameraZ));

    shaderProgram.Use();
    // pass them to the shaders
    shaderProgram.setMat4("model", model);
    shaderProgram.setMat4("view", view);
    shaderProgram.setMat4("projection", projection);

    glm::mat4 viewInverse = glm::inverse(view);
    glm::vec3 cameraPosition = glm::vec3(viewInverse[3]);
    shaderProgram.setVec3("viewPos", cameraPosition);

#ifdef BLINN_PHONG

    // Material properties
    Material material = materialsTypes[gMaterialType];
    shaderProgram.setVec3("material.ambient", material.ambient);
    shaderProgram.setVec3("material.diffuse", material.diffuse);
    shaderProgram.setVec3("material.specular", material.specular);
    shaderProgram.setFloat("material.shininess", material.shininess);
    shaderProgram.setVec3("color", material.color);

#else

    // Material properties
    Metal metal = metals[gMaterialType];
    shaderProgram.setVec3("albedo", metal.albedo);
    shaderProgram.setFloat("roughness", metal.roughness);
    shaderProgram.setFloat("metallic", metal.metallic);
    shaderProgram.setFloat("ao", metal.ao);

#endif

    // Light properties
    SetLighting();

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, gNumTriangles);

#ifdef TESTING
    Shader normalShaderProgram = *pNormalShaderProgram;
    normalShaderProgram.Use();
    normalShaderProgram.setMat4("projection", projection);
    normalShaderProgram.setMat4("view", view);
    normalShaderProgram.setMat4("model", model);

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, gNumTriangles);
#endif
    
    glfwSwapBuffers(gWindow);
}

void Renderer::ProcessInput() 
{
    int width, height;
    glfwGetFramebufferSize(gWindow, &width, &height);

    if (glfwGetKey(gWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(gWindow, true);

    if (glfwGetKey(gWindow, GLFW_KEY_M) == GLFW_PRESS) 
    {
        if (!gButtonHeld)
        {
            gButtonHeld = true;
            gMaterialType++;
#ifdef BLINN_PHONG
            if (gMaterialType >= MATERIAL_COUNT)
                gMaterialType = 0;
#else
            if (gMaterialType >= METAL_COUNT)
                gMaterialType = 0;
#endif
        }
    }

    if (gButtonHeld && glfwGetKey(gWindow, GLFW_KEY_M) == GLFW_RELEASE) 
    {
        gButtonHeld = false;
    }

    if (glfwGetKey(gWindow, GLFW_KEY_R) == GLFW_PRESS) 
    {
        gCameraX = 0.0f;
        gCameraY = 0.0f;
        gCameraZ = -3.0f;
        gAngleX = 0.0f;
        gAngleY = 0.0f;
        gFov = 45.0f;
        gRotationMatrix = glm::mat4(1.0f);
        Reshape(gWindow, width, height);
    }
}

void Renderer::StartRender(int width, int height, const float vertices[], const int vertexCount) 
{
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SAMPLES, 4);
    
    gWindow = glfwCreateWindow(width, height, "STL Renderer", NULL, NULL);
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

    // Set the scroll callback
    glfwSetScrollCallback(gWindow, MouseScrollCallback);

#ifdef BLINN_PHONG
    Shader shaderProgram = Shader("geometry_shader.glsl", "vertex_shader.glsl", "blinn_phong_fragment_shader.glsl");
#else
    Shader shaderProgram = Shader("geometry_shader.glsl", "vertex_shader.glsl", "pbr_fragment_shader.glsl");
#endif

#ifdef TESTING
    Shader normalShaderProgram = Shader("visualize_normal_geometry.glsl", "visualize_normal_vertex.glsl", "visualize_normal_fragment.glsl");
    pNormalShaderProgram = &normalShaderProgram;
#endif
    pShaderProgram = &shaderProgram;
    gNumTriangles = vertexCount / 3;
    SetupBuffers(vertices);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);

#ifdef BLINN_PHONG
    glEnable(GL_FRAMEBUFFER_SRGB);
#endif

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);

    while (!glfwWindowShouldClose(gWindow)) 
    {
        ProcessInput();
        RenderScene();
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    shaderProgram.Delete();

#ifdef TESTING
    normalShaderProgram.Delete();
#endif

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
