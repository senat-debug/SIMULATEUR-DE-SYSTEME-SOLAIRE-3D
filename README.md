# SIMULATEUR-DE-SYSTEME-SOLAIRE-3D
ce projet est un simulateur de système 3D réaliser avec la sdl3 sans l'utilisation d'un moteur de jeu comme opengl ou vulkan
<p align="center">
  <img src="https://img.shields.io/badge/Version-2.0-blue?style=for-the-badge" alt="Version"/>
  <img src="https://img.shields.io/badge/Build-Passing-brightgreen?style=for-the-badge" alt="Build"/>
  <img src="https://img.shields.io/badge/SDL-3.0-orange?style=for-the-badge&logo=data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAA4AAAAOCAMAAAAolt3jAAAAIGNIUk0AAHomAACAhAAA+gAAAIDoAAB1MAAA6mAAADqYAAAXcJy6UTwAAABjUExURf///wBLnQBLnQBLnQBLnQBLnQBLnQBLnQBLnQBLnQBLnQBLnQBLnQAAAABLnQBLnQBLnQBLnf///wBLnQBLnQBLnQBLnQBLnQBLnQBLnQBLnQBLnQBLnQBLnQBLnQBLnf///4EGcJYAAAAgdFJOU///////////////////////////////////////AN/xKfkAAAABYktHRACIBR1IAAAACXBIWXMAAA7DAAAOwwHHb6hkAAAAPUlEQVQI12NgYGRiZmFlY+fg5OJmYODh5eMXEBQSFhEVY2CQkJSSlpGVk1dQVGJgUFZRVVPX0NTS1tFlAAAzgQMBW01O7QAAAABJRU5ErkJggg==" alt="SDL3"/>
  <img src="https://img.shields.io/badge/C++-17-00599C?style=for-the-badge&logo=cplusplus&logoColor=white" alt="C++17"/>
  <img src="https://img.shields.io/badge/License-MIT-yellow?style=for-the-badge" alt="License"/>
</p>

<h1 align="center">🌌 Simulateur de Système Solaire 3D</h1>

<p align="center">
  <strong>Un moteur de rendu 3D temps réel développé from scratch</strong><br/>
  <em>Sans OpenGL. Sans DirectX. Juste des mathématiques pures et SDL3.</em>
</p>

<p align="center">
  <a href="#-fonctionnalités">Fonctionnalités</a> •
  <a href="#-stack-technique">Stack</a> •
  <a href="#-installation">Installation</a> •
  <a href="#-utilisation">Utilisation</a> •
  <a href="#-architecture">Architecture</a> •
  <a href="#-contribution">Contribution</a>
</p>

---

## 📖 À Propos

> *Explorez le système solaire comme jamais auparavant, rendu pixel par pixel par un moteur 3D entièrement codé à la main.*

Ce projet est un **simulateur interactif du système solaire en 3D temps réel**, développé dans un cadre académique pour démontrer une maîtrise complète du **pipeline graphique 3D**. Aucune API graphique de haut niveau n'est utilisée — chaque pixel est calculé et affiché via un **Software Renderer** utilisant exclusivement SDL3.

### � Projet Académique
- **Formation** : Art Numérique ING (Niveau 1)
- **Étudiant** : YOUBISSI NGATCHEUSSI David (25P921)

---

## ✨ Fonctionnalités

<table>
<tr>
<td width="50%">

### 🪐 Simulation Astronomique
- ☀️ **Soleil** avec effet de glow lumineux
- 🌍 **8 planètes** (Mercure → Neptune)
- 🌙 **Lune** en orbite autour de la Terre
- 💫 **Orbites elliptiques** (Lois de Kepler)
- 🔄 **Inclinaisons axiales** réalistes
- 🪐 **Anneaux de Saturne** animés

</td>
<td width="50%">

### 🎨 Rendu Graphique
- 🖥️ **Moteur 3D Software** from scratch
- 💡 **Éclairage Phong** (Ambient + Diffuse + Spéculaire)
- 📐 **Z-Buffer** pour la gestion de profondeur
- ✂️ **Backface Culling** optimisé
- ⭐ **Étoiles colorées** (blanches, bleutées, rougeâtres)
- 🔲 **Mode Wireframe** pour debug

</td>
</tr>
</table>

---

## �️ Stack Technique

| Catégorie | Technologie | Description |
|:---------:|:-----------:|:------------|
| **Langage** | ![C++](https://img.shields.io/badge/C++-17-00599C?logo=cplusplus&logoColor=white) | Standard moderne avec features avancées |
| **Graphiques** | ![SDL3](https://img.shields.io/badge/SDL-3.0-orange) | Simple DirectMedia Layer (pixel-level) |
| **Mathématiques** | Custom | Vecteurs 3D, Matrices 4x4, Projections |
| **Build** | ![GCC](https://img.shields.io/badge/G++-13-A42E2B) | GNU Compiler Collection |
| **IDE** | ![VSCode](https://img.shields.io/badge/VS_Code-007ACC?logo=visualstudiocode&logoColor=white) | Configuration incluse |

### 📚 Aucune Dépendance Externe
Le projet implémente **manuellement** :
- Opérations vectorielles (addition, dot product, cross product, normalisation)
- Matrices de transformation (translation, rotation, scale)
- Projection perspective et orthographique
- Algorithme de rasterisation (scanline)
- Z-Buffer (depth testing)
- Algorithme de Bresenham (lignes)

---

## 📦 Installation

### Prérequis

```bash
# Vérifier que g++ est installé
g++ --version  # Requis: 11.0+

# Vérifier SDL3
# Télécharger depuis: https://github.com/libsdl-org/SDL/releases
```

### Étapes d'installation

```bash
# 1. Cloner le repository
git clone https://github.com/votre-username/simulateur-systeme-solaire-3d.git
cd simulateur-systeme-solaire-3d

# 2. Compiler le projet
g++ src/main.cpp src/Application.cpp src/renderer3D.cpp src/solarsystem.cpp \
    src/celestialbody.cpp src/camera.cpp src/rasterizer.cpp src/Matrix4x4.cpp \
    src/vector3D.cpp src/mesh.cpp src/Sphere.cpp \
    -I include -lSDL3 -o Sim.exe

# 3. Lancer la simulation
./Sim.exe
```

### 🪟 Windows (PowerShell)
```powershell
# Compilation one-liner
g++ src/*.cpp -I include -lSDL3 -o Sim.exe; .\Sim.exe
```

---

## 🎮 Utilisation

### Contrôles Souris

| Action | Commande |
|:-------|:---------|
| **Rotation caméra** | `Clic gauche` + Glisser |
| **Zoom** | `Molette` haut/bas |

### Contrôles Clavier

#### ⏯️ Simulation

| Touche | Action |
|:------:|:-------|
| `ESPACE` | ⏸️ Pause / ▶️ Play |
| `+` / `=` | ⏩ Accélérer le temps |
| `-` | ⏪ Ralentir le temps |

#### 👁️ Affichage

| Touche | Action |
|:------:|:-------|
| `W` | Toggle **Wireframe** |
| `O` | Toggle **Orbites** |
| `S` | Toggle **Étoiles** |
| `G` | Toggle **Glow** du Soleil |
| `I` | Toggle **Infos** HUD |
| `R` | **Reset** caméra |

#### 🎯 Sélection des Corps Célestes

| Touche | Corps |
|:------:|:------|
| `0` | ☀️ Soleil |
| `1` | ☿ Mercure |
| `2` | ♀ Vénus |
| `3` | 🌍 Terre |
| `4` | 🌙 Lune |
| `5` | ♂ Mars |
| `6` | ♃ Jupiter |
| `7` | 🪐 Saturne |
| `8` | ⛢ Uranus |
| `9` | ♆ Neptune |
| `ESC` | ❌ Quitter |

---

## 📁 Architecture

```
SIMULATEUR SYSTEME SOLAIRE 3D/
│
├── 📂 include/                    # Headers C++
│   ├── Application.h              # Classe principale de l'application
│   ├── Camera.h                   # Caméra orbitale 3D
│   ├── CelestialBody.h            # Corps célestes (planètes, soleil, lune)
│   ├── Constants.h                # Toutes les constantes du projet
│   ├── Matrix4x4.h                # Opérations matricielles
│   ├── Mesh.h                     # Géométrie 3D de base
│   ├── Rasterizer.h               # Rendu pixel par pixel
│   ├── Renderer3D.h               # Orchestrateur du rendu
│   ├── SolarSystem.h              # Gestion du système solaire
│   ├── Sphere.h                   # Génération de sphères UV
│   └── Vector3D.h                 # Opérations vectorielles
│
├── 📂 src/                        # Implémentations C++
│   ├── main.cpp                   # Point d'entrée
│   ├── Application.cpp            # Boucle principale & événements
│   ├── camera.cpp                 # Matrices view/projection
│   ├── celestialbody.cpp          # Orbites Kepler & rotations
│   ├── Matrix4x4.cpp              # Transformations 3D
│   ├── mesh.cpp                   # Base de géométrie
│   ├── rasterizer.cpp             # Pipeline de rendu (Phong, glow)
│   ├── renderer3D.cpp             # Rendu étoiles, planètes, effets
│   ├── solarsystem.cpp            # Initialisation des 10 corps
│   ├── Sphere.cpp                 # Tessellation UV
│   └── vector3D.cpp               # Math vectorielle
│
├── 📂 assets/                     # Ressources (optionnel)
│   ├── textures/                  # Textures futures
│   └── audio/                     # Sons futurs
│
├── 📄 README.md                   # Ce fichier
└── 🎮 Sim.exe                     # Exécutable compilé
```

### 🔄 Pipeline de Rendu

```
┌─────────────┐    ┌─────────────┐    ┌─────────────┐    ┌─────────────┐
│   Vertex    │───▶│   Model     │───▶│    View     │───▶│ Projection  │
│   Buffer    │    │   Matrix    │    │   Matrix    │    │   Matrix    │
└─────────────┘    └─────────────┘    └─────────────┘    └─────────────┘
                                                                │
                                                                ▼
┌─────────────┐    ┌─────────────┐    ┌─────────────┐    ┌─────────────┐
│   Display   │◀───│   Phong     │◀───│   Z-Test    │◀───│  Rasterize  │
│             │    │  Lighting   │    │   Buffer    │    │  (Scanline) │
└─────────────┘    └─────────────┘    └─────────────┘    └─────────────┘
```

---

## 🤝 Contribution

Les contributions sont les bienvenues ! Voici comment participer :

```bash
# 1. Forker le projet

# 2. Créer une branche feature
git checkout -b feature/ma-super-feature

# 3. Commiter vos changements
git commit -m "✨ Add: ma super feature"

# 4. Pousser la branche
git push origin feature/ma-super-feature

# 5. Ouvrir une Pull Request
```

### 📋 Conventions de Commit

| Emoji | Type | Description |
|:-----:|:----:|:------------|
| ✨ | `Add` | Nouvelle fonctionnalité |
| 🐛 | `Fix` | Correction de bug |
| 🎨 | `Style` | Amélioration UI/UX |
| ♻️ | `Refactor` | Refactoring de code |
| 📚 | `Docs` | Documentation |
| ⚡ | `Perf` | Optimisation performance |

---

## 📜 Licence

Ce projet est sous licence **MIT** — voir le fichier [LICENSE](LICENSE) pour plus de détails.

```
MIT License

Copyright (c) 2026 YOUBISSI NGATCHEUSSI David

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction...
```

---

<p align="center">
  <strong>Fait avec ❤️ et beaucoup de mathématiques</strong><br/>
  <sub>⭐ Star ce projet si vous l'appréciez !</sub>
</p>

<p align="center">
  <img src="https://img.shields.io/badge/Made_with-C++-00599C?style=flat-square&logo=cplusplus" alt="Made with C++"/>
  <img src="https://img.shields.io/badge/Powered_by-SDL3-orange?style=flat-square" alt="Powered by SDL3"/>
  <img src="https://img.shields.io/badge/No_OpenGL-Pure_Math-blueviolet?style=flat-square" alt="No OpenGL"/>
</p>
