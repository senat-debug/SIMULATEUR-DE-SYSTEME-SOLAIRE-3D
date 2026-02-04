#include "SolarSystem.h"
#include "Constants.h"
#include <cmath>

SolarSystem::SolarSystem() : sun(nullptr), earth(nullptr), moon(nullptr) {}

SolarSystem::~SolarSystem() {
    for (auto body : bodies) {
        delete body;
    }
    bodies.clear();
}

void SolarSystem::initialize() {
    // ========================================
    // SOLEIL
    // ========================================
    sun = new CelestialBody(
        "Soleil",
        Constants::SUN_RADIUS,
        0.0f,  // Pas d'orbite
        0.0f,  // Pas de mouvement orbital
        Constants::SUN_ROTATION_SPEED,
        Constants::SUN_COLOR,
        true   // C'est le soleil
    );
    bodies.push_back(sun);
    
    // ========================================
    // MERCURE
    // ========================================
    bodies.push_back(new CelestialBody(
        "Mercure",
        Constants::MERCURY_RADIUS,
        Constants::MERCURY_ORBIT,
        Constants::MERCURY_ORBITAL_SPEED,
        Constants::PLANET_ROTATION_SPEED,
        Constants::MERCURY_COLOR,
        false,
        Constants::MERCURY_ECCENTRICITY,
        Constants::MERCURY_AXIAL_TILT
    ));
    
    // ========================================
    // VÉNUS
    // ========================================
    bodies.push_back(new CelestialBody(
        "Venus",
        Constants::VENUS_RADIUS,
        Constants::VENUS_ORBIT,
        Constants::VENUS_ORBITAL_SPEED,
        Constants::PLANET_ROTATION_SPEED * 0.1f,  // Rotation très lente
        Constants::VENUS_COLOR,
        false,
        Constants::VENUS_ECCENTRICITY,
        Constants::VENUS_AXIAL_TILT
    ));
    
    // ========================================
    // TERRE
    // ========================================
    earth = new CelestialBody(
        "Terre",
        Constants::EARTH_RADIUS,
        Constants::EARTH_ORBIT,
        Constants::EARTH_ORBITAL_SPEED,
        Constants::PLANET_ROTATION_SPEED,
        Constants::EARTH_COLOR,
        false,
        Constants::EARTH_ECCENTRICITY,
        Constants::EARTH_AXIAL_TILT
    );
    bodies.push_back(earth);
    
    // ========================================
    // LUNE (orbite autour de la Terre)
    // ========================================
    moon = new CelestialBody(
        "Lune",
        Constants::MOON_RADIUS,
        Constants::MOON_ORBIT,
        Constants::MOON_ORBITAL_SPEED,
        Constants::PLANET_ROTATION_SPEED * 0.5f,
        Constants::MOON_COLOR,
        false,
        Constants::MOON_ECCENTRICITY,
        Constants::MOON_AXIAL_TILT,
        earth  // Parent = Terre
    );
    bodies.push_back(moon);
    
    // ========================================
    // MARS
    // ========================================
    bodies.push_back(new CelestialBody(
        "Mars",
        Constants::MARS_RADIUS,
        Constants::MARS_ORBIT,
        Constants::MARS_ORBITAL_SPEED,
        Constants::PLANET_ROTATION_SPEED * 1.03f,
        Constants::MARS_COLOR,
        false,
        Constants::MARS_ECCENTRICITY,
        Constants::MARS_AXIAL_TILT
    ));
    
    // ========================================
    // JUPITER
    // ========================================
    bodies.push_back(new CelestialBody(
        "Jupiter",
        Constants::JUPITER_RADIUS,
        Constants::JUPITER_ORBIT,
        Constants::JUPITER_ORBITAL_SPEED,
        Constants::PLANET_ROTATION_SPEED * 2.4f,  // Rotation rapide
        Constants::JUPITER_COLOR,
        false,
        Constants::JUPITER_ECCENTRICITY,
        Constants::JUPITER_AXIAL_TILT
    ));
    
    // ========================================
    // SATURNE (avec anneaux)
    // ========================================
    CelestialBody* saturn = new CelestialBody(
        "Saturne",
        Constants::SATURN_RADIUS,
        Constants::SATURN_ORBIT,
        Constants::SATURN_ORBITAL_SPEED,
        Constants::PLANET_ROTATION_SPEED * 2.2f,
        Constants::SATURN_COLOR,
        false,
        Constants::SATURN_ECCENTRICITY,
        Constants::SATURN_AXIAL_TILT
    );
    saturn->setHasRings(true);  // Activer les anneaux !
    bodies.push_back(saturn);
    
    // ========================================
    // URANUS (inclinaison extrême ~98°)
    // ========================================
    bodies.push_back(new CelestialBody(
        "Uranus",
        Constants::URANUS_RADIUS,
        Constants::URANUS_ORBIT,
        Constants::URANUS_ORBITAL_SPEED,
        Constants::PLANET_ROTATION_SPEED * 1.4f,
        Constants::URANUS_COLOR,
        false,
        Constants::URANUS_ECCENTRICITY,
        Constants::URANUS_AXIAL_TILT
    ));
    
    // ========================================
    // NEPTUNE
    // ========================================
    bodies.push_back(new CelestialBody(
        "Neptune",
        Constants::NEPTUNE_RADIUS,
        Constants::NEPTUNE_ORBIT,
        Constants::NEPTUNE_ORBITAL_SPEED,
        Constants::PLANET_ROTATION_SPEED * 1.5f,
        Constants::NEPTUNE_COLOR,
        false,
        Constants::NEPTUNE_ECCENTRICITY,
        Constants::NEPTUNE_AXIAL_TILT
    ));
}

void SolarSystem::update(float deltaTime) {
    for (auto body : bodies) {
        body->update(deltaTime);
    }
}

void SolarSystem::draw(Rasterizer* rasterizer, Camera* camera) {
    Matrix4x4 view = camera->getViewMatrix();
    Matrix4x4 projection = camera->getProjectionMatrix();
    
    // Dessiner chaque corps céleste
    for (auto body : bodies) {
        Matrix4x4 model = body->getModelMatrix();
        
        rasterizer->drawMesh(
            body->getMesh(),
            model,
            view,
            projection,
            body->getColor()
        );
    }
}

void SolarSystem::drawOrbits(SDL_Renderer* renderer, Camera* camera, int screenWidth, int screenHeight) {
    Matrix4x4 viewProj = camera->getViewProjectionMatrix();
    
    SDL_SetRenderDrawColor(renderer, 
                          Constants::ORBIT_COLOR.r, 
                          Constants::ORBIT_COLOR.g, 
                          Constants::ORBIT_COLOR.b, 
                          Constants::ORBIT_COLOR.a);
    
    // Dessiner l'orbite de chaque planète (pas les lunes pour simplifier)
    for (const auto& body : bodies) {
        if (body->getIsSun()) continue;
        if (body->getParent() != nullptr) continue;  // Skip lunes
        
        float semiMajorAxis = body->getOrbitalRadius();
        float eccentricity = body->getEccentricity();
        int segments = 128;
        
        for (int i = 0; i < segments; i++) {
            float angle1 = i * Constants::TWO_PI / segments;
            float angle2 = (i + 1) * Constants::TWO_PI / segments;
            
            // Calcul de la position sur l'ellipse
            float r1 = semiMajorAxis * (1.0f - eccentricity * eccentricity) / 
                       (1.0f + eccentricity * std::cos(angle1));
            float r2 = semiMajorAxis * (1.0f - eccentricity * eccentricity) / 
                       (1.0f + eccentricity * std::cos(angle2));
            
            Vector3D p1(std::cos(angle1) * r1, 0.0f, std::sin(angle1) * r1);
            Vector3D p2(std::cos(angle2) * r2, 0.0f, std::sin(angle2) * r2);
            
            Vector3D proj1 = viewProj.transformPoint(p1);
            Vector3D proj2 = viewProj.transformPoint(p2);
            
            // Convertir en coordonnées écran
            float x1 = (proj1.x + 1.0f) * 0.5f * screenWidth;
            float y1 = (1.0f - proj1.y) * 0.5f * screenHeight;
            float x2 = (proj2.x + 1.0f) * 0.5f * screenWidth;
            float y2 = (1.0f - proj2.y) * 0.5f * screenHeight;
            
            // Clip test basique
            if (x1 >= 0 && x1 < screenWidth && y1 >= 0 && y1 < screenHeight &&
                x2 >= 0 && x2 < screenWidth && y2 >= 0 && y2 < screenHeight) {
                SDL_RenderLine(renderer, x1, y1, x2, y2);
            }
        }
    }
}

const std::vector<CelestialBody*>& SolarSystem::getBodies() const {
    return bodies;
}

CelestialBody* SolarSystem::getSun() const {
    return sun;
}

CelestialBody* SolarSystem::getEarth() const {
    return earth;
}

CelestialBody* SolarSystem::getMoon() const {
    return moon;
}

int SolarSystem::getTotalTriangleCount() const {
    int total = 0;
    for (const auto& body : bodies) {
        total += body->getMesh()->getTriangleCount();
    }
    return total;
}