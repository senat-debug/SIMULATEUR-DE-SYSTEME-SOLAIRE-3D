#ifndef MATRIX4X4_H
#define MATRIX4X4_H

#include "Vector3D.h"


class Matrix4x4 {
private:
    float m[4][4];  // Matrice 4x4 en row-major
    
public:
    
    // CONSTRUCTEURS
    
    
     Matrix4x4();
    
    
    Matrix4x4(float values[4][4]);
    
    
    // MATRICES SPÉCIALES
    
    
    
    static Matrix4x4 identity();
    
    
    static Matrix4x4 translation(float x, float y, float z);
    
    
    static Matrix4x4 rotationX(float angle);
    
    
    static Matrix4x4 rotationY(float angle);
    
   
    static Matrix4x4 rotationZ(float angle);
    
    
    static Matrix4x4 scale(float sx, float sy, float sz);
    
    
    static Matrix4x4 lookAt(const Vector3D& eye, 
                            const Vector3D& target, 
                            const Vector3D& up);
    
    
    static Matrix4x4 perspective(float fov, float aspect, float near, float far);
    
    
    // OPÉRATEURS
    
    Matrix4x4 operator*(const Matrix4x4& other) const;
    
    
    Vector3D operator*(const Vector3D& vec) const;
    

    // TRANSFORMATIONS
    
    Vector3D transformPoint(const Vector3D& point) const;
    
    
    Vector3D transformVector(const Vector3D& vec) const;
    
    
    // ACCÈS AUX ÉLÉMENTS
    
    
    /**
     * @brief Accès en lecture/écriture
     */
    float& at(int row, int col);
    
    /**
     * @brief Accès en lecture seule
     */
    float at(int row, int col) const;
    
    
    const float* data() const;
  
    // UTILITAIRES
   
    
  
    void print() const;
    
    
    Matrix4x4 transposed() const;
    
    
    Matrix4x4 inverse() const;
};

#endif 