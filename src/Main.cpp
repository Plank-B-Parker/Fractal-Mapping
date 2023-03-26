#include "GL/glew.h"

#include <GLFW/glfw3.h>

#include <iostream>
#include <Graphics/Shader.h>
#include <Input/Mouse.h>

#include "Math/Complex.h"

#include "Logger.h"

extern int WINDOW_HEIGHT, WINDOW_WIDTH;

int WINDOW_WIDTH = 640;
int WINDOW_HEIGHT = 640;

int setupOpenGL() {
	if (!glfwInit())
		return 0;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	return 1;
}

GLFWwindow* gimmeWindow(int width, int height){
	// Initialize the library.
	if (!setupOpenGL())
		return NULL;

	// Create a windowed mode window and its OpenGL context.
	GLFWwindow* window;

	window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Hello World", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return NULL;
	}

	// Make the window's context current.
	glfwMakeContextCurrent(window);
	
	//GLEW check
	if (glewInit() == GLEW_OK)
		std::cout << ":)" << std::endl;
	else
		std::cout << ":(" << std::endl;

	return window;

}

struct Window{

	float scale = 1.0f;
	complex centre = complex(0.0f, 0.0f);

	Window(float width, int height): width(width), height(height){

		float vertices[] = {
			-width/2.0f, -height/2.0f, 0.0f,
			width/2.0f, -height/2.0f, 0.0f,
			width/2.0f, height/2.0f, 0.0f,
			-width/2.0f, -height/2.0f, 0.0f,
			width/2.0f, height/2.0f, 0.0f,
			-width/2.0f, height/2.0f, 0.0f
		};

		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);

		glGenBuffers(1, &VBO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

	}

	void draw(){
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}

	private:
		float width, height;
		unsigned int VAO, VBO;
};



int main(void)
{

	// Create a windowed mode window and its OpenGL context.
	GLFWwindow* window = gimmeWindow(WINDOW_WIDTH, WINDOW_HEIGHT);
	glfwSetCursorPosCallback(window, mouse::mouse_callback);
	glfwSetMouseButtonCallback(window, mouse::mouse_button_callback);
	glfwSetScrollCallback(window, mouse::mouse_scroll_callback);
	//glfwSetSc
	if(window == NULL)
		return -1;

	Window mainWindow = Window(2.0f, 2.0f);

	std::cout<< "main" << std::endl; 

	Shader quadShader = Shader("QuadVertexShader.glsl", "QuadFragShader.glsl");

	complex centre = complex(0.25f, 0.0f);
	complex lastMouseClick = complex(0);
	float zoom = 1;
	float time_before = 0;
	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{

		float time_now = glfwGetTime();
		float dt = time_now - time_before;
		
		time_before = time_now;

		/* Render here */
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		quadShader.throwShade();

		complex mouse = mouse::pos/zoom + centre;
		zoom *= (1 + 0.005f * mouse::scroll);
		//std::cout << zoom << std::endl;
		centre = mouse - mouse::pos/zoom;
		if(mouse::clicked)
			lastMouseClick = mouse;
		
		quadShader.setComplex("mouse", mouse);
		quadShader.setComplex("lastMouseClick", lastMouseClick);
		quadShader.setComplex("centre", centre);
		quadShader.setFloat("zoom", zoom);
		quadShader.setFloat("clicked", mouse::clickDecay);
		quadShader.setFloat("aspRatio", (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT);

		
		mainWindow.draw();

		/* Poll for and process events */
		glfwPollEvents();
		mouse::update(dt);
		glfwGetWindowSize(window, &WINDOW_WIDTH, &WINDOW_HEIGHT);

		int pixelWidth = 0;
		int pixelHeight = 0;
		glfwGetFramebufferSize(window, &pixelWidth, &pixelHeight);
		
		glViewport(0, 0, pixelWidth, pixelWidth);

		/* Swap front and back buffers */
		glfwSwapBuffers(window);
	}

	glfwTerminate();
	return 0;
}