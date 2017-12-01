#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

void window_resized(GLFWwindow* window, int width, int height);
void key_pressed(GLFWwindow* window, int key, int scancode, int action, int mods);
void show_glfw_error(int error, const char* description);
static void errorCallback(int errorCode, const char* errorDescription);


int main()
{
	//glfwSetErrorCallback(show_glfw_error);
	glfwSetErrorCallback(errorCallback);


	if (!glfwInit()) {
		std::cerr << "GLFW 초기화 실패" << '\n';
		exit(EXIT_FAILURE);
	}


	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	//glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	glfwWindowHint(GLFW_SAMPLES, 4);

	GLFWwindow* window = glfwCreateWindow(
		800, // width
		600, // height
		"OpenGL Example",
		NULL, NULL);
	if (!window)
	{
		std::cerr << "윈도우 생성 실패" << '\n';
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(window);


	glfwSetWindowSizeCallback(window, window_resized);
	glfwSetKeyCallback(window, key_pressed);

	glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if (err != GLEW_OK) {
		std::cerr << "GLEW 초기화 실패 " << glewGetErrorString(err) << '\n';
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;
	std::cout << "GLSL version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
	std::cout << "Vendor: " << glGetString(GL_VENDOR) << std::endl;
	std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;

	int nr_extensions = 0;
	glGetIntegerv(GL_NUM_EXTENSIONS, &nr_extensions);

	for (int i = 0; i < nr_extensions; ++i) {
		std::cout << glGetStringi(GL_EXTENSIONS, i) << '\n';
	}


	glClearColor(0, 0, 1, 1);

	glfwSwapInterval(0);

	double lastTime = glfwGetTime();
	int numOfFrames = 0;
	int count = 0;

	while (!glfwWindowShouldClose(window)) {

		double currentTime = glfwGetTime();
		numOfFrames++;


		if (currentTime - lastTime >= 1.0) {

			printf("%f ms/frame  %d fps \n", 1000.0 / double(numOfFrames), numOfFrames);
			numOfFrames = 0;
			lastTime = currentTime;
		}

		if (count % 2 == 0)
			glClearColor(0, 0, 1, 0);
		else
			glClearColor(1, 0, 0, 0);

		glClear(GL_COLOR_BUFFER_BIT);

		count++;

		glfwSwapBuffers(window);

		glfwPollEvents();
		//glfwWaitEvents();
	}


	glfwDestroyWindow(window);


	glfwTerminate();
	return 0;
}


void show_glfw_error(int error, const char* description) {
	std::cerr << "Error: " << description << '\n';
}

static void errorCallback(int errorCode, const char* errorDescription)
{
	fprintf(stderr, "Error: %s\n", errorDescription);
}

void window_resized(GLFWwindow* window, int width, int height) {
	std::cout << "Window resized, new window size: " << width << " x " << height << '\n';

	glClearColor(0, 0, 1, 1);
	glClear(GL_COLOR_BUFFER_BIT);
	glfwSwapBuffers(window);
}

void key_pressed(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (key == 'Q' && action == GLFW_PRESS) {
		glfwTerminate(); 		//glfwSetWindowShouldClose(window, GLFW_TRUE);
		exit(0);
	}
}
