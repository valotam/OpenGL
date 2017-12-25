#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Shader.h"

const GLuint WIDTH = 800, HEIGHT = 600;
const GLuint FOILMAX = 10;

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
	glDisable(GL_CULL_FACE);
	glFrontFace(GL_CCW);

	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);


	Shader ourShader("core.vertexshader", "core.fragmentshader");
	ourShader.LoadOutFile("foil_spline.out");

	// Make an airfoil with differentent Z coordinates.
	std::vector<VertexAttribute> vVertex;
	for each (VertexAttribute var in ourShader.vVertexAttribute)
	{
		vVertex.push_back(var);
	}
	std::vector<GLuint> indices;
	for (size_t foilNum = 1; foilNum <= FOILMAX; foilNum++)
	{
		std::vector<VertexAttribute> vVertexAttributeTheOtherZ = ourShader.vVertexAttribute;
		for (VertexAttribute& var : vVertexAttributeTheOtherZ)
		{
			var.x = var.x * (1 + (GLfloat)foilNum / FOILMAX);
			var.y = var.y * (1 + (GLfloat)foilNum / FOILMAX);
			var.z = var.z * (1 + (GLfloat)foilNum / FOILMAX);
			// Its color attribute
			
			var.r = 1.0f - (GLfloat)foilNum / FOILMAX;
			var.g = 0.9f - (GLfloat)foilNum / (2 * FOILMAX);
			var.b = 0.5f - (GLfloat)foilNum / (2 * FOILMAX);
		}
		for each (VertexAttribute var in vVertexAttributeTheOtherZ)
		{
			vVertex.push_back(var);
		}
		for (size_t ii = vVertex.size() - 1; ii > vVertex.size() - vVertexAttributeTheOtherZ.size(); ii--)
		{
			indices.push_back((GLuint)ii);
			indices.push_back((GLuint)ii - 1);
			indices.push_back((GLuint)ii - 1 - vVertexAttributeTheOtherZ.size());

			indices.push_back((GLuint)ii);
			indices.push_back((GLuint)ii - 1 - vVertexAttributeTheOtherZ.size());
			indices.push_back((GLuint)ii - vVertexAttributeTheOtherZ.size());
		}
		vVertexAttributeTheOtherZ.clear();
		vVertexAttributeTheOtherZ.shrink_to_fit();
	}

	GLint modelLoc = glGetUniformLocation(ourShader.Program, "model");
	GLint viewLoc = glGetUniformLocation(ourShader.Program, "view");
	GLint projLoc = glGetUniformLocation(ourShader.Program, "projection");

	GLuint VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	GLuint VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(VertexAttribute) * vVertex.size(), &vVertex.front(), GL_STATIC_DRAW);
	GLuint EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices.front(), GL_STATIC_DRAW);

	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glUseProgram(ourShader.Program);

		glm::mat4 model;
		glm::mat4 view;
		glm::mat4 projection;
		model = glm::scale(model, glm::vec3(10.0f, 10.0f, 1.0f));
		model = glm::rotate(model, 0.0f + (GLfloat)glfwGetTime() * 1.0f, glm::vec3(0.0f, 0.0f, 1.0f));
		view = glm::translate(view, glm::vec3(0.0f, 0.0f, -500.11f));
		projection = glm::perspective(45.0f, (GLfloat)screenWidth / (GLfloat)screenHeight, 0.01f, 1000.0f);
		//view = glm::translate(view, glm::vec3((GLfloat)screenWidth / 2, (GLfloat)screenHeight / 2, -500.11f));
		//projection = glm::ortho(0.0f, (GLfloat)screenWidth, 0.0f, (GLfloat)screenHeight, 0.1f, 1000.0f);

		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

		GLuint vp = glGetAttribLocation(ourShader.Program, "position");
		glEnableVertexAttribArray(vp);
		glVertexAttribPointer(vp, 3, GL_FLOAT, GL_FALSE, sizeof(VertexAttribute), (GLvoid*)(0));

		GLuint vc = glGetAttribLocation(ourShader.Program, "color");
		glEnableVertexAttribArray(vc);
		glVertexAttribPointer(vc, 3, GL_FLOAT, GL_FALSE, sizeof(VertexAttribute), (GLvoid*)(3 * sizeof(GLfloat)));

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

		model = glm::rotate(model, 2.10f, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model)); 
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

		model = glm::rotate(model, 2.10f, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);


		glDisableVertexAttribArray(vp);
		glDisableVertexAttribArray(vc);

		glfwSwapBuffers(window);
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteProgram(ourShader.Program);

	glfwTerminate();

	return EXIT_SUCCESS;
}
