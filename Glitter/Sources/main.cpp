// Local Headers
#include "glitter.hpp"
// System Headers
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// Standard Headers
#include <cstdio>
#include <cstdlib>

//// Helper functions
#include <camera.hpp>
#include <shader.hpp>
#include <model.hpp>
#include <filesystem.hpp>

#include "Renderer.hpp"


#define MODEL "Resources/crytek_sponza/sponza.obj"

// Main's Functions
GLFWwindow* SetUpGLFW();
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);

Renderer renderer;

int main(int argc, char * argv[]) {

	GLFWwindow *mWindow = SetUpGLFW();

	renderer.SetCamera(glm::vec3(0.f, 0.f, 2.f));
	renderer.SetWorldModel(FileSystem::getPath(MODEL).c_str());

    // Rendering Loop
	double oldTime = glfwGetTime();
    while (glfwWindowShouldClose(mWindow) == false) 
	{
		glfwPollEvents();
		renderer.Render();
		renderer.Update(glfwGetTime() - oldTime);

        // Flip Buffers and Draw
        glfwSwapBuffers(mWindow);
		oldTime = glfwGetTime();
    }   
	
	glfwTerminate();
    return EXIT_SUCCESS;
}

GLFWwindow* SetUpGLFW()
{
	// Load GLFW and Create a Window
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	GLFWwindow *mWindow = glfwCreateWindow(mWidth, mHeight, "OpenGL", nullptr, nullptr);

	// Check for Valid Context
	if (mWindow == nullptr)
	{
		fprintf(stderr, "Failed to Create OpenGL Context");
		exit(1);
	}

	// Create Context and Load OpenGL Functions
	glfwMakeContextCurrent(mWindow);
	gladLoadGL();
	fprintf(stderr, "OpenGL %s\n", glGetString(GL_VERSION));

	// Set callback functions
	glfwSetKeyCallback(mWindow, key_callback);
	glfwSetCursorPosCallback(mWindow, mouse_callback);
	glfwSetScrollCallback(mWindow, scroll_callback);
	return mWindow;
}

// Is called whenever a key is pressed/released via GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if ((key == GLFW_KEY_ESCAPE || key == GLFW_KEY_Q) && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	if (action == GLFW_PRESS)
	{
		renderer.OnKeyPressed(key);
	}
	if (action == GLFW_RELEASE)
	{
		renderer.OnKeyReleased(key);
	}
}

int leftButtonState, rightButtonState;
bool leftButtonPressed = false, rightButtonPressed = false;
GLfloat lastX = 400, lastY = 300;

bool firstMouse = true;

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	int leftButtonState = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
	int rightButtonState = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT);

	// It only matters when the buttons are first pressed because they continue to be pressed
	if (!leftButtonPressed && leftButtonState == GLFW_PRESS)
	{
		renderer.OnMouseButtonPressed(GLFW_MOUSE_BUTTON_LEFT);
	}
	if (leftButtonPressed && leftButtonState == GLFW_RELEASE)
	{
		renderer.OnMouseButtonReleased(GLFW_MOUSE_BUTTON_LEFT);
	}
	if (!rightButtonPressed && rightButtonState == GLFW_PRESS)
	{
		renderer.OnMouseButtonPressed(GLFW_MOUSE_BUTTON_RIGHT);
	}
	if (rightButtonPressed && rightButtonState == GLFW_RELEASE)
	{
		renderer.OnMouseButtonReleased(GLFW_MOUSE_BUTTON_RIGHT);
	}

	leftButtonPressed = leftButtonState == GLFW_PRESS;
	rightButtonPressed = rightButtonState == GLFW_PRESS;

	renderer.OnMouseMoved(xpos - lastX, lastY - ypos);

	lastX = xpos;
	lastY = ypos;
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	renderer.OnMouseScrolled(yoffset);
}
