#ifndef COMPGRAF_CAMERA_H
#define COMPGRAF_CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

enum CameraMovement {
    BACKWARD,
    FORWARD,
    LEFT,
    RIGHT
};

class Camera {


public:
    int WIDTH = 800;
    int HEIGHT = 600;
    float yaw = -90.0f;
    float pitch = 0.0f;
    float speed = 2.5f;
    float sensitivity = 0.1f;
    float fieldOfView = 45.0f;

    glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 front = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 right = calculateRight();

    glm::vec3 target = glm::vec3(0.0f, 0.0f, 0.0f);


    glm::mat4 getViewMatrix() {
        return glm::lookAt(position, position + front, up);
    }

    glm::mat4 getProjectionMatrix() {
        return glm::perspective(glm::radians(fieldOfView), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);
    }

    Camera() {
        updateCameraVectors();
    }

    void processKeyboardInput(CameraMovement direction, float deltaTime) {
        float velocity = speed * deltaTime;
        if (direction == FORWARD) {
            position = position + (front * velocity);
        }

        if (direction == BACKWARD) {
            position = position - (front * velocity);
        }

        if (direction == LEFT) {
            position = position - (right * velocity);
        }

        if (direction == RIGHT) {
            position = position + (right * velocity);
        }
    }

    void processMouseInput(float offsetx, float offsety, bool constainpitch = true) {
        offsetx = offsetx * sensitivity;
        offsety = offsety * sensitivity;

        yaw = yaw + offsetx;
        pitch = pitch + offsety;

        if (constainpitch) {
            if (pitch > 89.0f)
                pitch = 89.0f;
            if (pitch < -89.0f)
                pitch = -89.0f;
        }

        updateCameraVectors();
    }

    glm::vec3 calculateUp() {
        return glm::normalize(glm::cross(right, front));
    }

private:
    void updateCameraVectors() {
        glm::vec3 front;
        front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        front.y = sin(glm::radians(pitch));
        front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        this->front = glm::normalize(front);
        this->right = calculateRight();
        this->up = calculateUp();
    }

    glm::vec3 calculateRight() {
        return glm::normalize(glm::cross(front, up));
    }


};
#endif //COMPGRAF_CAMERA_H