#ifndef CELESTIALBODY_H
#define CELESTIALBODY_H

#include <SDL3/SDL.h>
#include <string>
#include "Vector3D.h"
#include "Matrix4x4.h"
#include "Sphere.h"

class CelestialBody;

class CelestialBody {
private:
    std::string name;
    
    // Transformation
    Vector3D position;
    Vector3D rotation;
    float visualRadius;
    
    // Orbite
    float orbitalRadius;     // Distance moyenne au centre (demi-grand axe)
    float orbitalSpeed;      // Vitesse orbitale (rad/s)
    float rotationSpeed;     // Vitesse rotation propre (rad/s)
    float currentAngle;      // Angle actuel sur l'orbite
    float currentRotation;   // Rotation actuelle sur lui-même
    
    // Orbites elliptiques
    float eccentricity;      // Excentricité de l'orbite (0 = cercle, 0.5 = ellipse)
    
    // Inclinaisons
    float axialTilt;         // Inclinaison de l'axe de rotation
    float orbitalInclination;// Inclinaison du plan orbital
    
    // Hiérarchie (pour les lunes)
    CelestialBody* parentBody;  // Corps parent (nullptr si orbite autour du soleil)
    
    // Apparence
    SDL_Color color;
    bool isSun;
    bool hasSaturnRings;     // Pour Saturne uniquement
    
    // Géométrie
    Sphere* sphereMesh;
    
public:
    CelestialBody(const std::string& name,
                  float visualRadius,
                  float orbitalRadius,
                  float orbitalSpeed,
                  float rotationSpeed,
                  SDL_Color color,
                  bool isSun = false,
                  float eccentricity = 0.0f,
                  float axialTilt = 0.0f,
                  CelestialBody* parent = nullptr);
    
    ~CelestialBody();
    
    /**
     * @brief Met à jour la position et la rotation en fonction du temps écoulé
     * @param deltaTime Temps écoulé en secondes
     */
    void update(float deltaTime);
    
    /**
     * @brief Calcule la matrice de transformation model
     */
    Matrix4x4 getModelMatrix() const;
    
    /**
      Obtenir le mesh
     */
    const Sphere* getMesh() const;
    
    // Getters
    std::string getName() const;
    Vector3D getPosition() const;
    SDL_Color getColor() const;
    bool getIsSun() const;
    float getOrbitalRadius() const;
    float getCurrentAngle() const;
    float getVisualRadius() const;
    float getEccentricity() const;
    float getAxialTilt() const;
    CelestialBody* getParent() const;
    bool hasRings() const;
    
    // Setters
    void setHasRings(bool rings);
    void setOrbitalInclination(float inclination);
};

#endif 