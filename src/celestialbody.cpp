#include "celestialbody.h"
#include "constants.h"
#include <cmath>

CelestialBody::CelestialBody(const std::string& name,
                             float visualRadius,
                             float orbitalRadius,
                             float orbitalSpeed,
                             float rotationSpeed,
                             SDL_Color color,
                             bool isSun,
                             float eccentricity,
                             float axialTilt,
                             CelestialBody* parent)
    : name(name)
    , visualRadius(visualRadius)
    , orbitalRadius(orbitalRadius)
    , orbitalSpeed(orbitalSpeed)
    , rotationSpeed(rotationSpeed)
    , currentAngle(0.0f)
    , currentRotation(0.1f)
    , eccentricity(eccentricity)
    , axialTilt(axialTilt)
    , orbitalInclination(0.0f)
    , parentBody(parent)
    , color(color)
    , isSun(isSun)
    , hasSaturnRings(false) {
    
    // Créer la géométrie
    if (isSun) {
        sphereMesh = new Sphere(visualRadius, Constants::SUN_SECTORS, Constants::SUN_STACKS);
    } else {
        sphereMesh = new Sphere(visualRadius, Constants::SPHERE_SECTORS, Constants::SPHERE_STACKS);
    }
    
    // Position initiale
    // Pour les orbites elliptiques : r = a(1-e²)/(1+e*cos(θ))
    float r = orbitalRadius;
    if (eccentricity > 0.0f) {
        r = orbitalRadius * (1.0f - eccentricity * eccentricity) / 
            (1.0f + eccentricity * std::cos(currentAngle));
    }
    
    position.x = std::cos(currentAngle) * r;
    position.y = 0.0f;
    position.z = std::sin(currentAngle) * r;
    
    rotation = Vector3D::zero();
}

CelestialBody::~CelestialBody() {
    delete sphereMesh;
}

void CelestialBody::update(float deltaTime) {
    if (!isSun) {
        // 1. Mise à jour de l'angle orbital
        currentAngle += orbitalSpeed * deltaTime;
        
        // Garder l'angle dans [0, 2π]
        if (currentAngle > Constants::TWO_PI) {
            currentAngle -= Constants::TWO_PI;
        }
        
        // 2. Calculer la distance radiale pour orbite elliptique
        // Formule : r = a(1-e²)/(1+e*cos(θ)) où a = demi-grand axe
        float r = orbitalRadius;
        if (eccentricity > 0.0f) {
            r = orbitalRadius * (1.0f - eccentricity * eccentricity) / 
                (1.0f + eccentricity * std::cos(currentAngle));
        }
        
        // 3. Calculer la position de base sur l'orbite
        float localX = std::cos(currentAngle) * r;
        float localZ = std::sin(currentAngle) * r;
        float localY = 0.0f;
        
        // 4. Appliquer l'inclinaison orbitale
        if (orbitalInclination != 0.0f) {
            float cosInc = std::cos(orbitalInclination);
            float sinInc = std::sin(orbitalInclination);
            localY = localZ * sinInc;
            localZ = localZ * cosInc;
        }
        
        // 5. Position finale (relative au parent si c'est une lune)
        if (parentBody != nullptr) {
            Vector3D parentPos = parentBody->getPosition();
            position.x = parentPos.x + localX;
            position.y = parentPos.y + localY;
            position.z = parentPos.z + localZ;
        } else {
            position.x = localX;
            position.y = localY;
            position.z = localZ;
        }
    }
    
    // 6. Rotation propre de la planète sur elle-même
    currentRotation += rotationSpeed * deltaTime;
    rotation.y = currentRotation;
}

Matrix4x4 CelestialBody::getModelMatrix() const {
    // Créer les matrices individuelles
    Matrix4x4 translation = Matrix4x4::translation(position.x, position.y, position.z);
    Matrix4x4 rotationY = Matrix4x4::rotationY(rotation.y);
    
    // Appliquer l'inclinaison axiale (rotation autour de X)
    Matrix4x4 tiltMatrix = Matrix4x4::rotationX(axialTilt);
    
    // L'ordre : Translation * Tilt * Rotation
    return translation * tiltMatrix * rotationY;
}

const Sphere* CelestialBody::getMesh() const {
    return sphereMesh;
}

std::string CelestialBody::getName() const {
    return name;
}

Vector3D CelestialBody::getPosition() const {
    return position;
}

SDL_Color CelestialBody::getColor() const {
    return color;
}

bool CelestialBody::getIsSun() const {
    return isSun;
}

float CelestialBody::getOrbitalRadius() const {
    return orbitalRadius;
}

float CelestialBody::getCurrentAngle() const {
    return currentAngle;
}

float CelestialBody::getVisualRadius() const {
    return visualRadius;
}

float CelestialBody::getEccentricity() const {
    return eccentricity;
}

float CelestialBody::getAxialTilt() const {
    return axialTilt;
}

CelestialBody* CelestialBody::getParent() const {
    return parentBody;
}

bool CelestialBody::hasRings() const {
    return hasSaturnRings;
}

void CelestialBody::setHasRings(bool rings) {
    hasSaturnRings = rings;
}

void CelestialBody::setOrbitalInclination(float inclination) {
    orbitalInclination = inclination;
}