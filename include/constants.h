#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <SDL3/SDL.h>

/**
 * @file Constants.h
 * @brief Toutes les constantes du projet en un seul endroit
 */

namespace Constants {
    
    // ======================
    // FENÊTRE
    // ======================
    constexpr int WINDOW_WIDTH = 1280;
    constexpr int WINDOW_HEIGHT = 720;
    constexpr const char* WINDOW_TITLE = "Solar System Simulator 3D";
    
    // ======================
    // FPS ET TIMING
    // ======================
    constexpr int TARGET_FPS = 60;
    constexpr float FIXED_TIMESTEP = 1.0f / TARGET_FPS;
    constexpr int FRAME_DELAY_MS = 1000 / TARGET_FPS;  // ~16ms
    
    // ======================
    // MATHÉMATIQUES
    // ======================
    constexpr float PI = 3.14159265358979323846f;
    constexpr float TWO_PI = 2.0f * PI;
    constexpr float HALF_PI = PI / 2.0f;
    constexpr float DEG_TO_RAD = PI / 180.0f;
    constexpr float RAD_TO_DEG = 180.0f / PI;
    constexpr float EPSILON = 0.0001f;  // Pour comparaisons float
    
    // ======================
    // CAMÉRA
    // ======================
    constexpr float CAMERA_FOV = 60.0f;              // Field of view en degrés
    constexpr float CAMERA_NEAR = 0.1f;              // Plan proche
    constexpr float CAMERA_FAR = 2000.0f;            // Plan lointain (augmenté pour Neptune)
    constexpr float CAMERA_DISTANCE = 400.0f;        // Distance initiale (augmentée)
    constexpr float CAMERA_MIN_DISTANCE = 50.0f;     // Zoom max (plus proche)
    constexpr float CAMERA_MAX_DISTANCE = 1000.0f;   // Zoom min (plus loin pour Neptune)
    constexpr float CAMERA_ROTATION_SPEED = 0.005f;  // Sensibilité souris
    constexpr float CAMERA_ZOOM_SPEED = 15.0f;       // Vitesse zoom molette (augmentée)
    
    // ======================
    // QUALITÉ DES SPHÈRES
    // ======================
    // IMPORTANT : Valeurs optimisées pour software rendering
    constexpr int SPHERE_SECTORS = 18;   // Subdivisions horizontales
    constexpr int SPHERE_STACKS = 12;    // Subdivisions verticales
    // Total triangles par sphère : sectors * stacks * 2 = 432
    
    // Pour le Soleil (plus de détails)
    constexpr int SUN_SECTORS = 24;
    constexpr int SUN_STACKS = 16;
    
    // ======================
    // TAILLES DES CORPS CÉLESTES
    // ======================
    constexpr float SUN_RADIUS = 15.0f;
    constexpr float MERCURY_RADIUS = 3.0f;
    constexpr float VENUS_RADIUS = 5.0f;
    constexpr float EARTH_RADIUS = 5.5f;
    constexpr float MARS_RADIUS = 4.0f;
    constexpr float JUPITER_RADIUS = 12.0f;
    constexpr float SATURN_RADIUS = 10.0f;
    constexpr float URANUS_RADIUS = 7.0f;
    constexpr float NEPTUNE_RADIUS = 6.5f;
    constexpr float MOON_RADIUS = 1.5f;
    
    // ======================
    // DISTANCES ORBITALES
    // ======================
    constexpr float MERCURY_ORBIT = 50.0f;
    constexpr float VENUS_ORBIT = 80.0f;
    constexpr float EARTH_ORBIT = 110.0f;
    constexpr float MARS_ORBIT = 150.0f;
    constexpr float JUPITER_ORBIT = 200.0f;
    constexpr float SATURN_ORBIT = 260.0f;
    constexpr float URANUS_ORBIT = 320.0f;
    constexpr float NEPTUNE_ORBIT = 380.0f;
    constexpr float MOON_ORBIT = 15.0f;  // Distance de la Terre
    
    // ======================
    // VITESSES ORBITALES (basées sur les vraies périodes)
    // Terre = 1.0, autres proportionnelles
    // ======================
    constexpr float MERCURY_ORBITAL_SPEED = 1.0f / 0.24f;   // ~4.17
    constexpr float VENUS_ORBITAL_SPEED = 1.0f / 0.62f;     // ~1.61
    constexpr float EARTH_ORBITAL_SPEED = 1.0f;
    constexpr float MARS_ORBITAL_SPEED = 1.0f / 1.88f;      // ~0.53
    constexpr float JUPITER_ORBITAL_SPEED = 1.0f / 11.86f;  // ~0.08
    constexpr float SATURN_ORBITAL_SPEED = 1.0f / 29.46f;   // ~0.03
    constexpr float URANUS_ORBITAL_SPEED = 1.0f / 84.01f;   // ~0.01
    constexpr float NEPTUNE_ORBITAL_SPEED = 1.0f / 164.8f;  // ~0.006
    constexpr float MOON_ORBITAL_SPEED = 1.0f / 0.075f;     // ~13.3 (orbite rapide autour Terre)
    
    // ======================
    // EXCENTRICITÉS ORBITALES
    // ======================
    constexpr float MERCURY_ECCENTRICITY = 0.206f;
    constexpr float VENUS_ECCENTRICITY = 0.007f;
    constexpr float EARTH_ECCENTRICITY = 0.017f;
    constexpr float MARS_ECCENTRICITY = 0.093f;
    constexpr float JUPITER_ECCENTRICITY = 0.049f;
    constexpr float SATURN_ECCENTRICITY = 0.057f;
    constexpr float URANUS_ECCENTRICITY = 0.046f;
    constexpr float NEPTUNE_ECCENTRICITY = 0.010f;
    constexpr float MOON_ECCENTRICITY = 0.055f;
    
    // ======================
    // INCLINAISONS AXIALES (radians)
    // ======================
    constexpr float MERCURY_AXIAL_TILT = 0.03f * DEG_TO_RAD;
    constexpr float VENUS_AXIAL_TILT = 177.4f * DEG_TO_RAD;
    constexpr float EARTH_AXIAL_TILT = 23.4f * DEG_TO_RAD;
    constexpr float MARS_AXIAL_TILT = 25.2f * DEG_TO_RAD;
    constexpr float JUPITER_AXIAL_TILT = 3.1f * DEG_TO_RAD;
    constexpr float SATURN_AXIAL_TILT = 26.7f * DEG_TO_RAD;
    constexpr float URANUS_AXIAL_TILT = 97.8f * DEG_TO_RAD;
    constexpr float NEPTUNE_AXIAL_TILT = 28.3f * DEG_TO_RAD;
    constexpr float MOON_AXIAL_TILT = 6.7f * DEG_TO_RAD;
    
    // ======================
    // VITESSES DE ROTATION (rad/s)
    // ======================
    constexpr float SUN_ROTATION_SPEED = 0.1f;
    constexpr float PLANET_ROTATION_SPEED = 2.0f;
    
    // ======================
    // COULEURS (RGBA)
    // ======================
    inline SDL_Color makeColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255) {
        return SDL_Color{r, g, b, a};
    }
    
    const SDL_Color SUN_COLOR = {255, 200, 50, 255};
    const SDL_Color MERCURY_COLOR = {169, 169, 169, 255};
    const SDL_Color VENUS_COLOR = {255, 198, 73, 255};
    const SDL_Color EARTH_COLOR = {100, 149, 237, 255};
    const SDL_Color MARS_COLOR = {193, 68, 14, 255};
    const SDL_Color JUPITER_COLOR = {255, 166, 99, 255};    // Brun-orangé
    const SDL_Color SATURN_COLOR = {238, 197, 145, 255};    // Jaune pâle
    const SDL_Color URANUS_COLOR = {173, 216, 230, 255};    // Bleu clair
    const SDL_Color NEPTUNE_COLOR = {70, 130, 180, 255};    // Bleu profond
    const SDL_Color MOON_COLOR = {192, 192, 192, 255};      // Gris
    const SDL_Color SATURN_RING_COLOR = {210, 180, 140, 180}; // Beige translucide
    const SDL_Color ORBIT_COLOR = {100, 100, 100, 80};
    const SDL_Color BACKGROUND_COLOR = {0, 0, 0, 255};
    const SDL_Color WHITE = {255, 255, 255, 255};
    
    // ======================
    // ÉCLAIRAGE
    // ======================
    const SDL_Color LIGHT_COLOR = {255, 255, 255, 255};
    constexpr float AMBIENT_STRENGTH = 0.2f;   // Lumière ambiante
    constexpr float DIFFUSE_STRENGTH = 0.8f;   // Lumière diffuse
    
    // ======================
    // SIMULATION
    // ======================
    constexpr float DEFAULT_SIMULATION_SPEED = 1.0f;
    constexpr float MIN_SIMULATION_SPEED = 0.1f;
    constexpr float MAX_SIMULATION_SPEED = 5.0f;
    
    // ======================
    // RENDU
    // ======================
    constexpr int MAX_STARS = 500;  // Nombre d'étoiles de fond
    
} // namespace Constants

#endif // CONSTANTS_H