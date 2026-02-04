#ifndef APPLICATION_H
#define APPLICATION_H

#include <SDL3/SDL.h>
#include "Renderer3D.h"
#include "Camera.h"
#include "SolarSystem.h"


class Application {
private:
    // SDL
    SDL_Window* window;
    SDL_Renderer* renderer;
    
    // Composants principaux
    Renderer3D* renderer3D;
    Camera* camera;
    SolarSystem* solarSystem;
    
    // État
    bool isRunning;
    bool isPaused;
    float simulationSpeed;
    
    // Timing
    Uint64 lastFrameTime;
    int frameCount;
    float fpsTimer;
    int currentFPS;
    
    // Contrôles souris
    bool isDragging;
    int lastMouseX;
    int lastMouseY;
    
    // Sélection
    CelestialBody* selectedBody;
    
    // Dimensions
    int screenWidth;
    int screenHeight;
    
    bool initializeSDL();
    
  
    bool initializeComponents();
    
    
    void handleEvents();
    
    
    void update(float deltaTime);
    
    
    void render();
    
    
    void cleanup();
    
    
    void printControls();
    
public:
    Application();
    ~Application();
    
    
    bool initialize();
    
    
    void run();
};

#endif 