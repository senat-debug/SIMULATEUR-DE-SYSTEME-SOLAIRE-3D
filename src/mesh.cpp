#include "Mesh.h"
#include <iostream>

Mesh::Mesh() {}

Mesh::~Mesh() {
    clear();
}

void Mesh::calculateNormals() {
    // Réinitialiser toutes les normales à zéro
    for (auto& vertex : vertices) {
        vertex.normal = Vector3D::zero();
    }
    
    // Pour chaque triangle, calculer sa normale et l'ajouter aux vertices
    for (size_t i = 0; i < indices.size(); i += 3) {
        int i0 = indices[i];
        int i1 = indices[i + 1];
        int i2 = indices[i + 2];
        
        // Vérifier les indices valides
        if (i0 >= (int)vertices.size() || i1 >= (int)vertices.size() || i2 >= (int)vertices.size()) {
            continue;
        }
        
        // Récupérer les positions
        const Vector3D& v0 = vertices[i0].position;
        const Vector3D& v1 = vertices[i1].position;
        const Vector3D& v2 = vertices[i2].position;
        
        // Calculer les edges
        Vector3D edge1 = v1 - v0;
        Vector3D edge2 = v2 - v0;
        
        // Normale du triangle (produit vectoriel)
        Vector3D triangleNormal = edge1.cross(edge2);
        
        // Ajouter cette normale aux vertices du triangle
        vertices[i0].normal += triangleNormal;
        vertices[i1].normal += triangleNormal;
        vertices[i2].normal += triangleNormal;
    }
    
    // Normaliser toutes les normales
    for (auto& vertex : vertices) {
        vertex.normal.normalize();
    }
}

const std::vector<Vertex>& Mesh::getVertices() const {
    return vertices;
}

const std::vector<int>& Mesh::getIndices() const {
    return indices;
}

int Mesh::getTriangleCount() const {
    return indices.size() / 3;
}

void Mesh::clear() {
    vertices.clear();
    indices.clear();
}

void Mesh::printStats() const {
    std::cout << "Mesh Stats:" << std::endl;
    std::cout << "  Vertices: " << vertices.size() << std::endl;
    std::cout << "  Triangles: " << getTriangleCount() << std::endl;
    std::cout << "  Indices: " << indices.size() << std::endl;
}