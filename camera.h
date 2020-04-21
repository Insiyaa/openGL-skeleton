#ifndef CAMERA_H
#define CAMERA_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>

#include <glm/gtc/matrix_transform.hpp>

class Camera
{
    public:
        Camera();
        Camera(glm::vec3 startPosition, glm::vec3 startUp, GLfloat startYaw, GLfloat startPitch, GLfloat startMoveSpeed, GLfloat startTurnSpeed);

        void KeyControls(bool *keys, GLfloat deltaTime);
        void MouseControl(GLfloat xChange, GLfloat yChange);

        glm::mat4 CalculateViewMatrix();

        virtual ~Camera();

    protected:

    private:
        glm::vec3 position,
                  front,
                  up,
                  right,
                  worldUp;
        GLfloat yaw,    // looking left right
                pitch;  // looking up and down... roll for rotating head
        GLfloat moveSpeed,
                turnSpeed;

        void Update();

};

#endif // CAMERA_H
