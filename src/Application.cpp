#include <iostream>
#include <SDL3/SDL.h>
#include "Application.h"
#include "constants.h"
#include "vector3D.h"
#include "matrix4x4.h"
#include <iomanip>

Application::Application()
    : window(nullptr)
    , renderer(nullptr)
    , renderer3D(nullptr)
    , camera(nullptr)
    , solarSystem(nullptr)
    , isRunning(false)
    , isPaused(false)
    , simulationSpeed(Constants::DEFAULT_SIMULATION_SPEED)
    , lastFrameTime(0)
    , frameCount(0)
    , fpsTimer(0.0f)
    , currentFPS(0)
    , isDragging(false)
    , lastMouseX(0)
    , lastMouseY(0)
    , selectedBody(nullptr)
    , screenWidth(Constants::WINDOW_WIDTH)
    , screenHeight(Constants::WINDOW_HEIGHT) {
}

Application::~Application() {
    cleanup();
}

bool Application::initializeSDL() {
    std::cout << "\n========================================" << std::endl;
    std::cout << "  SOLAR SYSTEM SIMULATOR 3D" << std::endl;
    std::cout << "  Version 2.0 - Enhanced Edition" << std::endl;
    std::cout << "  8 Planetes + Lune + Effets Visuels" << std::endl;
    std::cout << "========================================\n" << std::endl;
    
    std::cout << "=== Initialisation SDL3 ===" << std::endl;
    
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        std::cerr << "[ERREUR] SDL_Init: " << SDL_GetError() << std::endl;
        return false;
    }
    std::cout << "[OK] SDL3 initialise" << std::endl;
    
    // Créer la fenêtre
    window = SDL_CreateWindow(
        Constants::WINDOW_TITLE,
        screenWidth,
        screenHeight,
        SDL_WINDOW_RESIZABLE
    );
    
    if (!window) {
        std::cerr << "[ERREUR] Creation fenetre: " << SDL_GetError() << std::endl;
        return false;
    }
    std::cout << "[OK] Fenetre creee: " << screenWidth << "x" << screenHeight << std::endl;
    
    // Créer le renderer
    renderer = SDL_CreateRenderer(window, nullptr);
    if (!renderer) {
        std::cerr << "[ERREUR] Creation renderer: " << SDL_GetError() << std::endl;
        return false;
    }
    std::cout << "[OK] Renderer SDL cree" << std::endl;
    
    return true;
}

bool Application::initializeComponents() {
    std::cout << "\n=== Initialisation des composants ===" << std::endl;
    
    // Créer le renderer 3D
    renderer3D = new Renderer3D(renderer, screenWidth, screenHeight);
    std::cout << "[OK] Renderer 3D initialise" << std::endl;
    
    // Configurer la lumière (direction vers le Soleil)
    renderer3D->getRasterizer()->setLightDirection(Vector3D(0.0f, 0.0f, 1.0f));
    
    // Créer la caméra
    float aspect = (float)screenWidth / screenHeight;
    camera = new Camera(aspect, Constants::CAMERA_FOV);
    std::cout << "[OK] Camera configuree" << std::endl;
    
    // Créer le système solaire
    solarSystem = new SolarSystem();
    solarSystem->initialize();
    std::cout << "[OK] Systeme solaire cree:" << std::endl;
    std::cout << "    - " << solarSystem->getBodies().size() << " corps celestes" << std::endl;
    std::cout << "    - " << solarSystem->getTotalTriangleCount() << " triangles totaux" << std::endl;
    
    selectedBody = solarSystem->getSun();
    
    return true;
}

void Application::printControls() {
    std::cout << "\n========================================" << std::endl;
    std::cout << "           CONTROLES" << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << "\nSOURIS :" << std::endl;
    std::cout << "  Clic gauche + Glisser : Rotation camera" << std::endl;
    std::cout << "  Molette haut/bas      : Zoom avant/arriere" << std::endl;
    
    std::cout << "\nCLAVIER :" << std::endl;
    std::cout << "  ESPACE    : Pause / Play" << std::endl;
    std::cout << "  W         : Toggle Wireframe" << std::endl;
    std::cout << "  O         : Toggle Orbites" << std::endl;
    std::cout << "  S         : Toggle Etoiles" << std::endl;
    std::cout << "  G         : Toggle Glow Soleil" << std::endl;
    std::cout << "  I         : Toggle Info HUD" << std::endl;
    std::cout << "  + / =     : Accelerer simulation" << std::endl;
    std::cout << "  -         : Ralentir simulation" << std::endl;
    std::cout << "  R         : Reset camera" << std::endl;
    std::cout << "\nSELECTION PLANETES :" << std::endl;
    std::cout << "  0         : Soleil" << std::endl;
    std::cout << "  1         : Mercure" << std::endl;
    std::cout << "  2         : Venus" << std::endl;
    std::cout << "  3         : Terre" << std::endl;
    std::cout << "  4         : Lune" << std::endl;
    std::cout << "  5         : Mars" << std::endl;
    std::cout << "  6         : Jupiter" << std::endl;
    std::cout << "  7         : Saturne" << std::endl;
    std::cout << "  8         : Uranus" << std::endl;
    std::cout << "  9         : Neptune" << std::endl;
    std::cout << "  ESC       : Quitter" << std::endl;
    std::cout << "\n========================================\n" << std::endl;
}

bool Application::initialize() {
    if (!initializeSDL()) {
        return false;
    }
    
    if (!initializeComponents()) {
        return false;
    }
    
    printControls();
    
    std::cout << "========================================" << std::endl;
    std::cout << "    INITIALISATION COMPLETE !" << std::endl;
    std::cout << "========================================\n" << std::endl;
    
    isRunning = true;
    lastFrameTime = SDL_GetTicks();
    
    return true;
}

void Application::handleEvents() {
    SDL_Event event;
    
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_EVENT_QUIT:
                isRunning = false;
                break;
            
            case SDL_EVENT_KEY_DOWN:
                switch (event.key.key) {
                    case SDLK_ESCAPE:
                        isRunning = false;
                        break;
                    
                    case SDLK_SPACE:
                        isPaused = !isPaused;
                        std::cout << (isPaused ? "[PAUSE]" : "[PLAY]") << std::endl;
                        break;
                    
                    case SDLK_W:
                        renderer3D->getRasterizer()->setWireframeMode(
                            !renderer3D->getRasterizer()->isWireframeMode()
                        );
                        std::cout << (renderer3D->getRasterizer()->isWireframeMode() ? 
                                    "[WIREFRAME ON]" : "[WIREFRAME OFF]") << std::endl;
                        break;
                    
                    case SDLK_O:
                        renderer3D->toggleOrbits();
                        std::cout << (renderer3D->areOrbitsVisible() ? 
                                    "[ORBITES ON]" : "[ORBITES OFF]") << std::endl;
                        break;
                    
                    case SDLK_S:
                        renderer3D->toggleStars();
                        std::cout << (renderer3D->areStarsVisible() ? 
                                    "[ETOILES ON]" : "[ETOILES OFF]") << std::endl;
                        break;
                    
                    case SDLK_G:
                        renderer3D->toggleGlow();
                        std::cout << (renderer3D->isGlowVisible() ? 
                                    "[GLOW ON]" : "[GLOW OFF]") << std::endl;
                        break;
                    
                    case SDLK_I:
                        renderer3D->toggleInfo();
                        std::cout << (renderer3D->isInfoVisible() ? 
                                    "[INFO ON]" : "[INFO OFF]") << std::endl;
                        break;
                    
                    case SDLK_EQUALS:
                    case SDLK_PLUS:
                        simulationSpeed += 0.2f;
                        if (simulationSpeed > Constants::MAX_SIMULATION_SPEED) {
                            simulationSpeed = Constants::MAX_SIMULATION_SPEED;
                        }
                        std::cout << "[VITESSE] " << std::fixed << std::setprecision(1) 
                                  << simulationSpeed << "x" << std::endl;
                        break;
                    
                    case SDLK_MINUS:
                        simulationSpeed -= 0.2f;
                        if (simulationSpeed < Constants::MIN_SIMULATION_SPEED) {
                            simulationSpeed = Constants::MIN_SIMULATION_SPEED;
                        }
                        std::cout << "[VITESSE] " << std::fixed << std::setprecision(1) 
                                  << simulationSpeed << "x" << std::endl;
                        break;
                    
                    case SDLK_R:
                        camera->setDistance(Constants::CAMERA_DISTANCE);
                        std::cout << "[CAMERA RESET]" << std::endl;
                        break;
                    
                    // ========================================
                    // SÉLECTION DES CORPS CÉLESTES (0-9)
                    // ========================================
                    case SDLK_0:
                        if (solarSystem->getBodies().size() > 0) {
                            selectedBody = solarSystem->getBodies()[0];  // Soleil
                            std::cout << "[SELECTION] " << selectedBody->getName() << std::endl;
                        }
                        break;
                    
                    case SDLK_1:
                        if (solarSystem->getBodies().size() > 1) {
                            selectedBody = solarSystem->getBodies()[1];  // Mercure
                            std::cout << "[SELECTION] " << selectedBody->getName() << std::endl;
                        }
                        break;
                    
                    case SDLK_2:
                        if (solarSystem->getBodies().size() > 2) {
                            selectedBody = solarSystem->getBodies()[2];  // Vénus
                            std::cout << "[SELECTION] " << selectedBody->getName() << std::endl;
                        }
                        break;
                    
                    case SDLK_3:
                        if (solarSystem->getBodies().size() > 3) {
                            selectedBody = solarSystem->getBodies()[3];  // Terre
                            std::cout << "[SELECTION] " << selectedBody->getName() << std::endl;
                        }
                        break;
                    
                    case SDLK_4:
                        if (solarSystem->getBodies().size() > 4) {
                            selectedBody = solarSystem->getBodies()[4];  // Lune
                            std::cout << "[SELECTION] " << selectedBody->getName() << std::endl;
                        }
                        break;
                    
                    case SDLK_5:
                        if (solarSystem->getBodies().size() > 5) {
                            selectedBody = solarSystem->getBodies()[5];  // Mars
                            std::cout << "[SELECTION] " << selectedBody->getName() << std::endl;
                        }
                        break;
                    
                    case SDLK_6:
                        if (solarSystem->getBodies().size() > 6) {
                            selectedBody = solarSystem->getBodies()[6];  // Jupiter
                            std::cout << "[SELECTION] " << selectedBody->getName() << std::endl;
                        }
                        break;
                    
                    case SDLK_7:
                        if (solarSystem->getBodies().size() > 7) {
                            selectedBody = solarSystem->getBodies()[7];  // Saturne
                            std::cout << "[SELECTION] " << selectedBody->getName() << std::endl;
                        }
                        break;
                    
                    case SDLK_8:
                        if (solarSystem->getBodies().size() > 8) {
                            selectedBody = solarSystem->getBodies()[8];  // Uranus
                            std::cout << "[SELECTION] " << selectedBody->getName() << std::endl;
                        }
                        break;
                    
                    case SDLK_9:
                        if (solarSystem->getBodies().size() > 9) {
                            selectedBody = solarSystem->getBodies()[9];  // Neptune
                            std::cout << "[SELECTION] " << selectedBody->getName() << std::endl;
                        }
                        break;
                }
                break;
            
            case SDL_EVENT_MOUSE_BUTTON_DOWN:
                if (event.button.button == SDL_BUTTON_LEFT) {
                    isDragging = true;
                    lastMouseX = (int)event.button.x;
                    lastMouseY = (int)event.button.y;
                }
                break;
            
            case SDL_EVENT_MOUSE_BUTTON_UP:
                if (event.button.button == SDL_BUTTON_LEFT) {
                    isDragging = false;
                }
                break;
            
            case SDL_EVENT_MOUSE_MOTION:
                if (isDragging) {
                    int deltaX = (int)event.motion.x - lastMouseX;
                    int deltaY = (int)event.motion.y - lastMouseY;
                    
                    camera->rotate(
                        deltaX * Constants::CAMERA_ROTATION_SPEED,
                        -deltaY * Constants::CAMERA_ROTATION_SPEED
                    );
                    
                    lastMouseX = (int)event.motion.x;
                    lastMouseY = (int)event.motion.y;
                }
                break;
            
            case SDL_EVENT_MOUSE_WHEEL:
                camera->zoom(-event.wheel.y * Constants::CAMERA_ZOOM_SPEED);
                break;
            
            case SDL_EVENT_WINDOW_RESIZED:
                screenWidth = event.window.data1;
                screenHeight = event.window.data2;
                renderer3D->resize(screenWidth, screenHeight);
                camera->setAspectRatio((float)screenWidth / screenHeight);
                std::cout << "[RESIZE] " << screenWidth << "x" << screenHeight << std::endl;
                break;
        }
    }
}

void Application::update(float deltaTime) {
    if (!isPaused) {
        solarSystem->update(deltaTime * simulationSpeed);
    }
    
    camera->update();
}

void Application::render() {
    renderer3D->clear();
    renderer3D->render(solarSystem, camera);
    renderer3D->renderInfo(currentFPS, 
                          renderer3D->getRasterizer()->getLastTriangleCount(),
                          simulationSpeed,
                          selectedBody);
    renderer3D->present();
}

void Application::run() {
    std::cout << "Demarrage de la simulation..." << std::endl;
    std::cout << "Appuyez sur ESPACE pour pause/play\n" << std::endl;
    
    while (isRunning) {
        // Timing
        Uint64 currentTime = SDL_GetTicks();
        float deltaTime = (currentTime - lastFrameTime) / 1000.0f;
        lastFrameTime = currentTime;
        
        // Événements
        handleEvents();
        
        // Mise à jour
        update(deltaTime);
        
        // Rendu
        render();
        
        // FPS
        frameCount++;
        fpsTimer += deltaTime;
        
        if (fpsTimer >= 1.0f) {
            currentFPS = frameCount;
            
            std::cout << "FPS: " << std::setw(3) << currentFPS 
                      << " | Triangles: " << std::setw(4) << renderer3D->getRasterizer()->getLastTriangleCount()
                      << " | Vitesse: " << std::fixed << std::setprecision(1) << simulationSpeed << "x"
                      << " | Zoom: " << std::setw(3) << (int)camera->getDistance()
                      << std::endl;
            
            frameCount = 0;
            fpsTimer = 0.0f;
        }
        
        // Limiter FPS
        SDL_Delay(Constants::FRAME_DELAY_MS);
    }
}

void Application::cleanup() {
    std::cout << "\n=== Nettoyage ===" << std::endl;
    
    if (solarSystem) {
        delete solarSystem;
        solarSystem = nullptr;
        std::cout << "[OK] Systeme solaire detruit" << std::endl;
    }
    
    if (camera) {
        delete camera;
        camera = nullptr;
        std::cout << "[OK] Camera detruite" << std::endl;
    }
    
    if (renderer3D) {
        delete renderer3D;
        renderer3D = nullptr;
        std::cout << "[OK] Renderer 3D detruit" << std::endl;
    }
    
    if (renderer) {
        SDL_DestroyRenderer(renderer);
        renderer = nullptr;
        std::cout << "[OK] Renderer SDL detruit" << std::endl;
    }
    
    if (window) {
        SDL_DestroyWindow(window);
        window = nullptr;
        std::cout << "[OK] Fenetre detruite" << std::endl;
    }
    
    SDL_Quit();
    std::cout << "[OK] SDL3 ferme" << std::endl;
    
    std::cout << "\n========================================" << std::endl;
    std::cout << "  Merci d'avoir utilise le simulateur !" << std::endl;
    std::cout << "========================================\n" << std::endl;
}