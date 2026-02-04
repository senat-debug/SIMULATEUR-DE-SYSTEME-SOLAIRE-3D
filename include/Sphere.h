#ifndef SPHERE_H
#define SPHERE_H

#include "Mesh.h"

/**
 * @class Sphere
 * @brief Génère une sphère 3D par subdivision latitude/longitude
 */
// Correction d
class Sphere : public Mesh {

private:
    float radius;
    int sectors;  // Subdivisions horizontales (longitude)
    int stacks;   // Subdivisions verticales (latitude)
    
public:
    /**
     * @brief Constructeur
     * @param radius Rayon de la sphère
     * @param sectors Nombre de subdivisions horizontales (recommandé: 18-36)
     * @param stacks Nombre de subdivisions verticales (recommandé: 12-24)
     */
    Sphere(float radius = 1.0f, int sectors = 18, int stacks = 12);
    
    /**
     * @brief Destructeur
     */
    ~Sphere();
    
    /**
     * @brief Génère la géométrie de la sphère
     */
    void generate() override;
    
    /**
     * @brief Modifie le rayon et régénère
     */
    void setRadius(float r);
    
    /**
     * @brief Obtenir le rayon
     */
    float getRadius() const;
};

#endif // SPHERE_H