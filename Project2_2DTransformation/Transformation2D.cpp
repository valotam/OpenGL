#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Shader.h"

const GLuint WIDTH = 800, HEIGHT = 600;

int main()
{
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "LearnOpenGL", nullptr, nullptr);

	int screenWidth, screenHeight;
	glfwGetFramebufferSize(window, &screenWidth, &screenHeight);

	if (nullptr == window)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();

		return EXIT_FAILURE;
	}

	glfwMakeContextCurrent(window);

	glewExperimental = GL_TRUE;
	if (GLEW_OK != glewInit())
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return EXIT_FAILURE;
	}

	glViewport(0, 0, screenWidth, screenHeight);
	glEnable(GL_DEPTH_TEST);

	GLfloat x, y, z, angle;
	std::cout << "Set the position of a polygon." << std::endl;
	std::cout << "x: ";
	std::cin >> x;
	std::cout << "y: ";
	std::cin >> y;
	std::cout << "z: ";
	std::cin >> z;
	std::cout << "Set the angle of rotation." << std::endl;
	std::cout << "angle: ";
	std::cin >> angle;

	Shader ourShader("core.vertexshader", "core.fragmentshader");
	GLuint size = 50;
	GLfloat vertices[] = {
		-0.5f * size, -0.5f * size, -0.5f * size,  0.0f, 0.0f, 0.0f,
		0.5f *  size, -0.5f * size, -0.5f * size,  1.0f, 0.0f, 0.0f,
		0.5f *  size,  0.5f * size, -0.5f * size,  1.0f, 1.0f, 0.0f,
		0.5f *  size,  0.5f * size, -0.5f * size,  1.0f, 1.0f, 1.0f,
		-0.5f * size,  0.5f * size, -0.5f * size,  0.0f, 1.0f, 1.0f,
		-0.5f * size, -0.5f * size, -0.5f * size,  0.0f, 0.0f, 1.0f
	};

	GLuint VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	GLuint VBO;
	glGenBuffers(1, &VBO);

	GLuint vertexAttribute_buffer = VBO;
	glBindBuffer(GL_ARRAY_BUFFER, vertexAttribute_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	GLuint vp = glGetAttribLocation(ourShader.Program, "position");
	glEnableVertexAttribArray(vp);
	glVertexAttribPointer(vp, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(0));

	GLuint vc = glGetAttribLocation(ourShader.Program, "color");
	glEnableVertexAttribArray(vc);
	glVertexAttribPointer(vc, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));

	glBindVertexArray(0);

	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glDisable(GL_CULL_FACE);
		glFrontFace(GL_CCW);

		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		glUseProgram(ourShader.Program);

		glm::mat4 model;
		glm::mat4 view;
		glm::mat4 projection;
		model = glm::translate(model, glm::vec3(x, y, z));
		model = glm::rotate(model, angle, glm::vec3(0.0f, 0.0f, 1.0f));
		view = glm::translate(view, glm::vec3(screenWidth / 2, screenHeight / 2, -700.0f));
		projection = glm::ortho(0.0f, (GLfloat)screenWidth, 0.0f, (GLfloat)screenHeight, 0.1f, 1000.0f);

		GLint modelLoc = glGetUniformLocation(ourShader.Program, "model");
		GLint viewLoc = glGetUniformLocation(ourShader.Program, "view");
		GLint projLoc = glGetUniformLocation(ourShader.Program, "projection");
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));


		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLE_FAN, 0, 6);
		glBindVertexArray(0);

		glfwSwapBuffers(window);
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

	glfwTerminate();

	return EXIT_SUCCESS;
}
