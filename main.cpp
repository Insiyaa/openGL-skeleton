#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string.h>
#include <stdio.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/mat4x4.hpp>

using namespace std;

const GLint WIDTH = 800, HEIGHT = 600;
const float toRadians = 3.14159265f / 180.0f;

GLuint  VAO,
        VBO,
        shader,
        //uniformXMove;
        uniformModel;

bool direction = true;
float triOffset = 0.0f,
      triMaxOffset = 0.7f, // at this offset, we'll switch direction
      triIncrement = 0.005f,
      curAngle = 0.0f;

// Vertex Shader
static const char* vShader = "                                           \n\
#version 330                                                             \n\
                                                                         \n\
layout (location = 0) in vec3 pos;                                       \n\
uniform mat4 model;                                                      \n\
void main()                                                              \n\
{                                                                        \n\
    gl_Position = model * vec4(0.4 * pos.x, 0.4 * pos.y, pos.z, 1.0f);   \n\
}";

// Fragment Shader
static const char* fShader = "                                           \n\
#version 330                                                             \n\
                                                                         \n\
out vec4 colors;                                                         \n\
void main()                                                              \n\
{                                                                        \n\
    colors = vec4(1.0f, 0.0f, 0.0f, 1.0f);                               \n\
}";

void CreateTriangle()
{
    //x, y, z for each vertex
    // center of the screen is 0, 0
    // max values swing from 1 to -1

    GLfloat vertices[] = {
        -1.0f, -1.0f, 0.0f,
        1.0f, -1.0f, 0.0f,
        0.0f, 1.0f, 0.0f
    };

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
            glEnableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);


}

void AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType)
{
    GLuint theShader = glCreateShader(shaderType);
    const GLchar* theCode[1];
    theCode[0] = shaderCode;

    GLint codeLength[1];
    codeLength[0] = strlen(shaderCode);

    glShaderSource(theShader, 1, theCode, codeLength);
    glCompileShader(theShader);

    GLint result = 0;
    GLchar eLog[1024] = { 0 };

    glGetShaderiv(theShader, GL_COMPILE_STATUS, &result);

    if (!result)
    {
        glGetShaderInfoLog(theShader, sizeof(eLog), NULL, eLog);
        std::cerr << "Error Compiling: " << shaderType << eLog << std::endl;
        return;
    }
    glAttachShader(theProgram, theShader);
}

void CompileShaders()
{
    shader =  glCreateProgram();

    if (!shader)
    {
        std::cerr << "Error creating shader!\n";
        return;
    }

    AddShader(shader, vShader, GL_VERTEX_SHADER);
    AddShader(shader, fShader, GL_FRAGMENT_SHADER);

    GLint result = 0;
    GLchar eLog[1024] = { 0 };

    glLinkProgram(shader);
    glGetProgramiv(shader, GL_LINK_STATUS, &result);

    if (!result)
    {
        glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
        std::cerr << "Error Linking: " << eLog << std::endl;
        return;
    }

    glValidateProgram(shader);
    glGetProgramiv(shader, GL_VALIDATE_STATUS, &result);

    if (!result)
    {
        glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
        std::cerr << "Error Validating: " << eLog << std::endl;
        return;
    }

    // get uniform variable ID
    //uniformXMove = glGetUniformLocation(shader, "xMove");
    uniformModel = glGetUniformLocation(shader, "model");
}

int main()
{
    // intialize GLFW
    if (!glfwInit())
    {
        cerr << "GLFW INIT FAILED!\n";
        glfwTerminate();
        return 1;
    }

    // Setup GLFW window properties

    // OpenGL Version
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    // not backward compatible, not using depracated stuff
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // allow frwd compatibitlty
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow *mainWindow = glfwCreateWindow(WIDTH, HEIGHT, "Test Window", NULL, NULL);
    if (!mainWindow)
    {
        cerr << "GLFW window creation failed!\n";
        glfwTerminate();
        return 1;
    }

    // Get Buffer size information
    // not window size, but instead where we can draw
    int bufferWidth, bufferHeight;
    glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

    // set context for glew to use
    glfwMakeContextCurrent(mainWindow);

    // allow modern extension features
    glewExperimental = GL_TRUE;

    // init glew
    if (glewInit() != GLEW_OK)
    {
        cerr << "GLEW INIT FAILED!\n";
        glfwDestroyWindow(mainWindow);
        glfwTerminate();
        return 1;
    }

    // setup viewport size
    glViewport(0, 0, bufferWidth, bufferHeight);

    CreateTriangle();
    CompileShaders();

    // loop until window closed
    while (!glfwWindowShouldClose(mainWindow))
    {
        // get and handle user input events
        glfwPollEvents();

        if (direction)
        {
            triOffset += triIncrement;
        } else {
            triOffset -= triIncrement;
        }
        if (abs(triOffset) >= triMaxOffset)
            direction = !direction;

        curAngle += 0.1f;
        if (curAngle >= 360)
        {
            curAngle -= 360;
        }


        // clear window
        glClearColor(0.0f, 0.0f, 0.0f, 0.1f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shader);


            // Order matters. What is you see happens from bottom to top.

            // Default initialization is to all zeroes matrix, thus initialising to identity matrix
            glm::mat4 model = glm::mat4(1.0);

            // rotate about z-axis
            model = glm::rotate(model, curAngle * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
            model = glm::translate(model, glm::vec3(triOffset, triOffset, 0.0f));



            glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

            glBindVertexArray(VAO);
                glDrawArrays(GL_TRIANGLES, 0, 3);
            glBindVertexArray(0);

        // unassign shader;
        glUseProgram(0);

        glfwSwapBuffers(mainWindow);
    }


    return 0;
}
