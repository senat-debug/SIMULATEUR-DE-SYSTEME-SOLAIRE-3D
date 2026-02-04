#ifndef RENDERER3D_H
#define RENDERER3D_H

#include <SDL3/SDL.h>
#include <vector>
#include "Vector3D.h"
#include "Rasterizer.h"
#include "SolarSystem.h"
#include "Camera.h"


class Renderer3D {
private:
    SDL_Renderer* sdlRenderer;
    Rasterizer* rasterizer;
    
    int screenWidth;
    int screenHeight;
    
    bool showOrbits;
    bool showStars;
    bool showInfo;
    bool showGlow;  // Effet glow du Soleil
    
    // Étoiles de fond
    struct Star {
        Vector3D position;
        float brightness;
        float size;
        SDL_Color color;  // Couleur variée
    };
    std::vector<Star> stars;
    
    /**
     * @brief Génère les étoiles de fond
     */
    void generateStars();
    
    /**
     * @brief Dessine les étoiles
     */
    void renderStars(Camera* camera);
    
    /**
     * @brief Dessine le glow du Soleil
     */
    void renderSunGlow(Camera* camera, CelestialBody* sun);
    
    /**
     * @brief Dessine les anneaux de Saturne
     */
    void renderSaturnRings(Camera* camera, CelestialBody* saturn);
    
    /**
     * @brief Dessine un texte (pour debug/info)
     */
    void drawText(const std::string& text, int x, int y);
    
public:
    Renderer3D(SDL_Renderer* renderer, int width, int height);
    ~Renderer3D();
    
    /**
     * @brief Nettoie l'écran
     */
    void clear();
    
    /**
     * @brief Présente le frame
     */
    void present();
    
    /**
     * @brief Redimensionne
     */
    void resize(int width, int height);
    
    /**
     * @brief Rendu complet du système solaire
     */
    void render(SolarSystem* solarSystem, Camera* camera);
    
    /**
     * @brief Affiche les informations à l'écran
     */
    void renderInfo(int fps, int triangles, float simSpeed, CelestialBody* selectedBody);
    
    // Toggles
    void toggleOrbits() { showOrbits = !showOrbits; }
    void toggleStars() { showStars = !showStars; }
    void toggleInfo() { showInfo = !showInfo; }
    void toggleGlow() { showGlow = !showGlow; }
    
    bool areOrbitsVisible() const { return showOrbits; }
    bool areStarsVisible() const { return showStars; }
    bool isInfoVisible() const { return showInfo; }
    bool isGlowVisible() const { return showGlow; }
    
    Rasterizer* getRasterizer() { return rasterizer; }
};

#endif // RENDERER3D_H