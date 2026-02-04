#include "Application.h"
#include <SDL3/SDL.h>
#include "constants.h"
#include "CelestialBody.h"
#include <iostream>

/**
 * @file main.cpp
 * @brief Point d'entrée du simulateur de système solaire 3D
 * @author [Votre Nom]
 * @date 2025
 * 
 * Projet réalisé avec :
 * - SDL3 uniquement (pas d'OpenGL)
 * - Software rasterizer 3D from scratch
 * - Pipeline graphique 3D complet implémenté manuellement
 */

int main(int argc, char* argv[]) {
    // Créer l'application
    Application app;
    
    // Initialiser
    if (!app.initialize()) {
        std::cerr << "\n[ERREUR FATALE] Impossible d'initialiser l'application" << std::endl;
        return 1;
    }
    
    // Lancer la boucle principale
    app.run();
    
    return 0;
}