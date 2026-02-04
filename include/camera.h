#ifndef CAMERA_H
#define CAMERA_H

#include "Vector3D.h"
#include "Matrix4x4.h"

class Camera {
private:
    Vector3D position;
    Vector3D target;
    Vector3D up;
    
    float distance;      // Distance de la cible
    float yaw;           // Rotation horizontale (radians)
    float pitch;         // Rotation verticale (radians)
    
    float fov;           // Field of view (degrés)
    float aspectRatio;
    float nearPlane;
    float farPlane;
    
    Matrix4x4 viewMatrix;
    Matrix4x4 projectionMatrix;
    
    void updatePosition();
    void updateMatrices();
    
public:
    Camera(float aspectRatio, float fov = 60.0f);
    ~Camera();
    
    void update();
    
    // Contrôles
    void rotate(float deltaYaw, float deltaPitch);
    void zoom(float delta);
    void setDistance(float dist);
    void setTarget(const Vector3D& target);
    
    // Getters
    Matrix4x4 getViewMatrix() const;
    Matrix4x4 getProjectionMatrix() const;
    Matrix4x4 getViewProjectionMatrix() const;
    Vector3D getPosition() const;
    Vector3D getTarget() const;
    float getDistance() const;
    
    void setAspectRatio(float ratio);
};

#endif 