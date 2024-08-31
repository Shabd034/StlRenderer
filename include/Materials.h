#include <glm/vec3.hpp> // Assuming you are using glm for vector math

typedef struct MaterialProperties 
{
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    float shininess;
    glm::vec3 color; // Define color as needed
} Material;

typedef enum 
{
    EMERALD,
    JADE,
    OBSIDIAN,
    PEARL,
    RUBY,
    TURQUOISE,
    BRASS,
    BRONZE,
    CHROME,
    COPPER,
    GOLD,
    SILVER,
    ALUMINUM,
    BLACK_PLASTIC,
    CYAN_PLASTIC,
    GREEN_PLASTIC,
    RED_PLASTIC,
    WHITE_PLASTIC,
    YELLOW_PLASTIC,
    BLACK_RUBBER,
    CYAN_RUBBER,
    GREEN_RUBBER,
    RED_RUBBER,
    WHITE_RUBBER,
    YELLOW_RUBBER,
    MATERIAL_COUNT // Helper value to get the count of materials
} MaterialType;

const Material materialsTypes[MATERIAL_COUNT] = 
{
    // Emerald
    { glm::vec3(0.0215f,   0.1745f,   0.0215f),   // Ambient
      glm::vec3(0.07568f,  0.61424f,  0.07568f),  // Diffuse
      glm::vec3(0.633f,    0.727811f, 0.633f),    // Specular
      0.6f,                                      // Shininess
      glm::vec3(0.0f, 1.0f, 0.0f) },             // Color

    // Jade
    { glm::vec3(0.135f,    0.2225f,   0.1575f),   // Ambient
      glm::vec3(0.54f,     0.89f,     0.63f),     // Diffuse
      glm::vec3(0.316228f, 0.316228f, 0.316228f), // Specular
      0.1f,                                      // Shininess
      glm::vec3(0.0f, 1.0f, 0.0f) },             // Color

    // Obsidian
    { glm::vec3(0.05375f,  0.05f,     0.06625f),  // Ambient
      glm::vec3(0.18275f,  0.17f,     0.22525f),  // Diffuse
      glm::vec3(0.332741f, 0.328634f, 0.346435f), // Specular
      0.3f,                                      // Shininess
      glm::vec3(0.0f, 0.0f, 0.0f) },             // Color

    // Pearl
    { glm::vec3(0.25f,     0.20725f,  0.20725f),  // Ambient
      glm::vec3(1.0f,      0.829f,    0.829f),    // Diffuse
      glm::vec3(0.296648f, 0.296648f, 0.296648f), // Specular
      0.088f,                                    // Shininess
      glm::vec3(1.0f, 1.0f, 1.0f) },             // Color

    // Ruby
    { glm::vec3(0.1745f,   0.01175f,  0.01175f),  // Ambient
      glm::vec3(0.61424f,  0.04136f,  0.04136f),  // Diffuse
      glm::vec3(0.727811f, 0.626959f, 0.626959f), // Specular
      0.6f,                                      // Shininess
      glm::vec3(1.0f, 0.0f, 0.0f) },             // Color

    // Turquoise
    { glm::vec3(0.1f,      0.18725f,  0.1745f),   // Ambient
      glm::vec3(0.396f,    0.74151f,  0.69102f),  // Diffuse
      glm::vec3(0.297254f, 0.30829f,  0.306678f), // Specular
      0.1f,                                      // Shininess
      glm::vec3(0.0f, 1.0f, 1.0f) },             // Color

    // Brass
    { glm::vec3(0.329412f, 0.223529f, 0.027451f), // Ambient
      glm::vec3(0.780392f, 0.568627f, 0.113725f), // Diffuse
      glm::vec3(0.992157f, 0.941176f, 0.807843f), // Specular
      0.21794872f,                                // Shininess
      glm::vec3(1.0f, 0.84f, 0.0f) },            // Color

    // Bronze
    { glm::vec3(0.2125f,   0.1275f,   0.054f),    // Ambient
      glm::vec3(0.714f,    0.4284f,   0.18144f),  // Diffuse
      glm::vec3(0.393548f, 0.271906f, 0.166721f), // Specular
      0.2f,                                      // Shininess
      glm::vec3(0.8f, 0.5f, 0.2f) },             // Color

    // Chrome
    { glm::vec3(0.25f,     0.25f,     0.25f),     // Ambient
      glm::vec3(0.4f,      0.4f,      0.4f),      // Diffuse
      glm::vec3(0.774597f, 0.774597f, 0.774597f), // Specular
      0.6f,                                      // Shininess
      glm::vec3(0.75f, 0.75f, 0.75f) },          // Color

    // Copper
    { glm::vec3(0.19125f,  0.0735f,   0.0225f),   // Ambient
      glm::vec3(0.7038f,   0.27048f,  0.0828f),   // Diffuse
      glm::vec3(0.256777f, 0.137622f, 0.086014f), // Specular
      0.1f,                                      // Shininess
      glm::vec3(0.72f, 0.45f, 0.2f) },           // Color

    // Gold
    { glm::vec3(0.24725f,  0.1995f,   0.0745f),   // Ambient
      glm::vec3(0.75164f,  0.60648f,  0.22648f),  // Diffuse
      glm::vec3(0.628281f, 0.555802f, 0.366065f), // Specular
      0.4f,                                      // Shininess
      glm::vec3(1.0f, 0.84f, 0.0f) },            // Color

    // Silver
    { glm::vec3(0.19225f,  0.19225f,  0.19225f),  // Ambient
      glm::vec3(0.50754f,  0.50754f,  0.50754f),  // Diffuse
      glm::vec3(0.508273f, 0.508273f, 0.508273f), // Specular
      0.4f,                                      // Shininess
      glm::vec3(0.75f, 0.75f, 0.75f) },          // Color
    
    // ALUMINUM
    {
        glm::vec3(0.23f, 0.23f, 0.23f),  // Ambient
        glm::vec3(0.55f, 0.55f, 0.55f),  // Diffuse
        glm::vec3(0.70f, 0.70f, 0.70f),  // Specular
        0.6f,                            // Shininess
        glm::vec3(0.75f, 0.75f, 0.75f)   // Color
    },

    // Black Plastic
    { glm::vec3(0.0f,      0.0f,      0.0f),      // Ambient
      glm::vec3(0.01f,     0.01f,     0.01f),     // Diffuse
      glm::vec3(0.50f,     0.50f,     0.50f),     // Specular
      0.25f,                                     // Shininess
      glm::vec3(0.01f, 0.01f, 0.01f) },          // Color

    // Cyan Plastic
    { glm::vec3(0.0f,      0.1f,      0.06f),     // Ambient
      glm::vec3(0.0f,      0.50980392f, 0.50980392f), // Diffuse
      glm::vec3(0.50196078f, 0.50196078f, 0.50196078f), // Specular
      0.25f,                                     // Shininess
      glm::vec3(0.0f, 1.0f, 1.0f) },             // Color

    // Green Plastic
    { glm::vec3(0.0f,      0.0f,      0.0f),      // Ambient
      glm::vec3(0.1f,      0.35f,     0.1f),      // Diffuse
      glm::vec3(0.45f,     0.55f,     0.45f),     // Specular
      0.25f,                                     // Shininess
      glm::vec3(0.0f, 1.0f, 0.0f) },             // Color

    // Red Plastic
    { glm::vec3(0.0f,      0.0f,      0.0f),      // Ambient
      glm::vec3(0.5f,      0.0f,      0.0f),      // Diffuse
      glm::vec3(0.7f,      0.6f,      0.6f),      // Specular
      0.25f,                                     // Shininess
      glm::vec3(1.0f, 0.0f, 0.0f) },             // Color

    // White Plastic
    { glm::vec3(0.0f,      0.0f,      0.0f),      // Ambient
      glm::vec3(0.55f,     0.55f,     0.55f),     // Diffuse
      glm::vec3(0.7f,      0.7f,      0.7f),      // Specular
      0.25f,                                     // Shininess
      glm::vec3(1.0f, 1.0f, 1.0f) },             // Color

    // Yellow Plastic
    { glm::vec3(0.0f,      0.0f,      0.0f),      // Ambient
      glm::vec3(0.5f,      0.5f,      0.0f),      // Diffuse
      glm::vec3(0.60f,     0.60f,     0.50f),     // Specular
      0.25f,                                     // Shininess
      glm::vec3(1.0f, 1.0f, 0.0f) },             // Color

    // Black Rubber
    { glm::vec3(0.02f,     0.02f,     0.02f),     // Ambient
      glm::vec3(0.01f,     0.01f,     0.01f),     // Diffuse
      glm::vec3(0.4f,      0.4f,      0.4f),      // Specular
      0.078125f,                                 // Shininess
      glm::vec3(0.1f, 0.1f, 0.1f) },             // Color

    // Cyan Rubber
    { glm::vec3(0.0f,      0.05f,     0.05f),     // Ambient
      glm::vec3(0.4f,      0.5f,      0.5f),      // Diffuse
      glm::vec3(0.04f,     0.7f,      0.7f),      // Specular
      0.078125f,                                 // Shininess
      glm::vec3(0.0f, 1.0f, 1.0f) },             // Color

    // Green Rubber
    { glm::vec3(0.0f,      0.05f,     0.0f),      // Ambient
      glm::vec3(0.4f,      0.5f,      0.4f),      // Diffuse
      glm::vec3(0.04f,     0.7f,      0.04f),     // Specular
      0.078125f,                                 // Shininess
      glm::vec3(0.0f, 1.0f, 0.0f) },             // Color

    // Red Rubber
    { glm::vec3(0.05f,     0.0f,      0.0f),      // Ambient
      glm::vec3(0.5f,      0.4f,      0.4f),      // Diffuse
      glm::vec3(0.7f,      0.04f,     0.04f),     // Specular
      0.078125f,                                 // Shininess
      glm::vec3(1.0f, 0.0f, 0.0f) },             // Color

    // White Rubber
    { glm::vec3(0.05f,     0.05f,     0.05f),     // Ambient
      glm::vec3(0.5f,      0.5f,      0.5f),      // Diffuse
      glm::vec3(0.7f,      0.7f,      0.7f),      // Specular
      0.078125f,                                 // Shininess
      glm::vec3(1.0f, 1.0f, 1.0f) },             // Color

    // Yellow Rubber
    { glm::vec3(0.05f,     0.05f,     0.0f),      // Ambient
      glm::vec3(0.5f,      0.5f,      0.4f),      // Diffuse
      glm::vec3(0.7f,      0.7f,      0.04f),     // Specular
      0.078125f,                                 // Shininess
      glm::vec3(1.0f, 1.0f, 0.0f) }              // Color
};

typedef enum
{
    RED,
    ALUMINUM_METAL,
    COPPER_METAL,
    SILVER_METAL,
    GOLD_METAL,
    METAL_COUNT // Represents the total number of metals
} MetalType;

// Define the Metal struct
typedef struct MetalInfo {
    glm::vec3 albedo;      // Base color of the metal
    float metallic;   // Metallic factor (typically 1.0 for metals)
    float roughness;  // Roughness of the surface
    float ao;         // Ambient Occlusion (general setting, can be scene-dependent)
} Metal;

// Initialize an array of Metal structs with specific properties
const Metal metals[METAL_COUNT] =
{
    {glm::vec3(0.5, 0.0, 0.0), 0.0, 0.05, 1.0}, // PLASTIC
    {glm::vec3(0.91, 0.92, 0.92), 0.1, 0.2, 1.0}, // ALUMINUM
    {glm::vec3(0.95, 0.64, 0.54), 0.4, 0.25, 1.0}, // COPPER
    {glm::vec3(0.97, 0.96, 0.91), 0.8, 0.15, 1.0}, // SILVER
    {glm::vec3(1.0, 0.766, 0.336), 1.0, 0.25, 1.0}  // GOLD
};