#include <gl/glew.h>

#include <GLFW/glfw3.h>

#include <iostream>

int main()
{
	if (!glewInit())
	{
		std::cout << "Failed to init glew" << std::endl;
	}

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


}