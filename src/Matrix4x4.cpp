#include "Matrix4x4.h"
#include <cmath>
#include <cstring>
#include <iostream>

// ======================
// CONSTRUCTEURS
// ======================

Matrix4x4::Matrix4x4() {
    // Initialiser à l'identité
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            m[i][j] = (i == j) ? 1.0f : 0.0f;
        }
    }
}

Matrix4x4::Matrix4x4(float values[4][4]) {
    std::memcpy(m, values, 16 * sizeof(float));
}

// ======================
// MATRICES SPÉCIALES
// ======================

Matrix4x4 Matrix4x4::identity() {
    Matrix4x4 result;
    // Déjà initialisé à l'identité dans le constructeur
    return result;
}

Matrix4x4 Matrix4x4::translation(float x, float y, float z) {
    Matrix4x4 result;
    result.m[0][3] = x;
    result.m[1][3] = y;
    result.m[2][3] = z;
    return result;
}

Matrix4x4 Matrix4x4::rotationX(float angle) {
    Matrix4x4 result;
    float c = std::cos(angle);
    float s = std::sin(angle);
    
    result.m[1][1] = c;
    result.m[1][2] = -s;
    result.m[2][1] = s;
    result.m[2][2] = c;
    
    return result;
}

Matrix4x4 Matrix4x4::rotationY(float angle) {
    Matrix4x4 result;
    float c = std::cos(angle);
    float s = std::sin(angle);
    
    result.m[0][0] = c;
    result.m[0][2] = s;
    result.m[2][0] = -s;
    result.m[2][2] = c;
    
    return result;
}

Matrix4x4 Matrix4x4::rotationZ(float angle) {
    Matrix4x4 result;
    float c = std::cos(angle);
    float s = std::sin(angle);
    
    result.m[0][0] = c;
    result.m[0][1] = -s;
    result.m[1][0] = s;
    result.m[1][1] = c;
    
    return result;
}

Matrix4x4 Matrix4x4::scale(float sx, float sy, float sz) {
    Matrix4x4 result;
    result.m[0][0] = sx;
    result.m[1][1] = sy;
    result.m[2][2] = sz;
    return result;
}

Matrix4x4 Matrix4x4::lookAt(const Vector3D& eye, 
                            const Vector3D& target, 
                            const Vector3D& up) {
    // Calculer les axes de la caméra
    Vector3D zAxis = (eye - target).normalized();  // Forward
    Vector3D xAxis = up.cross(zAxis).normalized(); // Right
    Vector3D yAxis = zAxis.cross(xAxis);           // Up
    
    Matrix4x4 result;
    
    // Rotation
    result.m[0][0] = xAxis.x;
    result.m[0][1] = xAxis.y;
    result.m[0][2] = xAxis.z;
    
    result.m[1][0] = yAxis.x;
    result.m[1][1] = yAxis.y;
    result.m[1][2] = yAxis.z;
    
    result.m[2][0] = zAxis.x;
    result.m[2][1] = zAxis.y;
    result.m[2][2] = zAxis.z;
    
    // Translation
    result.m[0][3] = -xAxis.dot(eye);
    result.m[1][3] = -yAxis.dot(eye);
    result.m[2][3] = -zAxis.dot(eye);
    
    return result;
}

Matrix4x4 Matrix4x4::perspective(float fov, float aspect, float near, float far) {
    Matrix4x4 result;
    
    // Convertir FOV en radians et calculer la tangente
    float tanHalfFov = std::tan(fov * 0.5f * 3.14159265f / 180.0f);
    
    // Initialiser à zéro
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            result.m[i][j] = 0.0f;
        }
    }
    
    // Matrice de projection perspective
    result.m[0][0] = 1.0f / (aspect * tanHalfFov);
    result.m[1][1] = 1.0f / tanHalfFov;
    result.m[2][2] = -(far + near) / (far - near);
    result.m[2][3] = -(2.0f * far * near) / (far - near);
    result.m[3][2] = -1.0f;
    result.m[3][3] = 0.0f;
    
    return result;
}

// ======================
// OPÉRATEURS
// ======================

Matrix4x4 Matrix4x4::operator*(const Matrix4x4& other) const {
    Matrix4x4 result;
    
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            result.m[i][j] = 0.0f;
            for (int k = 0; k < 4; k++) {
                result.m[i][j] += m[i][k] * other.m[k][j];
            }
        }
    }
    
    return result;
}

Vector3D Matrix4x4::operator*(const Vector3D& vec) const {
    return transformPoint(vec);
}

// ======================
// TRANSFORMATIONS
// ======================

Vector3D Matrix4x4::transformPoint(const Vector3D& point) const {
    // Transformer le point en coordonnées homogènes
    float x = m[0][0] * point.x + m[0][1] * point.y + m[0][2] * point.z + m[0][3];
    float y = m[1][0] * point.x + m[1][1] * point.y + m[1][2] * point.z + m[1][3];
    float z = m[2][0] * point.x + m[2][1] * point.y + m[2][2] * point.z + m[2][3];
    float w = m[3][0] * point.x + m[3][1] * point.y + m[3][2] * point.z + m[3][3];
    
    // Division perspective
    if (std::abs(w) > 0.0001f) {
        return Vector3D(x / w, y / w, z / w);
    }
    
    return Vector3D(x, y, z);
}

Vector3D Matrix4x4::transformVector(const Vector3D& vec) const {
    // Transformer sans la translation (w=0)
    float x = m[0][0] * vec.x + m[0][1] * vec.y + m[0][2] * vec.z;
    float y = m[1][0] * vec.x + m[1][1] * vec.y + m[1][2] * vec.z;
    float z = m[2][0] * vec.x + m[2][1] * vec.y + m[2][2] * vec.z;
    
    return Vector3D(x, y, z);
}

// ======================
// ACCÈS AUX ÉLÉMENTS
// ======================

float& Matrix4x4::at(int row, int col) {
    return m[row][col];
}

float Matrix4x4::at(int row, int col) const {
    return m[row][col];
}

const float* Matrix4x4::data() const {
    return &m[0][0];
}

// ======================
// UTILITAIRES
// ======================

void Matrix4x4::print() const {
    std::cout << "Matrix4x4:" << std::endl;
    for (int i = 0; i < 4; i++) {
        std::cout << "  [";
        for (int j = 0; j < 4; j++) {
            std::cout << m[i][j];
            if (j < 3) std::cout << ", ";
        }
        std::cout << "]" << std::endl;
    }
}

Matrix4x4 Matrix4x4::transposed() const {
    Matrix4x4 result;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            result.m[i][j] = m[j][i];
        }
    }
    return result;
}

Matrix4x4 Matrix4x4::inverse() const {
    // Inverse simplifié pour matrices de transformation
    // Pour une matrice complète, il faudrait l'algorithme de Gauss-Jordan
    Matrix4x4 result;
    
    // Extraire la rotation/scale (3x3)
    float det = m[0][0] * (m[1][1] * m[2][2] - m[2][1] * m[1][2])
              - m[0][1] * (m[1][0] * m[2][2] - m[1][2] * m[2][0])
              + m[0][2] * (m[1][0] * m[2][1] - m[1][1] * m[2][0]);
    
    if (std::abs(det) < 0.0001f) {
        // Matrice non inversible, retourner l'identité
        return identity();
    }
    
    float invDet = 1.0f / det;
    
    // Inverse de la partie 3x3
    result.m[0][0] = (m[1][1] * m[2][2] - m[2][1] * m[1][2]) * invDet;
    result.m[0][1] = (m[0][2] * m[2][1] - m[0][1] * m[2][2]) * invDet;
    result.m[0][2] = (m[0][1] * m[1][2] - m[0][2] * m[1][1]) * invDet;
    
    result.m[1][0] = (m[1][2] * m[2][0] - m[1][0] * m[2][2]) * invDet;
    result.m[1][1] = (m[0][0] * m[2][2] - m[0][2] * m[2][0]) * invDet;
    result.m[1][2] = (m[1][0] * m[0][2] - m[0][0] * m[1][2]) * invDet;
    
    result.m[2][0] = (m[1][0] * m[2][1] - m[2][0] * m[1][1]) * invDet;
    result.m[2][1] = (m[2][0] * m[0][1] - m[0][0] * m[2][1]) * invDet;
    result.m[2][2] = (m[0][0] * m[1][1] - m[1][0] * m[0][1]) * invDet;
    
    // Inverse de la translation
    result.m[0][3] = -(result.m[0][0] * m[0][3] + result.m[0][1] * m[1][3] + result.m[0][2] * m[2][3]);
    result.m[1][3] = -(result.m[1][0] * m[0][3] + result.m[1][1] * m[1][3] + result.m[1][2] * m[2][3]);
    result.m[2][3] = -(result.m[2][0] * m[0][3] + result.m[2][1] * m[1][3] + result.m[2][2] * m[2][3]);
    
    return result;
}