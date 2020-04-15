#include "window.h"

Window::Window()
{
	width = 800;
	height = 600;
	bufferWidth = 0, bufferHeight = 0;
	mainWindow = 0;
}

Window::Window(GLint windowWidth, GLint windowHeight)
{
	width = windowWidth;
	height = windowHeight;
	bufferWidth = 0, bufferHeight = 0;
	mainWindow = 0;
}

int Window::initialize()
{
	// intialize GLFW
    if (!glfwInit())
    {
        //cerr << "GLFW INIT FAILED!\n";
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

    mainWindow = glfwCreateWindow(width, height, "Main Window", NULL, NULL);
    if (!mainWindow)
    {
//        cerr << "GLFW window creation failed!\n";
        glfwTerminate();
        return 1;
    }

    // Get Buffer size information
    // not window size, but instead where we can draw
    glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

    // set context for glew to use
    glfwMakeContextCurrent(mainWindow);

    // allow modern extension features
    glewExperimental = GL_TRUE;

    // init glew
    if (glewInit() != GLEW_OK)
    {
//        cerr << "GLEW INIT FAILED!\n";
        glfwDestroyWindow(mainWindow);
        glfwTerminate();
        return 1;
    }

    // which triangle to draw over top of the other
    glEnable(GL_DEPTH_TEST);

    // setup viewport size
    glViewport(0, 0, bufferWidth, bufferHeight);

	return 0;
}


Window::~Window()
{
	glfwDestroyWindow(mainWindow);
	glfwTerminate();
}
