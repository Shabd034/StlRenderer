/*
Acknowledgements:
- The code is based on the tutorial from learnopengl.com
- Amazing OpenGL tutorial
*/

#include <iostream>
#include <fstream>
#include <vector>

#include "Renderer.h"

std::vector<float> CalculateCentroid(const std::vector<float>& points) 
{
    std::vector<float> centroid(3, 0.0f); // Initialize centroid to (0, 0, 0)
    
    if (points.size() % 3 != 0 || points.empty()) 
    {
        std::cerr << "Invalid points data or no points provided." << std::endl;
        return centroid; // Return (0, 0, 0) if data is invalid
    }

    float sumX = 0.0f, sumY = 0.0f, sumZ = 0.0f;
    size_t numPoints = points.size() / 3;
    
    // Accumulate the sum of all points' coordinates
    for (size_t i = 0; i < points.size(); i += 3) 
    {
        sumX += points[i];
        sumY += points[i + 1];
        sumZ += points[i + 2];
    }

    // Compute the average position
    centroid[0] = sumX / numPoints;
    centroid[1] = sumY / numPoints;
    centroid[2] = sumZ / numPoints;

    return centroid;
}

// Function to adjust points so that the centroid is at the origin
void AdjustPointsToCenter(std::vector<float>& points) 
{
    std::vector<float> centroid = CalculateCentroid(points);

    for (size_t i = 0; i < points.size(); i += 3) 
    {
        points[i] -= centroid[0];
        points[i + 1] -= centroid[1];
        points[i + 2] -= centroid[2];
    }
}

std::vector<float> parseSTL(const std::string& filePath) 
{
    std::vector<float> vertices; // Vector to store all vertices (x, y, z) sequentially

    // Open the file in binary mode
    std::ifstream file(filePath, std::ios::binary);
    if (!file) {
        std::cerr << "Error: Unable to open file " << filePath << std::endl;
        return vertices;
    }

    // Skip the 80-byte header
    file.seekg(80);

    // Read the number of triangles (4 bytes)
    uint32_t numTriangles = 0;
    file.read(reinterpret_cast<char*>(&numTriangles), sizeof(numTriangles));

    // Read each triangle
    for (uint32_t i = 0; i < numTriangles; ++i) {
        // Skip the normal vector (3 floats)
        file.seekg(12, std::ios::cur);

        // Read the 3 vertices of the triangle
        for (int j = 0; j < 3; ++j) {
            float vertex[3]; // Array to hold a single vertex (x, y, z)
            file.read(reinterpret_cast<char*>(vertex), sizeof(vertex));
            vertices.insert(vertices.end(), vertex, vertex + 3); // Append the vertex to the vector
        }

        // Skip the attribute byte count (2 bytes)
        file.seekg(2, std::ios::cur);
    }

    file.close();
    return vertices;
}

int main()
{
    // Change file as required
    std::string filePath = "<path_to_file>.stl";

    std::vector<float> vertices = parseSTL(filePath); 

    float maxValue = 0.0f;
    for (size_t i = 0; i < vertices.size(); ++i) 
    {
        float value = vertices[i];
        if (value < 0)
        {
            value = -value;
        }

        if (value > maxValue)
        {
            maxValue = value;
        }
    }

    for (size_t i = 0; i < vertices.size(); ++i) 
    {
        vertices[i] /= maxValue;
        vertices[i] *= 1.50f;
    }


    AdjustPointsToCenter(vertices);
    int vertexCount = vertices.size();

    Renderer renderer;
    renderer.StartRender(800, 600, vertices.data(), vertexCount);

    return 0;
}