#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "shader.h"
#include <assert.h>
#include "Galaxy.h"

void process_input(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}
GLFWwindow* init_window()
{ 
    glfwInit(); 
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); 
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); // Set glfw version to 3.3
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow *window = glfwCreateWindow(1920, 1080, "openGL_course", GL_NONE, GL_NONE);

	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		exit(-1);
	}

	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) // Load GLAD
	{
		std::cout << "Could not load GLAD!" << std::endl;
		exit(-1);
	}
    return window;
}

int main()
{
    GLFWwindow* window = init_window();	
    Galaxy galaxy;	

	while (!glfwWindowShouldClose(window)) // Main Loop
	{
		process_input(window);
        galaxy.Update();

		glClearColor(0.f, 0.f, 0.f, 0.f);
		glClear(GL_COLOR_BUFFER_BIT); // Apply the color previously set

		galaxy.Draw();

		glfwSwapBuffers(window); 
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}
