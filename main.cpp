#include <iostream>
#include <string.h>
#include <stdio.h>
#include <fstream>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/mat4x4.hpp>

#include <mesh.h>
#include <shader.h>
#include <window.h>

using namespace std;

const float toRadians = 3.14159265f / 180.0f;

vector<Mesh*> meshList;
vector<Shader> shaderList;
Window mainWindow = Window(1000, 700);

// Vertex Shader
static const char* vShader = "res/basicShader.vs";

// Fragment Shader
static const char* fShader = "res/basicShader.fs";

bool direction = true,
     sizeDirection = true;
float triOffset = 0.0f,
      triMaxOffset = 0.7f, // at this offset, we'll switch direction
      triIncrement = 0.005f,
      curAngle = 0.0f,
      curSize = 0.4f,
      maxSize = 0.8f,
      minSize = 0.1f;




void CreateObjects()
{
    // 4 sides of the pyramid
    unsigned int indices[] = {
        0, 3, 1,
        1, 3, 2,
        2, 3, 0,
        0, 1, 2
    };
    //x, y, z for each vertex
    // center of the screen is 0, 0
    // max values swing from 1 to -1
    GLfloat vertices[] = {
        -1.0f, -1.0f, 0.0f,
         0.0f, -1.0f, 1.0f,
         1.0f, -1.0f, 0.0f,
         0.0f, 1.0f, 0.0f
    };

    Mesh *obj1 = new Mesh();
    obj1->CreateMesh(vertices, indices, 12, 12);
    meshList.push_back(obj1);
    Mesh *obj2 = new Mesh();
    obj2->CreateMesh(vertices, indices, 12, 12);
    meshList.push_back(obj2);
}

void CreateShaders()
{
    Shader* shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}

int main()
{

    mainWindow.initialize();
    CreateObjects();
    CreateShaders();

    GLuint uniformModel, uniformProjection;

    // fov: using standard. It is the field of view in the y direction
    // aspect: width of window divided by the height of window
    // nearest thing you can see
    // farthest thing you can see
    glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.GetBufferWidth() / (GLfloat)mainWindow.GetBufferHeight(), 0.1f, 100.0f);

    // loop until window closed
    while (!mainWindow.GetShouldClose())
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

        if(direction)
        {
            curSize += 0.001f;
        } else {
            curSize -= 0.001f;
        }

        if(curSize >= maxSize || curSize <= minSize)
        {
            sizeDirection = !sizeDirection;
        }


        // clear window
        glClearColor(0.0f, 0.0f, 0.0f, 0.1f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shaderList[0].UseShader();
		uniformModel = shaderList[0].GetModelLocation();
		uniformProjection = shaderList[0].GetProjectionLocation();




            // Order matters. What is you see happens from bottom to top.

            // Default initialization is to all zeroes matrix, thus initialising to identity matrix
            glm::mat4 model = glm::mat4(1.0);

            // rotate about z-axis
            //model = glm::rotate(model, curAngle * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
            //model = glm::translate(model, glm::vec3(triOffset, triOffset, 0.0f));
            //model = glm::scale(model, glm::vec3(curSize, curSize, 1.0f));

            model = glm::translate(model, glm::vec3(triOffset, triOffset, -2.5f));
            //model = glm::rotate(model, curAngle * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
            model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));


            glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
            glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));

            meshList[0]->RenderMesh();

            model = glm::mat4(1.0);

            model = glm::translate(model, glm::vec3(-triOffset, triOffset, -2.5f));
            model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));


            glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
            //glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));

            meshList[1]->RenderMesh();

        // unassign shader;
        glUseProgram(0);

        mainWindow.SwapBuffers();
    }


    return 0;
}
