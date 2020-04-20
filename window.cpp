#include "window.h"

Window::Window()
{
	width = 800;
	height = 600;

	bufferWidth = 0, bufferHeight = 0;

	mainWindow = 0;

	for (size_t i = 0; i < 1024; i++)
        keys[i] = 0;
}

Window::Window(GLint windowWidth, GLint windowHeight)
{
	width = windowWidth;
	height = windowHeight;

	bufferWidth = 0, bufferHeight = 0;

	mainWindow = 0;

	for (size_t i = 0; i < 1024; i++)
        keys[i] = 0;
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

    // handle key + mouse input
    CreateCallbacks();
    glfwSetInputMode(mainWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

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

    // owner of the window is 'this' class
    // helps the callback function to know the class
    glfwSetWindowUserPointer(mainWindow, this);

	return 0;
}

void Window::CreateCallbacks()
{
    glfwSetKeyCallback(mainWindow, HandleKeys);
    glfwSetCursorPosCallback(mainWindow, HandleMouse);
}

GLfloat Window::GetXChange()
{
    GLfloat theChange = xChange;
    xChange = 0.0f;
    return theChange;
}

GLfloat Window::GetYChange()
{
    GLfloat theChange = yChange;
    yChange = 0.0f;
    return theChange;
}

void Window::HandleKeys(GLFWwindow *window, int key, int code, int action, int mode)
{
    // to access keys[], getting the current window object
    Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }

    if (key >= 0 && key < 1024)
    {
        if (action == GLFW_PRESS)
        {
            theWindow -> keys[key] = true;
        }
        else if (action == GLFW_RELEASE)
        {
            theWindow->keys[key] = false;
        }
    }
}

void Window::HandleMouse(GLFWwindow *window, double xPos, double yPos)
{
    Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

    if (theWindow->mouseFirstMoved)
    {
        theWindow->lastX = xPos;
        theWindow->lastY = yPos;
        theWindow->mouseFirstMoved = false;
    }
    theWindow->xChange = xPos - theWindow->lastX;
    // to prevent inverted up and down movements, depends on coordinate sys
    theWindow->yChange = theWindow->lastY - yPos;

    theWindow->lastX = xPos;
    theWindow->lastY = yPos;

}

Window::~Window()
{
	glfwDestroyWindow(mainWindow);
	glfwTerminate();
}
