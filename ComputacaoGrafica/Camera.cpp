#include "Camera.h"

glm::mat4 Camera::getViewMatrix() {
    return glm::lookAt(position, position + front, up);
}

void Camera::processKeyboard(Camera_Movement direction, float deltaTime) {
    float velocity = movementSpeed * deltaTime;
    if (direction == FORWARD)
        position += front * velocity;
    if (direction == BACKWARD)
        position -= front * velocity;
    if (direction == LEFT)
        position -= right * velocity;
    if (direction == RIGHT)
        position += right * velocity;

}

void Camera::processMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch) {
    xoffset *= mouseSensitivity;
    yoffset *= mouseSensitivity;

    yaw += xoffset;
    pitch += yoffset;

    if (constrainPitch) {
        if (pitch > MAX_PITCH) {
            pitch = MAX_PITCH;
        }
        if (pitch < MIN_PITCH) {
            pitch = MIN_PITCH;
        }
    }
    updateCameraVectors();

}

void Camera::processMouseScroll(float yoffset) {
    if (zoom >= MIN_ZOOM && zoom <= MAX_ZOOM) {
        zoom -= yoffset;
    }
    if (zoom <= MIN_ZOOM) {
        zoom = MIN_ZOOM;
    }
    if (zoom >= MAX_ZOOM) {
        zoom = MAX_ZOOM;
    }
}

void Camera::updateCameraVectors() {
    // Calculate the new Front vector
    glm::vec3 _front;
    _front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    _front.y = sin(glm::radians(pitch));
    _front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

    this->front = glm::normalize(_front);

    // Also re-calculate the Right and Up vector
    right = glm::normalize(glm::cross(front, worldUp)); //Normalize the vectors, because their length gets closer to 0 
    up = glm::normalize(glm::cross(right, front)); //     the more you look up or down which results in slower movement.
}