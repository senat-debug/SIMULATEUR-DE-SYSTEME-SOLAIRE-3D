#ifndef SOLARSYSTEM_H
#define SOLARSYSTEM_H

#include <vector>
#include "CelestialBody.h"
#include "Rasterizer.h"
#include "Camera.h"

class SolarSystem {
private:
    std::vector<CelestialBody*> bodies;
    CelestialBody* sun;
    CelestialBody* earth;  // Pour accéder facilement à la Terre (parent de la Lune)
    CelestialBody* moon;   // Pour accéder à la Lune
    
public:
    SolarSystem();
    ~SolarSystem();
    
    
    void initialize();
    
    /**
     * @brief Met à jour tous les corps célestes
     */
    void update(float deltaTime);
    
    /**
     * @brief Dessine le système complet
     */
     void draw(Rasterizer* rasterizer, Camera* camera);
    /**
     * @brief Dessine les orbites (SDL simple)
     */
    void drawOrbits(SDL_Renderer* renderer, Camera* camera, int screenWidth, int screenHeight);
    
    /**
     * @brief Obtenir tous les corps
     */
    const std::vector<CelestialBody*>& getBodies() const;
    
    /**
     * @brief Obtenir le soleil
     */
    CelestialBody* getSun() const;
    CelestialBody* getEarth() const;
    CelestialBody* getMoon() const;
    
    
    int getTotalTriangleCount() const;
};

#endif 