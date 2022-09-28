#ifndef CAMERA_H
#define CAMERA_H

#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>

#include <vector>


// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};


// Default camera values
const float DEFAULT_YAW = -100.0f;
const float DEFAULT_PITCH = 0.0f;
const float DEFAULT_SPEED = 2.5f;
const float DEFAULT_SENSITIVITY = 0.1f;
const float DEFAULT_ZOOM = 10.0f;
const glm::vec3 DEFAULT_POSITION = glm::vec3(0.0f, 0.0f, 0.0f);
const glm::vec3 DEFAULT_UP = glm::vec3(0.0f, 2.0f, 0.0f);
const glm::vec3 DEFAULT_FRONT = glm::vec3(3.0f, 0.0f, 0.0f);

//Configs
const float MAX_PITCH = 89.0f;
const float MIN_PITCH = -89.0f;
const float MAX_ZOOM = 20.0f;
const float MIN_ZOOM = 1.0f;


class Camera {

public:
    // Camera Attributes
    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;
    glm::vec3 worldUp;

    // Euler Angles
    float yaw;
    float pitch;

    // Camera options
    float movementSpeed;
    float mouseSensitivity;
    float zoom;

    // Constructor with vectors
    Camera(glm::vec3 position = DEFAULT_POSITION,
        glm::vec3 up = DEFAULT_UP,
        float yaw = DEFAULT_YAW, float pitch = DEFAULT_PITCH) : front(DEFAULT_FRONT),
        movementSpeed(DEFAULT_SPEED),
        mouseSensitivity(DEFAULT_SENSITIVITY),
        zoom(DEFAULT_ZOOM) {
        this->position = position;
        this->worldUp = up;
        this->yaw = yaw;
        this->pitch = pitch;

        updateCameraVectors();
    }

    // Constructor with scalar values
    Camera(float posX, float posY, float posZ,
        float upX, float upY, float upZ,
        float yaw, float pitch) : front(DEFAULT_FRONT),
        movementSpeed(DEFAULT_SPEED),
        mouseSensitivity(DEFAULT_SENSITIVITY),
        zoom(DEFAULT_ZOOM) {

        this->position = glm::vec3(posX, posY, posZ);
        this->worldUp = glm::vec3(upX, upY, upZ);
        this->yaw = yaw;
        this->pitch = pitch;

        updateCameraVectors();
    }


    /*
     *  Returns the view matrix calculated using Euler Angles and the LookAt Matrix
     */
    glm::mat4 getViewMatrix();

    /*
     * Processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
     */
    void processKeyboard(Camera_Movement direction, float deltaTime);

    /*
     * Processes input received from a mouse input system. Expects the offset value in both the x and y direction.
     */
    void processMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true);


    /*
     * Processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
     */
    void processMouseScroll(float yoffset);

    glm::vec3 calculateUp() {
        return glm::normalize(glm::cross(right, front));
    }

private:


    /*
     * Calculates the front vector from the Camera's (updated) Euler Angles
     */
    void updateCameraVectors();

};

#endif