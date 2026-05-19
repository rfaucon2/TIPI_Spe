#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
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
GLFWwindow* init_window(int window_size)
{ 
    glfwInit(); 
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); 
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); // Set glfw version to 3.3
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow *window = glfwCreateWindow(window_size, window_size, "openGL_course", GL_NONE, GL_NONE);

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
    int window_size = 1000;
    GLFWwindow* window = init_window(window_size);	
    Galaxy galaxy(window_size, Algorithm::Barnes_Hut);

    double curent_time = 0.f;
    double prev_time = 0.f;
    int fps = 0.f;

    int frame_counter = 0;
    std::ofstream record("Data/BH_record.csv");
    std::cout << record.is_open() << std::endl;
    record << "time, vx, vy\n";

    while (!glfwWindowShouldClose(window) /*&& frame_counter <= 500*/) // Main Loop
	{
        process_input(window);

        curent_time = glfwGetTime();
        galaxy.Update();
        Vector2 speed = galaxy.get_star_speed(0);
        record << frame_counter << ", " << speed.x << ", " << speed.y <<"\n";
        
        double dt = curent_time - prev_time;
        prev_time = curent_time;
        glfwSetWindowTitle(window, (std::string("Simulation: ") + std::to_string(1/dt)).c_str());
		glClearColor(0.f, 0.f, 0.f, 0.f);
		glClear(GL_COLOR_BUFFER_BIT); // Apply the color previously set

		galaxy.Draw();

		glfwSwapBuffers(window); 
		glfwPollEvents();
        frame_counter += 1;
	}

    record.close();
	glfwTerminate();
	return 0;
}
