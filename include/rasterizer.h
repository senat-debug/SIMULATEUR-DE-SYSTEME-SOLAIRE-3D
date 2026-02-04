#ifndef RASTERIZER_H
#define RASTERIZER_H

#include <SDL3/SDL.h>
#include <vector>
#include "Vector3D.h"
#include "Matrix4x4.h"
#include "Mesh.h"

class Rasterizer {
private:
    SDL_Renderer* renderer;
    int width;
    int height;
    
    // Z-Buffer pour gestion de profondeur
    std::vector<float> zBuffer;
    
    // Éclairage
    Vector3D lightDirection;
    Vector3D cameraPosition;  // Position caméra pour éclairage spéculaire
    SDL_Color lightColor;
    float ambientStrength;
    float specularStrength;   // Force de la composante spéculaire
    int shininess;            // Brillance (exposant spéculaire)
    
    // Mode wireframe
    bool wireframeMode;
    
    /**
     * @brief Structure pour un vertex projeté
     */
    struct ProjectedVertex {
        float x, y, z;  // Position écran + profondeur
        Vector3D normal;
        Vector3D worldPos;  // Position monde pour éclairage
        float u, v;
        bool visible;
    };
    
    /**
     * @brief Nettoie le Z-Buffer
     */
    void clearZBuffer();
    
    /**
     * @brief Test de profondeur
     */
    bool depthTest(int x, int y, float depth);
    
    /**
     * @brief Dessine une ligne (algorithme de Bresenham)
     */
    void drawLine(int x0, int y0, int x1, int y1, SDL_Color color);
    
    /**
     * @brief Remplit un triangle
     */
    void fillTriangle(const ProjectedVertex& v0, 
                     const ProjectedVertex& v1, 
                     const ProjectedVertex& v2,
                     SDL_Color baseColor);
    
    /**
     * @brief Calcule l'éclairage Phong pour une normale
     */
    SDL_Color calculateLighting(const Vector3D& normal, const Vector3D& worldPos, SDL_Color baseColor);
    
    /**
     * @brief Projette un vertex 3D en 2D
     */
    ProjectedVertex projectVertex(const Vertex& vertex, const Matrix4x4& mvp, const Matrix4x4& model);
    
public:
    Rasterizer(SDL_Renderer* renderer, int width, int height);
    ~Rasterizer();
    
    /**
     * @brief Nettoie l'écran et le z-buffer
     */
    void clear();
    
    /**
     * @brief Redimensionne le rasterizer
     */
    void resize(int width, int height);
    
    /**
     * @brief Configure la direction de la lumière
     */
    void setLightDirection(const Vector3D& dir);
    
    /**
     * @brief Configure la position de la caméra
     */
    void setCameraPosition(const Vector3D& pos);
    
    /**
     * @brief Dessine un mesh complet
     */
    void drawMesh(const Mesh* mesh,
                  const Matrix4x4& model,
                  const Matrix4x4& view,
                  const Matrix4x4& projection,
                  SDL_Color color);
    
    /**
     * @brief Dessine un effet de glow autour d'un point
     */
    void drawGlow(int centerX, int centerY, float radius, SDL_Color color, int layers = 8);
    
    /**
     * @brief Dessine les anneaux d'une planète (Saturne)
     */
    void drawRings(const Vector3D& planetPos, float innerRadius, float outerRadius,
                   const Matrix4x4& viewProj, float tilt, SDL_Color color);
    
    /**
     * @brief Toggle wireframe mode
     */
    void setWireframeMode(bool enabled);
    bool isWireframeMode() const;
    
    int getLastTriangleCount() const;
    int getWidth() const { return width; }
    int getHeight() const { return height; }
    
private:
    int lastTriangleCount;
    int culledTriangleCount;
};

#endif 