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

	Shader ourShader("core.vertexshader", "core.fragmentshader");
	ourShader.LoadOutFile("foil_spline.out");

	// Make an airfoil with differentent Z coordinates.
	std::vector<VertexAttribute> vVertexAttributeTheOtherZ = ourShader.vVertexAttribute;
	for (VertexAttribute& var : vVertexAttributeTheOtherZ)
	{
		var.x = var.x * 1.5f;
		var.y = var.y * 1.5f;
		var.z = 2.0f;
		// Its color attribute is red.
		var.r = 1.0f;
		var.g = 0.0f;
		var.b = 0.0f;
	}
	std::vector<VertexAttribute> vVertexAttributeTheOtherZ2 = ourShader.vVertexAttribute;
	for (VertexAttribute& var : vVertexAttributeTheOtherZ2) 
	{
		var.x = var.x * 2.0f;
		var.y = var.y * 2.0f;
		var.z = 3.0f;
		// Its color attribute is red.
		var.r = 0.0f;
		var.g = 0.0f;
		var.b = 1.0f;
	}

	std::vector<VertexAttribute> vVertex;
	for each (VertexAttribute var in ourShader.vVertexAttribute)
	{
		vVertex.push_back(var);
	}
	for each (VertexAttribute var in vVertexAttributeTheOtherZ)
	{
		vVertex.push_back(var);
	}
	for each (VertexAttribute var in vVertexAttributeTheOtherZ2)
	{
		vVertex.push_back(var);
	}

	std::vector<GLuint> indices;
	for (size_t ii = 0; ii < ourShader.vVertexAttribute.size() - 1; ii++)
	{
		indices.push_back((GLuint)ii);
		indices.push_back((GLuint)ii + ourShader.vVertexAttribute.size());
		indices.push_back((GLuint)ii + 1 + ourShader.vVertexAttribute.size());

		indices.push_back((GLuint)ii);
		indices.push_back((GLuint)ii + 1);
		indices.push_back((GLuint)ii + 1 + ourShader.vVertexAttribute.size());

		indices.push_back((GLuint)ii + ourShader.vVertexAttribute.size());
		indices.push_back((GLuint)ii + 2 * ourShader.vVertexAttribute.size());
		indices.push_back((GLuint)ii + 1 + 2 * ourShader.vVertexAttribute.size());

		indices.push_back((GLuint)ii + ourShader.vVertexAttribute.size());
		indices.push_back((GLuint)ii + 1 + ourShader.vVertexAttribute.size());
		indices.push_back((GLuint)ii + 1 + 2 * ourShader.vVertexAttribute.size());
	}

	vVertexAttributeTheOtherZ2.clear();
	vVertexAttributeTheOtherZ2.shrink_to_fit();
	vVertexAttributeTheOtherZ.clear();
	vVertexAttributeTheOtherZ.shrink_to_fit();

	GLuint VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	GLuint VBO;
	glGenBuffers(1, &VBO);

	GLuint vertexAttribute_buffer = VBO;
	glBindBuffer(GL_ARRAY_BUFFER, vertexAttribute_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(VertexAttribute) * vVertex.size(), &vVertex.front(), GL_STATIC_DRAW);

	GLuint vp = glGetAttribLocation(ourShader.Program, "position");
	glEnableVertexAttribArray(vp);
	glVertexAttribPointer(vp, 3, GL_FLOAT, GL_FALSE, sizeof(VertexAttribute), (GLvoid*)(0));

	GLuint vc = glGetAttribLocation(ourShader.Program, "color");
	glEnableVertexAttribArray(vc);
	glVertexAttribPointer(vc, 3, GL_FLOAT, GL_FALSE, sizeof(VertexAttribute), (GLvoid*)(3 * sizeof(GLfloat)));

	GLuint EBO;
	glGenBuffers(1, &EBO);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size()*3*sizeof(GLuint), &indices.front(), GL_STATIC_DRAW);

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
		model = glm::scale(model, glm::vec3(50.0f, 50.0f, 1.0f));
		model = glm::rotate(model, 30.0f, glm::vec3(1.0f, 0.0f, 0.0f));
		view = glm::translate(view, glm::vec3(0.0f, 0.0f, -500.11f));
		projection = glm::ortho(0.0f, (GLfloat)screenWidth, 0.0f, (GLfloat)screenHeight, 0.1f, 1000.0f);

		GLint modelLoc = glGetUniformLocation(ourShader.Program, "model");
		GLint viewLoc = glGetUniformLocation(ourShader.Program, "view");
		GLint projLoc = glGetUniformLocation(ourShader.Program, "projection");
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));


		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		glfwSwapBuffers(window);
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

	glfwTerminate();

	return EXIT_SUCCESS;
}
