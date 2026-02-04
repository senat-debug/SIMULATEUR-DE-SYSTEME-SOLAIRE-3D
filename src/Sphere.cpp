#include "Sphere.h"
#include "Constants.h"
#include <cmath>


Sphere::Sphere(float radius, int sectors, int stacks)
    : radius(radius), sectors(sectors), stacks(stacks) {
    generate();
}

Sphere::~Sphere() {
    clear();
}

void Sphere::generate() {
    clear();
    
    float sectorStep = Constants::TWO_PI / sectors;
    float stackStep = Constants::PI / stacks;
    
    // Générer les vertices
    for (int i = 0; i <= stacks; ++i) {
        float stackAngle = Constants::PI / 2.0f - i * stackStep;  // De π/2 à -π/2
        float xy = radius * std::cos(stackAngle);                 // r * cos(u)
        float z = radius * std::sin(stackAngle);                  // r * sin(u)
        
        for (int j = 0; j <= sectors; ++j) {
            float sectorAngle = j * sectorStep;                   // De 0 à 2π
            
            Vertex vertex;
            
            // Position
            vertex.position.x = xy * std::cos(sectorAngle);       // r * cos(u) * cos(v)
            vertex.position.y = xy * std::sin(sectorAngle);       // r * cos(u) * sin(v)
            vertex.position.z = z;                                // r * sin(u)
            
            // Normale (pour une sphère centrée à l'origine)
            vertex.normal = vertex.position.normalized();
            
            // Coordonnées de texture (UV mapping)
            vertex.u = (float)j / sectors;
            vertex.v = (float)i / stacks;
            
            vertices.push_back(vertex);
        }
    }
    
    // Générer les indices (triangles)
    for (int i = 0; i < stacks; ++i) {
        int k1 = i * (sectors + 1);       // Début de la rangée actuelle
        int k2 = k1 + sectors + 1;        // Début de la rangée suivante
        
        for (int j = 0; j < sectors; ++j, ++k1, ++k2) {
            // 2 triangles par quad
            
            // Triangle 1
            if (i != 0) {
                indices.push_back(k1);
                indices.push_back(k2);
                indices.push_back(k1 + 1);
            }
            
            // Triangle 2
            if (i != (stacks - 1)) {
                indices.push_back(k1 + 1);
                indices.push_back(k2);
                indices.push_back(k2 + 1);
            }
        }
    }
}

void Sphere::setRadius(float r) {
    radius = r;
    generate();
}

float Sphere::getRadius() const {
    return radius;
}