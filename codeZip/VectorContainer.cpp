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
	glEnable(GL_DEPTH_TEST);

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

	Shader ourShader("core_mat.vertexshader", "core_mat.fragmentshader");
	ourShader.LoadOutFile("foil_spline.out");

	GLuint indices[] =
	{
		0, 1, 3,
		1, 2, 3
	};


	GLuint VAO;
	glGenVertexArrays(1, &VAO);

	glBindVertexArray(VAO);

	GLuint VBO;
	glGenBuffers(1, &VBO);

	GLuint vertexAttribute_buffer = VBO;
	glBindBuffer(GL_ARRAY_BUFFER, vertexAttribute_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(VertexAttribute) * ourShader.vVertexAttribute.size(), &ourShader.vVertexAttribute.front(), GL_STATIC_DRAW);
						
	GLuint vp = glGetAttribLocation(ourShader.Program, "position");
	glEnableVertexAttribArray(vp);
	glVertexAttribPointer(vp, 3, GL_FLOAT, GL_FALSE, sizeof(VertexAttribute), (GLvoid*)(0));
									
	GLuint vc = glGetAttribLocation(ourShader.Program, "color");
	glEnableVertexAttribArray(vc);
	glVertexAttribPointer(vc, 3, GL_FLOAT, GL_FALSE, sizeof(VertexAttribute), (GLvoid*)(3 * sizeof(GLfloat)));

	GLuint EBO;
	glGenBuffers(1, &EBO);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glBindVertexArray(0);

	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glDisable(GL_CULL_FACE);
		glFrontFace(GL_CCW);

		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		glUseProgram(ourShader.Program);

		glm::mat4 transform;
		transform = glm::translate(transform, glm::vec3(-2.0f, -1.5f, 0.0f));
		transform = glm::scale(transform, glm::vec3(3.0f, 3.0f, 1.0f));
		//transform = glm::rotate(transform, (GLfloat)glfwGetTime() * -5.0f, glm::vec3(0.0f, 0.0f, 1.0f));


		GLint transformLocation = glGetUniformLocation(ourShader.Program, "transform");
		glUniformMatrix4fv(transformLocation, 1, GL_FALSE, glm::value_ptr(transform));

		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, ourShader.vVertexAttribute.size());
		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		glfwSwapBuffers(window);
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

	glfwTerminate();

	return EXIT_SUCCESS;
}
