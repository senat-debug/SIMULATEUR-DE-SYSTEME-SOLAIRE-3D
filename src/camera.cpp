#include "Camera.h"
#include "Constants.h"
#include <cmath>

Camera::Camera(float aspectRatio, float fov)
    : target(0.0f, 0.0f, 0.0f)
    , up(0.0f, 1.0f, 0.0f)
    , distance(Constants::CAMERA_DISTANCE)
    , yaw(0.0f)
    , pitch(0.3f)
    , fov(fov)
    , aspectRatio(aspectRatio)
    , nearPlane(Constants::CAMERA_NEAR)
    , farPlane(Constants::CAMERA_FAR) {
    
    updatePosition();
    updateMatrices();
}

Camera::~Camera() {}

void Camera::updatePosition() {
    // Position de la caméra en coordonnées sphériques
    position.x = target.x + std::sin(yaw) * std::cos(pitch) * distance;
    position.y = target.y + std::sin(pitch) * distance;
    position.z = target.z + std::cos(yaw) * std::cos(pitch) * distance;
}

void Camera::updateMatrices() {
    viewMatrix = Matrix4x4::lookAt(position, target, up);
    projectionMatrix = Matrix4x4::perspective(fov, aspectRatio, nearPlane, farPlane);
}

void Camera::update() {
    updatePosition();
    updateMatrices();
}

void Camera::rotate(float deltaYaw, float deltaPitch) {
    yaw += deltaYaw;
    pitch += deltaPitch;
    
    // Limiter le pitch pour éviter le gimbal lock
    const float maxPitch = Constants::HALF_PI - 0.1f;
    if (pitch > maxPitch) pitch = maxPitch;
    if (pitch < -maxPitch) pitch = -maxPitch;
    
    update();
}

void Camera::zoom(float delta) {
    distance += delta;
    
    // Limiter la distance
    if (distance < Constants::CAMERA_MIN_DISTANCE) {
        distance = Constants::CAMERA_MIN_DISTANCE;
    }
    if (distance > Constants::CAMERA_MAX_DISTANCE) {
        distance = Constants::CAMERA_MAX_DISTANCE;
    }
    
    update();
}

void Camera::setDistance(float dist) {
    distance = dist;
    update();
}

void Camera::setTarget(const Vector3D& newTarget) {
    target = newTarget;
    update();
}

Matrix4x4 Camera::getViewMatrix() const {
    return viewMatrix;
}

Matrix4x4 Camera::getProjectionMatrix() const {
    return projectionMatrix;
}

Matrix4x4 Camera::getViewProjectionMatrix() const {
    return projectionMatrix * viewMatrix;
}

Vector3D Camera::getPosition() const {
    return position;
}

Vector3D Camera::getTarget() const {
    return target;
}

float Camera::getDistance() const {
    return distance;
}

void Camera::setAspectRatio(float ratio) {
    aspectRatio = ratio;
    updateMatrices();
}