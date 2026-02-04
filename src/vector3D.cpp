#include "Vector3D.h"
#include "constants.h"
#include <cmath>
#include <iostream>

// ======================
// CONSTRUCTEURS
// ======================

Vector3D::Vector3D() : x(0.0f), y(0.0f), z(0.0f) {}

Vector3D::Vector3D(float x, float y, float z) : x(x), y(y), z(z) {}

// ======================
// OPÉRATEURS ARITHMÉTIQUES
// ======================

Vector3D Vector3D::operator+(const Vector3D& other) const {
    return Vector3D(x + other.x, y + other.y, z + other.z);
}

Vector3D Vector3D::operator-(const Vector3D& other) const {
    return Vector3D(x - other.x, y - other.y, z - other.z);
}

Vector3D Vector3D::operator*(float scalar) const {
    return Vector3D(x * scalar, y * scalar, z * scalar);
}

Vector3D Vector3D::operator/(float scalar) const {
    // Protection contre division par zéro
    if (std::abs(scalar) < Constants::EPSILON) {
        std::cerr << "Warning: Division by zero in Vector3D" << std::endl;
        return Vector3D(0.0f, 0.0f, 0.0f);
    }
    return Vector3D(x / scalar, y / scalar, z / scalar);
}

Vector3D Vector3D::operator-() const {
    return Vector3D(-x, -y, -z);
}

// ======================
// OPÉRATEURS D'AFFECTATION
// ======================

Vector3D& Vector3D::operator+=(const Vector3D& other) {
    x += other.x;
    y += other.y;
    z += other.z;
    return *this;
}

Vector3D& Vector3D::operator-=(const Vector3D& other) {
    x -= other.x;
    y -= other.y;
    z -= other.z;
    return *this;
}

Vector3D& Vector3D::operator*=(float scalar) {
    x *= scalar;
    y *= scalar;
    z *= scalar;
    return *this;
}

Vector3D& Vector3D::operator/=(float scalar) {
    if (std::abs(scalar) < Constants::EPSILON) {
        std::cerr << "Warning: Division by zero in Vector3D" << std::endl;
        return *this;
    }
    x /= scalar;
    y /= scalar;
    z /= scalar;
    return *this;
}

// ======================
// OPÉRATEURS DE COMPARAISON
// ======================

bool Vector3D::operator==(const Vector3D& other) const {
    return std::abs(x - other.x) < Constants::EPSILON &&
           std::abs(y - other.y) < Constants::EPSILON &&
           std::abs(z - other.z) < Constants::EPSILON;
}

bool Vector3D::operator!=(const Vector3D& other) const {
    return !(*this == other);
}

// ======================
// PRODUITS VECTORIELS
// ======================

float Vector3D::dot(const Vector3D& other) const {
    return x * other.x + y * other.y + z * other.z;
}

Vector3D Vector3D::cross(const Vector3D& other) const {
    return Vector3D(
        y * other.z - z * other.y,
        z * other.x - x * other.z,
        x * other.y - y * other.x
    );
}

// ======================
// LONGUEUR ET NORMALISATION
// ======================

float Vector3D::length() const {
    return std::sqrt(x * x + y * y + z * z);
}

float Vector3D::lengthSquared() const {
    return x * x + y * y + z * z;
}

Vector3D Vector3D::normalized() const {
    float len = length();
    if (len < Constants::EPSILON) {
        std::cerr << "Warning: Normalizing zero-length vector" << std::endl;
        return Vector3D(0.0f, 0.0f, 0.0f);
    }
    return *this / len;
}

void Vector3D::normalize() {
    float len = length();
    if (len < Constants::EPSILON) {
        std::cerr << "Warning: Normalizing zero-length vector" << std::endl;
        x = y = z = 0.0f;
        return;
    }
    x /= len;
    y /= len;
    z /= len;
}

// ======================
// DISTANCE
// ======================

float Vector3D::distance(const Vector3D& other) const {
    return (*this - other).length();
}

float Vector3D::distanceSquared(const Vector3D& other) const {
    return (*this - other).lengthSquared();
}

// ======================
// INTERPOLATION
// ======================

Vector3D Vector3D::lerp(const Vector3D& a, const Vector3D& b, float t) {
    // Clamp t entre 0 et 1
    t = std::max(0.0f, std::min(1.0f, t));
    return a + (b - a) * t;
}

// ======================
// UTILITAIRES
// ======================

void Vector3D::set(float newX, float newY, float newZ) {
    x = newX;
    y = newY;
    z = newZ;
}

void Vector3D::setZero() {
    x = y = z = 0.0f;
}

void Vector3D::print() const {
    std::cout << "(" << x << ", " << y << ", " << z << ")" << std::endl;
}

// ======================
// VECTEURS CONSTANTS
// ======================

Vector3D Vector3D::zero() { return Vector3D(0.0f, 0.0f, 0.0f); }
Vector3D Vector3D::one() { return Vector3D(1.0f, 1.0f, 1.0f); }
Vector3D Vector3D::up() { return Vector3D(0.0f, 1.0f, 0.0f); }
Vector3D Vector3D::down() { return Vector3D(0.0f, -1.0f, 0.0f); }
Vector3D Vector3D::left() { return Vector3D(-1.0f, 0.0f, 0.0f); }
Vector3D Vector3D::right() { return Vector3D(1.0f, 0.0f, 0.0f); }
Vector3D Vector3D::forward() { return Vector3D(0.0f, 0.0f, 1.0f); }
Vector3D Vector3D::back() { return Vector3D(0.0f, 0.0f, -1.0f); }

// ======================
// OPÉRATEURS EXTERNES
// ======================

Vector3D operator*(float scalar, const Vector3D& vec) {
    return vec * scalar;
}

std::ostream& operator<<(std::ostream& os, const Vector3D& vec) {
    os << "(" << vec.x << ", " << vec.y << ", " << vec.z << ")";
    return os;
}