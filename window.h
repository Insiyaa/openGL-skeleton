#ifndef WINDOW_H
#define WINDOW_H

#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>


class Window
{
public:
	Window();
	Window(GLint windowWidth, GLint windowHeight);

	int initialize();

	GLint GetBufferWidth() { return bufferWidth; }
	GLint GetBufferHeight() { return bufferHeight; }
	bool GetShouldClose() { return glfwWindowShouldClose(mainWindow); }

	bool* GetKeys() { return keys; }
	GLfloat GetXChange();
	GLfloat GetYChange();

	void SwapBuffers() { glfwSwapBuffers(mainWindow); }

	virtual ~Window();
private:
	GLFWwindow* mainWindow;

	GLint width, height;
	GLint bufferWidth, bufferHeight;

	bool keys[1024];

	GLfloat lastX,
            lastY,
            xChange,
            yChange;

    bool mouseFirstMoved;

	void CreateCallbacks();

	// static because we have to use as a callback. normal member functions won't work
	// parameters according to callback req
	// A static function is a member function of a class that can be called even when an object of the class is not initialized. It doesn't know the object
	static void HandleKeys(GLFWwindow *window, int key, int code, int action, int mode);

	static void HandleMouse(GLFWwindow *window, double xPos, double yPos);
};

#endif // WINDOW_H
