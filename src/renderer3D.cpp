#include "renderer3D.h"
#include "Constants.h"
#include <cmath>
#include <random>

Renderer3D::Renderer3D(SDL_Renderer* renderer, int width, int height)
    : sdlRenderer(renderer)
    , screenWidth(width)
    , screenHeight(height)
    , showOrbits(true)
    , showStars(true)
    , showInfo(true)
    , showGlow(true) {
    
    rasterizer = new Rasterizer(renderer, width, height);
    generateStars();
}

Renderer3D::~Renderer3D() {
    delete rasterizer;
}

void Renderer3D::generateStars() {
    stars.clear();
    
    std::random_device rd;
    std::mt19937 gen(42);  // Seed fixe pour des étoiles cohérentes
    std::uniform_real_distribution<float> posDist(-800.0f, 800.0f);
    std::uniform_real_distribution<float> brightDist(0.3f, 1.0f);
    std::uniform_real_distribution<float> sizeDist(0.5f, 2.5f);
    std::uniform_int_distribution<int> colorType(0, 10);
    
    for (int i = 0; i < Constants::MAX_STARS; i++) {
        Star star;
        star.position = Vector3D(
            posDist(gen),
            posDist(gen),
            posDist(gen)
        );
        star.brightness = brightDist(gen);
        star.size = sizeDist(gen);
        
        // ========================================
        // COULEURS VARIÉES POUR LES ÉTOILES
        // ========================================
        int type = colorType(gen);
        if (type < 6) {
            // 60% Blanches
            star.color = {255, 255, 255, 255};
        } else if (type < 8) {
            // 20% Bleutées (étoiles chaudes)
            star.color = {200, 220, 255, 255};
        } else if (type < 9) {
            // 10% Jaunâtres (étoiles moyennes)
            star.color = {255, 240, 200, 255};
        } else {
            // 10% Rougeâtres (étoiles froides)
            star.color = {255, 200, 180, 255};
        }
        
        stars.push_back(star);
    }
}

void Renderer3D::renderStars(Camera* camera) {
    if (!showStars) return;
    
    Matrix4x4 viewProj = camera->getViewProjectionMatrix();
    
    for (const auto& star : stars) {
        Vector3D projected = viewProj.transformPoint(star.position);
        
        // Convertir en coordonnées écran
        float x = (projected.x + 1.0f) * 0.5f * screenWidth;
        float y = (1.0f - projected.y) * 0.5f * screenHeight;
        
        // Vérifier si visible
        if (x >= 0 && x < screenWidth && y >= 0 && y < screenHeight) {
            Uint8 brightness = (Uint8)(255 * star.brightness);
            SDL_SetRenderDrawColor(sdlRenderer, 
                                  (Uint8)(star.color.r * star.brightness),
                                  (Uint8)(star.color.g * star.brightness),
                                  (Uint8)(star.color.b * star.brightness), 
                                  255);
            
            // Dessiner l'étoile (petit carré ou point)
            if (star.size > 1.5f) {
                SDL_FRect rect = {x - star.size/2, y - star.size/2, star.size, star.size};
                SDL_RenderFillRect(sdlRenderer, &rect);
            } else {
                SDL_RenderPoint(sdlRenderer, x, y);
            }
        }
    }
}

void Renderer3D::renderSunGlow(Camera* camera, CelestialBody* sun) {
    if (!showGlow || !sun) return;
    
    // Projeter la position du Soleil
    Matrix4x4 viewProj = camera->getViewProjectionMatrix();
    Vector3D sunPos = sun->getPosition();
    Vector3D projected = viewProj.transformPoint(sunPos);
    
    // Convertir en coordonnées écran
    int centerX = (int)((projected.x + 1.0f) * 0.5f * screenWidth);
    int centerY = (int)((1.0f - projected.y) * 0.5f * screenHeight);
    
    // Calculer la taille du glow basée sur la distance
    float distance = camera->getDistance();
    float glowRadius = (sun->getVisualRadius() * 3.0f) * (300.0f / distance);
    glowRadius = std::max(20.0f, std::min(100.0f, glowRadius));
    
    // Dessiner le glow AVANT les planètes (il sera partiellement couvert)
    rasterizer->drawGlow(centerX, centerY, glowRadius, Constants::SUN_COLOR, 6);
}

void Renderer3D::renderSaturnRings(Camera* camera, CelestialBody* saturn) {
    if (!saturn || !saturn->hasRings()) return;
    
    Matrix4x4 viewProj = camera->getViewProjectionMatrix();
    Vector3D saturnPos = saturn->getPosition();
    
    float innerRadius = saturn->getVisualRadius() * 1.3f;
    float outerRadius = saturn->getVisualRadius() * 2.2f;
    float tilt = saturn->getAxialTilt();
    
    rasterizer->drawRings(saturnPos, innerRadius, outerRadius, viewProj, tilt, Constants::SATURN_RING_COLOR);
}

void Renderer3D::clear() {
    rasterizer->clear();
}

void Renderer3D::present() {
    SDL_RenderPresent(sdlRenderer);
}

void Renderer3D::resize(int width, int height) {
    screenWidth = width;
    screenHeight = height;
    rasterizer->resize(width, height);
}

void Renderer3D::render(SolarSystem* solarSystem, Camera* camera) {
    // Mettre à jour la position de la caméra dans le rasterizer pour l'éclairage spéculaire
    rasterizer->setCameraPosition(camera->getPosition());
    
    // 1. Fond étoilé
    renderStars(camera);
    
    // 2. Glow du Soleil (AVANT les planètes pour qu'il soit partiellement masqué)
    renderSunGlow(camera, solarSystem->getSun());
    
    // 3. Orbites
    if (showOrbits) {
        solarSystem->drawOrbits(sdlRenderer, camera, screenWidth, screenHeight);
    }
    
    // 4. Corps célestes
    solarSystem->draw(rasterizer, camera);
    
    // 5. Anneaux de Saturne (APRÈS les planètes)
    for (const auto& body : solarSystem->getBodies()) {
        if (body->hasRings()) {
            renderSaturnRings(camera, body);
        }
    }
}

void Renderer3D::renderInfo(int fps, int triangles, float simSpeed, CelestialBody* selectedBody) {
    if (!showInfo) return;
    
    // Dessiner un fond semi-transparent pour le texte
    SDL_SetRenderDrawColor(sdlRenderer, 0, 0, 0, 180);
    SDL_FRect bgRect = {10, 10, 250, 120};
    SDL_RenderFillRect(sdlRenderer, &bgRect);
    
    // Bordure
    SDL_SetRenderDrawColor(sdlRenderer, 0, 255, 0, 255);
    SDL_RenderRect(sdlRenderer, &bgRect);
    
    // Indicateur FPS (barre colorée)
    float fpsRatio = fps / 60.0f;
    if (fpsRatio > 1.0f) fpsRatio = 1.0f;
    
    Uint8 green = (Uint8)(255 * fpsRatio);
    Uint8 red = (Uint8)(255 * (1.0f - fpsRatio));
    SDL_SetRenderDrawColor(sdlRenderer, red, green, 0, 255);
    SDL_FRect fpsBar = {20, 20, 230 * fpsRatio, 10};
    SDL_RenderFillRect(sdlRenderer, &fpsBar);
    
    // Indicateur triangles (barre)
    float triRatio = triangles / 5000.0f;  // Augmenté pour plus de planètes
    if (triRatio > 1.0f) triRatio = 1.0f;
    
    SDL_SetRenderDrawColor(sdlRenderer, 100, 200, 255, 255);
    SDL_FRect triBar = {20, 40, 230 * triRatio, 10};
    SDL_RenderFillRect(sdlRenderer, &triBar);
    
    // Indicateur vitesse simulation
    float speedRatio = simSpeed / Constants::MAX_SIMULATION_SPEED;
    SDL_SetRenderDrawColor(sdlRenderer, 255, 200, 0, 255);
    SDL_FRect speedBar = {20, 60, 230 * speedRatio, 10};
    SDL_RenderFillRect(sdlRenderer, &speedBar);
    
    // Points pour représenter les planètes sélectionnées
    const std::vector<SDL_Color> planetColors = {
        Constants::SUN_COLOR,
        Constants::MERCURY_COLOR,
        Constants::VENUS_COLOR,
        Constants::EARTH_COLOR,
        Constants::MOON_COLOR,
        Constants::MARS_COLOR,
        Constants::JUPITER_COLOR,
        Constants::SATURN_COLOR,
        Constants::URANUS_COLOR,
        Constants::NEPTUNE_COLOR
    };
    
    for (size_t i = 0; i < planetColors.size() && i < 10; i++) {
        SDL_SetRenderDrawColor(sdlRenderer, planetColors[i].r, planetColors[i].g, planetColors[i].b, 255);
        SDL_FRect dot = {20.0f + i * 23, 80, 8, 8};
        SDL_RenderFillRect(sdlRenderer, &dot);
    }
}

void Renderer3D::drawText(const std::string& text, int x, int y) {
    // Fonction placeholder pour le texte
    // Dans un vrai projet, utiliser SDL_ttf
    (void)text;
    (void)x;
    (void)y;
}