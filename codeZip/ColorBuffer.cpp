#include <iostream>

// GLEW
//#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Other includes
#include "Shader.h"

// Window dimensions
const GLuint WIDTH = 800, HEIGHT = 600;

// The MAIN function, from here we start the application and run the game loop
int main()
{
	// Init GLFW
	glfwInit();
	glEnable(GL_DEPTH_TEST);

	// Set all the required options for GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	// Create a GLFWwindow object that we can use for GLFW's functions
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

	// Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
	glewExperimental = GL_TRUE;
	// Initialize GLEW to setup the OpenGL Function pointers
	if (GLEW_OK != glewInit())
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return EXIT_FAILURE;
	}

	// Define the viewport dimensions
	glViewport(0, 0, screenWidth, screenHeight);

	// Build and compile our shader program
	Shader ourShader("core_mat.vertexshader", "core_mat.fragmentshader");	
	//ourShader.LoadOutFile("foil_spline.out");

	// Set up vertex data (and buffer(s)) and attribute pointers
	GLfloat vertices[] =
	{
		// Positions      
		0.5f,  0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		-0.5f, -0.5f, 0.0f,
		-0.5f,  0.5f, 0.0f
	};
	GLfloat colors[] =
	{
		1.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 1.0f,
		1.0f, 1.0f, 1.0f
	};
	GLuint indices[] =
	{  // Note that we start from 0!
		0, 1, 3, // First Triangle
		1, 2, 3  // Second Triangle
	};


	GLuint VAO; 	                    // this will hold the "name" of the vertex array
	glGenVertexArrays(1, &VAO); 	    // only need one vertex array, where to put its name

	glBindVertexArray(VAO);             // actually allocate that array

	GLuint VBO[2];                      // this will hold the "names" of the buffers on the GPU
	glGenBuffers(2, VBO);               // get two unused buffer names

	GLuint position_buffer = VBO[0];	// just a mnemonic
	GLuint color_buffer = VBO[1];
	glBindBuffer(GL_ARRAY_BUFFER, position_buffer); // say what we're using it for
	glBufferData(GL_ARRAY_BUFFER, 	// which buffer are we filling?
		sizeof(vertices),	        // how many bytes of data to send to GPU?
		vertices,		            // where are they?
		GL_STATIC_DRAW);	        // tell GPU that we're not planning to change
							        // the data at run-time, and the
							        // information will go one-way from
							        // application to GPU
							        // (which affects how it stores the data)
							        // Agree on what GLSL attribute(s) should receive the data we're going to
							        // send with glDrawElements()

	GLuint vp = glGetAttribLocation(ourShader.Program, "position");
	glEnableVertexAttribArray(vp);	// use this info in glDrawElements() et al.
									// Next, tell the vertex array object that values of the "position"
									// variable should come from the current ARRAY_BUFFER, and how this
									// array is organized.  Note that "current" means "whichever buffer
									// was most recently bound to GL_ARRAY_BUFFER as of the time
									// glVertexAttribPointer is called."
	glVertexAttribPointer(vp,		// which shared variable are we using?
		3,		                    // dimensions in "position"
		GL_FLOAT,	                // element type in "position"
		GL_FALSE,	                // don't normalize to [-1,1] range
		0,		                    // distance between elements of "position"
		(GLvoid*)(0));              // how far to start from beginning of the current
	                                // ARRAY_BUFFER, which is position_buffer.


	glBindBuffer(GL_ARRAY_BUFFER, color_buffer); // let's do it again with the color buffer
	glBufferData(GL_ARRAY_BUFFER,
		sizeof(colors),
		colors,
		GL_STATIC_DRAW);
	GLuint vc = glGetAttribLocation(ourShader.Program, "color");
	glEnableVertexAttribArray(vc); // pay attention to color info too
	glVertexAttribPointer(vc,
		3,		                   // dimensions
		GL_FLOAT,	               // element type
		GL_FALSE,	               // don't normalize
		0,		                   // stride
		(GLvoid*)(0));

	GLuint EBO;
	glGenBuffers(1, &EBO);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glBindVertexArray(0); // Unbind VAO

	// loop
	while (!glfwWindowShouldClose(window))
	{
		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();

		// Render
		// Clear the colorbuffer
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// Activate Face culling, Front face direction is counter clock-wise(CCW).
		glDisable(GL_CULL_FACE);
		glFrontFace(GL_CCW);

		// Set the mode of polygon, Front and back faces are drawn as lines.
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		// Draw the triangle
		glUseProgram(ourShader.Program);

		// Create transformations
		glm::mat4 transform;
		transform = glm::translate(transform, glm::vec3(0.5f, -0.5f, 0.0f));
		transform = glm::rotate(transform, (GLfloat)glfwGetTime() * -5.0f, glm::vec3(0.0f, 0.0f, 1.0f));

		// Get matrix's uniform location and set matrix
		GLint transformLocation = glGetUniformLocation(ourShader.Program, "transform");
		glUniformMatrix4fv(transformLocation, 1, GL_FALSE, glm::value_ptr(transform));


		// Draw container
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		// Swap the screen buffers
		glfwSwapBuffers(window);
	}

	// Properly de-allocate all resources once they've outlived their purpose
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(2, VBO);
	glDeleteBuffers(1, &EBO);

	// Terminate GLFW, clearing any resources allocated by GLFW.
	glfwTerminate();

	return EXIT_SUCCESS;
}




