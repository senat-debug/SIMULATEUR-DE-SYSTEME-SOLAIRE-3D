#ifndef MESH_H
#define MESH_H

#include "Vector3D.h"
#include <vector>

/**
 * @struct Vertex
 * @brief Structure représentant un sommet 3D
 */
struct Vertex {
    Vector3D position;  // Position 3D
    Vector3D normal;    // Normale (pour l'éclairage)
    float u, v;         // Coordonnées de texture
    
    Vertex() : position(), normal(), u(0.0f), v(0.0f) {}
    Vertex(const Vector3D& pos) : position(pos), normal(), u(0.0f), v(0.0f) {}
    Vertex(const Vector3D& pos, const Vector3D& norm, float u, float v) 
        : position(pos), normal(norm), u(u), v(v) {}
};

/**
 * @struct Triangle
 * @brief Structure représentant un triangle (3 indices de vertices)
 */
struct Triangle {
    int indices[3];  // Indices des 3 vertices
    Vector3D normal; // Normale du triangle (calculée)
    
    Triangle() : indices{0, 0, 0}, normal() {}
    Triangle(int i0, int i1, int i2) : indices{i0, i1, i2}, normal() {}
};

/**
 * @class Mesh
 * @brief Classe de base pour une géométrie 3D
 */
class Mesh {
protected:
    std::vector<Vertex> vertices;    // Liste des sommets
    std::vector<int> indices;        // Liste des indices (3 par triangle)
    
public:
    Mesh();
    virtual ~Mesh();
    
    /**
     * @brief Génère la géométrie (à implémenter dans les classes dérivées)
     */
    virtual void generate() = 0;
    
    /**
     * @brief Calcule les normales des vertices
     */
    void calculateNormals();
    
    /**
     * @brief Obtenir les vertices
     */
    const std::vector<Vertex>& getVertices() const;
    
    /**
     * @brief Obtenir les indices
     */
    const std::vector<int>& getIndices() const;
    
    /**
     * @brief Obtenir le nombre de triangles
     */
    int getTriangleCount() const;
    
    /**
     * @brief Nettoyer la géométrie
     */
    void clear();
    
    /**
     * @brief Afficher les statistiques du mesh (debug)
     */
    void printStats() const;
};

#endif // MESH_H