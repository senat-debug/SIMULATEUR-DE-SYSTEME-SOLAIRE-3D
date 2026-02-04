#include "Rasterizer.h"
#include "constants.h"
#include <algorithm>
#include <cmath>

Rasterizer::Rasterizer(SDL_Renderer* renderer, int width, int height)
    : renderer(renderer)
    , width(width)
    , height(height)
    , lightDirection(0.0f, 0.0f, 1.0f)
    , cameraPosition(0.0f, 0.0f, 300.0f)
    , lightColor{255, 255, 255, 255}
    , ambientStrength(Constants::AMBIENT_STRENGTH)
    , specularStrength(0.5f)
    , shininess(32)
    , wireframeMode(false)
    , lastTriangleCount(0)
    , culledTriangleCount(0) {
    
    zBuffer.resize(width * height, 1e10f);
    lightDirection.normalize();
}

Rasterizer::~Rasterizer() {
    zBuffer.clear();
}

void Rasterizer::clearZBuffer() {
    std::fill(zBuffer.begin(), zBuffer.end(), 1e10f);
}

void Rasterizer::clear() {
    clearZBuffer();
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
}

void Rasterizer::resize(int w, int h) {
    width = w;
    height = h;
    zBuffer.resize(width * height, 1e10f);
}

bool Rasterizer::depthTest(int x, int y, float depth) {
    if (x < 0 || x >= width || y < 0 || y >= height) {
        return false;
    }
    
    int index = y * width + x;
    if (depth < zBuffer[index]) {
        zBuffer[index] = depth;
        return true;
    }
    
    return false;
}

void Rasterizer::setLightDirection(const Vector3D& dir) {
    lightDirection = dir.normalized();
}

void Rasterizer::setCameraPosition(const Vector3D& pos) {
    cameraPosition = pos;
}

SDL_Color Rasterizer::calculateLighting(const Vector3D& normal, const Vector3D& worldPos, SDL_Color baseColor) {
    // ========================================
    // ÉCLAIRAGE PHONG (Ambient + Diffuse + Specular)
    // ========================================
    
    // 1. Composante ambiante
    float ambient = ambientStrength;
    
    // 2. Composante diffuse (Lambert)
    float diffuse = std::max(0.0f, -normal.dot(lightDirection));
    
    // 3. Composante spéculaire (Blinn-Phong)
    Vector3D viewDir = (cameraPosition - worldPos).normalized();
    Vector3D halfDir = (-lightDirection + viewDir).normalized();
    float spec = std::pow(std::max(0.0f, normal.dot(halfDir)), (float)shininess);
    float specular = specularStrength * spec;
    
    // Combiner toutes les composantes
    float brightness = ambient + diffuse * Constants::DIFFUSE_STRENGTH + specular;
    brightness = std::min(1.5f, brightness);  // Permettre un peu d'overbrightness
    
    return SDL_Color{
        (Uint8)std::min(255, (int)(baseColor.r * brightness)),
        (Uint8)std::min(255, (int)(baseColor.g * brightness)),
        (Uint8)std::min(255, (int)(baseColor.b * brightness)),
        baseColor.a
    };
}

Rasterizer::ProjectedVertex Rasterizer::projectVertex(const Vertex& vertex, const Matrix4x4& mvp, const Matrix4x4& model) {
    ProjectedVertex result;
    
    // Transformer le point
    Vector3D projected = mvp.transformPoint(vertex.position);
    
    // Position monde pour l'éclairage
    result.worldPos = model.transformPoint(vertex.position);
    
    // Convertir en coordonnées écran
    result.x = (projected.x + 1.0f) * 0.5f * width;
    result.y = (1.0f - projected.y) * 0.5f * height;
    result.z = projected.z;
    result.normal = vertex.normal;
    result.u = vertex.u;
    result.v = vertex.v;
    result.visible = (result.x >= -100 && result.x <= width + 100 && 
                     result.y >= -100 && result.y <= height + 100);
    
    return result;
}

void Rasterizer::drawLine(int x0, int y0, int x1, int y1, SDL_Color color) {
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    
    int dx = abs(x1 - x0);
    int dy = abs(y1 - y0);
    int sx = x0 < x1 ? 1 : -1;
    int sy = y0 < y1 ? 1 : -1;
    int err = dx - dy;
    
    int maxSteps = std::max(width, height) * 2;
    int steps = 0;
    
    while (steps++ < maxSteps) {
        SDL_RenderPoint(renderer, x0, y0);
        
        if (x0 == x1 && y0 == y1) break;
        
        int e2 = 2 * err;
        if (e2 > -dy) {
            err -= dy;
            x0 += sx;
        }
        if (e2 < dx) {
            err += dx;
            y0 += sy;
        }
    }
}

void Rasterizer::fillTriangle(const ProjectedVertex& v0,
                               const ProjectedVertex& v1,
                               const ProjectedVertex& v2,
                               SDL_Color baseColor) {
    // Trier les vertices par Y
    ProjectedVertex verts[3] = {v0, v1, v2};
    if (verts[0].y > verts[1].y) std::swap(verts[0], verts[1]);
    if (verts[1].y > verts[2].y) std::swap(verts[1], verts[2]);
    if (verts[0].y > verts[1].y) std::swap(verts[0], verts[1]);
    
    // Normale moyenne et position moyenne pour l'éclairage
    Vector3D avgNormal = (v0.normal + v1.normal + v2.normal) * (1.0f / 3.0f);
    avgNormal.normalize();
    Vector3D avgWorldPos = (v0.worldPos + v1.worldPos + v2.worldPos) * (1.0f / 3.0f);
    SDL_Color litColor = calculateLighting(avgNormal, avgWorldPos, baseColor);
    
    // Scanline rasterization
    int minY = std::max(0, (int)verts[0].y);
    int maxY = std::min(height - 1, (int)verts[2].y);
    
    for (int y = minY; y <= maxY; y++) {
        float t1 = (verts[2].y - verts[0].y) > 0.001f ? 
                   (y - verts[0].y) / (verts[2].y - verts[0].y) : 0.0f;
        
        float x1 = verts[0].x + t1 * (verts[2].x - verts[0].x);
        float z1 = verts[0].z + t1 * (verts[2].z - verts[0].z);
        
        float x2, z2;
        if (y < verts[1].y) {
            float t2 = (verts[1].y - verts[0].y) > 0.001f ? 
                       (y - verts[0].y) / (verts[1].y - verts[0].y) : 0.0f;
            x2 = verts[0].x + t2 * (verts[1].x - verts[0].x);
            z2 = verts[0].z + t2 * (verts[1].z - verts[0].z);
        } else {
            float t2 = (verts[2].y - verts[1].y) > 0.001f ? 
                       (y - verts[1].y) / (verts[2].y - verts[1].y) : 0.0f;
            x2 = verts[1].x + t2 * (verts[2].x - verts[1].x);
            z2 = verts[1].z + t2 * (verts[2].z - verts[1].z);
        }
        
        if (x1 > x2) {
            std::swap(x1, x2);
            std::swap(z1, z2);
        }
        
        int startX = std::max(0, (int)x1);
        int endX = std::min(width - 1, (int)x2);
        
        for (int x = startX; x <= endX; x++) {
            float t = (x2 - x1) > 0.001f ? (x - x1) / (x2 - x1) : 0.0f;
            float z = z1 + t * (z2 - z1);
            
            if (depthTest(x, y, z)) {
                SDL_SetRenderDrawColor(renderer, litColor.r, litColor.g, litColor.b, litColor.a);
                SDL_RenderPoint(renderer, x, y);
            }
        }
    }
}

void Rasterizer::drawMesh(const Mesh* mesh,
                          const Matrix4x4& model,
                          const Matrix4x4& view,
                          const Matrix4x4& projection,
                          SDL_Color color) {
    
    Matrix4x4 mvp = projection * view * model;
    
    const std::vector<Vertex>& vertices = mesh->getVertices();
    const std::vector<int>& indices = mesh->getIndices();
    
    // Projeter tous les vertices
    std::vector<ProjectedVertex> projectedVerts;
    projectedVerts.reserve(vertices.size());
    
    for (const auto& vertex : vertices) {
        // Transformer la normale
        Vector3D transformedNormal = model.transformVector(vertex.normal);
        transformedNormal.normalize();
        
        Vertex transformedVertex = vertex;
        transformedVertex.normal = transformedNormal;
        
        projectedVerts.push_back(projectVertex(transformedVertex, mvp, model));
    }
    
    // Dessiner tous les triangles
    lastTriangleCount = 0;
    culledTriangleCount = 0;
    
    for (size_t i = 0; i < indices.size(); i += 3) {
        int i0 = indices[i];
        int i1 = indices[i + 1];
        int i2 = indices[i + 2];
        
        const ProjectedVertex& p0 = projectedVerts[i0];
        const ProjectedVertex& p1 = projectedVerts[i1];
        const ProjectedVertex& p2 = projectedVerts[i2];
        
        // Backface culling
        float cross = (p1.x - p0.x) * (p2.y - p0.y) - (p1.y - p0.y) * (p2.x - p0.x);
        if (cross < 0) {
            culledTriangleCount++;
            continue;
        }
        
        if (wireframeMode) {
            // Mode fil de fer
            SDL_Color wireColor = {0, 255, 0, 255};
            if (p0.visible && p1.visible) drawLine((int)p0.x, (int)p0.y, (int)p1.x, (int)p1.y, wireColor);
            if (p1.visible && p2.visible) drawLine((int)p1.x, (int)p1.y, (int)p2.x, (int)p2.y, wireColor);
            if (p2.visible && p0.visible) drawLine((int)p2.x, (int)p2.y, (int)p0.x, (int)p0.y, wireColor);
        } else {
            // Mode rempli avec éclairage
            fillTriangle(p0, p1, p2, color);
        }
        
        lastTriangleCount++;
    }
}

void Rasterizer::drawGlow(int centerX, int centerY, float radius, SDL_Color color, int layers) {
    // ========================================
    // EFFET GLOW - Cercles concentriques avec alpha décroissant
    // ========================================
    
    for (int layer = layers; layer >= 0; layer--) {
        float layerRadius = radius * (1.0f + layer * 0.3f);
        float alpha = (float)(layers - layer) / layers;
        alpha = alpha * alpha * 0.6f;  // Courbe d'atténuation quadratique
        
        Uint8 r = (Uint8)(color.r * alpha);
        Uint8 g = (Uint8)(color.g * alpha);
        Uint8 b = (Uint8)(color.b * alpha);
        
        SDL_SetRenderDrawColor(renderer, r, g, b, 255);
        
        // Dessiner un cercle rempli
        for (int y = (int)(centerY - layerRadius); y <= (int)(centerY + layerRadius); y++) {
            if (y < 0 || y >= height) continue;
            
            float dy = y - centerY;
            float dx = std::sqrt(layerRadius * layerRadius - dy * dy);
            
            int startX = std::max(0, (int)(centerX - dx));
            int endX = std::min(width - 1, (int)(centerX + dx));
            
            for (int x = startX; x <= endX; x++) {
                // Distance au centre pour atténuation radiale
                float dist = std::sqrt((x - centerX) * (x - centerX) + dy * dy);
                float falloff = 1.0f - (dist / layerRadius);
                if (falloff > 0) {
                    Uint8 pixelR = (Uint8)(r * falloff);
                    Uint8 pixelG = (Uint8)(g * falloff);
                    Uint8 pixelB = (Uint8)(b * falloff);
                    SDL_SetRenderDrawColor(renderer, pixelR, pixelG, pixelB, 255);
                    SDL_RenderPoint(renderer, x, y);
                }
            }
        }
    }
}

void Rasterizer::drawRings(const Vector3D& planetPos, float innerRadius, float outerRadius,
                           const Matrix4x4& viewProj, float tilt, SDL_Color color) {
    // ========================================
    // ANNEAUX DE SATURNE - Ellipse projetée
    // ========================================
    
    int segments = 180;
    
    for (int ring = 0; ring < 3; ring++) {
        float ringRadius = innerRadius + (outerRadius - innerRadius) * ring / 2.0f;
        Uint8 alpha = (Uint8)(180 - ring * 40);  // Anneaux plus transparents à l'extérieur
        
        for (int i = 0; i < segments; i++) {
            float angle1 = i * Constants::TWO_PI / segments;
            float angle2 = (i + 1) * Constants::TWO_PI / segments;
            
            // Points sur le plan de l'anneau (avec inclinaison)
            float cosTilt = std::cos(tilt);
            float sinTilt = std::sin(tilt);
            
            Vector3D p1(
                planetPos.x + std::cos(angle1) * ringRadius,
                planetPos.y + std::sin(angle1) * ringRadius * sinTilt,
                planetPos.z + std::sin(angle1) * ringRadius * cosTilt
            );
            
            Vector3D p2(
                planetPos.x + std::cos(angle2) * ringRadius,
                planetPos.y + std::sin(angle2) * ringRadius * sinTilt,
                planetPos.z + std::sin(angle2) * ringRadius * cosTilt
            );
            
            // Projection
            Vector3D proj1 = viewProj.transformPoint(p1);
            Vector3D proj2 = viewProj.transformPoint(p2);
            
            // Coordonnées écran
            int x1 = (int)((proj1.x + 1.0f) * 0.5f * width);
            int y1 = (int)((1.0f - proj1.y) * 0.5f * height);
            int x2 = (int)((proj2.x + 1.0f) * 0.5f * width);
            int y2 = (int)((1.0f - proj2.y) * 0.5f * height);
            
            // Clip et dessin
            if (x1 >= 0 && x1 < width && y1 >= 0 && y1 < height &&
                x2 >= 0 && x2 < width && y2 >= 0 && y2 < height) {
                SDL_Color ringColor = {color.r, color.g, color.b, alpha};
                drawLine(x1, y1, x2, y2, ringColor);
            }
        }
    }
}

void Rasterizer::setWireframeMode(bool enabled) {
    wireframeMode = enabled;
}

bool Rasterizer::isWireframeMode() const {
    return wireframeMode;
}

int Rasterizer::getLastTriangleCount() const {
    return lastTriangleCount;
}