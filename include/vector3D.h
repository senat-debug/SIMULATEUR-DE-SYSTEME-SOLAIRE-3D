#ifndef VECTOR3D_H
#define VECTOR3D_H

#include <cmath>
#include <iostream>

/**
 * @class Vector3D
 * @brief Classe représentant un vecteur 3D (x, y, z)
 * 
 * Utilisée pour :
 * - Positions des objets dans l'espace
 * - Directions et déplacements
 * - Normales pour l'éclairage
 * - Calculs géométriques
 */
class Vector3D {
public:
    float x, y, z;
    
    // ======================
    // CONSTRUCTEURS
    // ======================
    Vector3D();
    Vector3D(float x, float y, float z);
    
    // ======================
    // OPÉRATEURS ARITHMÉTIQUES
    // ======================
    Vector3D operator+(const Vector3D& other) const;
    Vector3D operator-(const Vector3D& other) const;
    Vector3D operator*(float scalar) const;
    Vector3D operator/(float scalar) const;
    Vector3D operator-() const;  // Négation
    
    // ======================
    // OPÉRATEURS D'AFFECTATION
    // ======================
    Vector3D& operator+=(const Vector3D& other);
    Vector3D& operator-=(const Vector3D& other);
    Vector3D& operator*=(float scalar);
    Vector3D& operator/=(float scalar);
    
    // ======================
    // OPÉRATEURS DE COMPARAISON
    // ======================
    bool operator==(const Vector3D& other) const;
    bool operator!=(const Vector3D& other) const;
    
    // ======================
    // PRODUITS VECTORIELS
    // ======================
    
    /**
     * @brief Produit scalaire (dot product)
     * @return Scalaire résultant du produit
     * 
     * Utilisé pour :
     * - Calculer l'angle entre deux vecteurs
     * - Projections
     * - Éclairage (cos de l'angle)
     */
    float dot(const Vector3D& other) const;
    
    /**
     * @brief Produit vectoriel (cross product)
     * @return Vecteur perpendiculaire aux deux vecteurs
     * 
     * Utilisé pour :
     * - Calculer les normales des triangles
     * - Trouver un vecteur perpendiculaire
     */
    Vector3D cross(const Vector3D& other) const;
    
    // ======================
    // LONGUEUR ET NORMALISATION
    // ======================
    
    /**
     * @brief Calcule la longueur (magnitude) du vecteur
     * @return Longueur (toujours positive)
     */
    float length() const;
    
    /**
     * @brief Calcule le carré de la longueur (plus rapide, pas de sqrt)
     * @return Longueur au carré
     */
    float lengthSquared() const;
    
    /**
     * @brief Retourne une copie normalisée (longueur = 1)
     * @return Vecteur normalisé
     */
    Vector3D normalized() const;
    
    /**
     * @brief Normalise ce vecteur (modifie l'objet)
     */
    void normalize();
    
    // ======================
    // DISTANCE
    // ======================
    
    /**
     * @brief Distance entre deux points
     */
    float distance(const Vector3D& other) const;
    
    /**
     * @brief Distance au carré (plus rapide)
     */
    float distanceSquared(const Vector3D& other) const;
    
    // ======================
    // INTERPOLATION
    // ======================
    
    /**
     * @brief Interpolation linéaire entre deux vecteurs
     * @param a Premier vecteur
     * @param b Deuxième vecteur
     * @param t Facteur d'interpolation [0, 1]
     * @return Vecteur interpolé
     */
    static Vector3D lerp(const Vector3D& a, const Vector3D& b, float t);
    
    // ======================
    // UTILITAIRES
    // ======================
    
    /**
     * @brief Modifie les composantes du vecteur
     */
    void set(float x, float y, float z);
    
    /**
     * @brief Met toutes les composantes à zéro
     */
    void setZero();
    
    /**
     * @brief Affiche le vecteur (pour debug)
     */
    void print() const;
    
    // ======================
    // VECTEURS CONSTANTS UTILES
    // ======================
    static Vector3D zero();      // (0, 0, 0)
    static Vector3D one();       // (1, 1, 1)
    static Vector3D up();        // (0, 1, 0)
    static Vector3D down();      // (0, -1, 0)
    static Vector3D left();      // (-1, 0, 0)
    static Vector3D right();     // (1, 0, 0)
    static Vector3D forward();   // (0, 0, 1)
    static Vector3D back();      // (0, 0, -1)
};

// ======================
// OPÉRATEURS EXTERNES
// ======================

/**
 * @brief Permet d'écrire: 2.0f * vec
 */
Vector3D operator*(float scalar, const Vector3D& vec);

/**
 * @brief Affichage avec std::cout
 */
std::ostream& operator<<(std::ostream& os, const Vector3D& vec);

#endif // VECTOR3D_H