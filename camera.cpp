#include "camera.h"

Camera::Camera(){ }

Camera::Camera(glm::vec3 startPosition, glm::vec3 startUp, GLfloat startYaw, GLfloat startPitch, GLfloat startMoveSpeed, GLfloat startTurnSpeed)
{
    position = startPosition;
    worldUp = startUp;
    yaw = startYaw;
    pitch = startPitch;

    front = glm::vec3(0.0f, 0.0f, -1.0f);

    moveSpeed = startMoveSpeed;
    turnSpeed = startTurnSpeed;

    Update();
}

void Camera::KeyControls(bool *keys, GLfloat deltaTime)
{
    GLfloat velocity = moveSpeed * deltaTime;

    if (keys[GLFW_KEY_W] == true || keys[GLFW_KEY_UP] == true)
    {
        position += (front * velocity);
    }

    if (keys[GLFW_KEY_S] == true || keys[GLFW_KEY_DOWN] == true)
    {
        position -= (front * velocity);
    }

    if (keys[GLFW_KEY_A] == true || keys[GLFW_KEY_LEFT] == true)
    {
        position -= (right * velocity);
    }

    if (keys[GLFW_KEY_D] == true || keys[GLFW_KEY_RIGHT] == true)
    {
        position += (right * velocity);
    }
}

void Camera::MouseControl(GLfloat xChange, GLfloat yChange)
{
    xChange *= turnSpeed;
    yChange *= turnSpeed;

    // across X axis
    yaw += xChange;
    // across y change
    pitch += yChange;

    // can't turn your head beyond certain limit
    if (pitch > 89.0f)
    {
        pitch = 89.0;
    }

    if (pitch < -89.0f)
    {
        pitch = -89.0;
    }

    Update();
}

glm::mat4 Camera::CalculateViewMatrix()
{
    // look in front of us
    return glm::lookAt(position, position + front, up);
}

void Camera::Update()
{
    // Refer theory lesson
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * sin(glm::radians(pitch));

    front = glm::normalize(front);

    right = glm::normalize(glm::cross(front, worldUp));
    up = glm::normalize(glm::cross(right, front));

}

Camera::~Camera()
{
    //dtor
}
