#include <GL/glew.h>		
#include <GLFW/glfw3.h> 
#include <iostream>


using namespace std;


GLuint triangleVertexArrayObject;
GLuint triangleShaderProgramID;
GLuint trianglePositionVertexBufferObjectID, triangleColorVertexBufferObjectID;


bool initShaderProgram() {

	//#3
	const GLchar* vertexShaderSource =
		"#version 330 core\n"
		"in vec3 positionAttribute;"
		"in vec3 colorAttribute;"
		"out vec3 passColorAttribute;"
		"void main()"
		"{"
		"gl_Position = vec4(positionAttribute, 1.0);"
		"passColorAttribute = colorAttribute;"
		"}";


	//#4
	const GLchar* fragmentShaderSource =
		"#version 330 core\n"
		"in vec3 passColorAttribute;"
		"out vec4 fragmentColor;"
		"void main()"
		"{"
		"fragmentColor = vec4(passColorAttribute, 1.0);"
		"}";



	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	GLint result;
	GLchar errorLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, errorLog);
		cerr << "ERROR: failed to compile vertex shader\n" << errorLog << endl;
		glDeleteShader(vertexShader);
		return false;
	}


	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, errorLog);
		cerr << "ERROR: failed to compile fragment shader\n" << errorLog << endl;

		return false;
	}


	//#5
	triangleShaderProgramID = glCreateProgram();

	glAttachShader(triangleShaderProgramID, vertexShader);
	glAttachShader(triangleShaderProgramID, fragmentShader);

	glLinkProgram(triangleShaderProgramID);


	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);


	glGetProgramiv(triangleShaderProgramID, GL_LINK_STATUS, &result);
	if (!result) {
		glGetProgramInfoLog(triangleShaderProgramID, 512, NULL, errorLog);
		cerr << "ERROR: failed to connect shader program\n" << errorLog << endl;
		return false;
	}

	return true;
}



bool defineVertexArrayObject() {

	//#1
	//Vertex datum for triangles - position and color
	float sizeOfTriangle = 0.0f;
	float position[] = {
		0.0f, 0.0f, 0.0f, //vertex 1-1  center-top
		0.0f, 0.0f, 0.0f, //vertex 1-2  bottom-right
		0.0f, 0.0f, 0.0f, //vertex 1-3  bottom-left
		0.5f, 0.5f, 0.0f, //vertex 2-1  center-top
		0.5f, 0.5f, 0.0f, //vertex 2-2  bottom-right
		0.5f, 0.5f, 0.0f, //vertex 2-3  bottom-left
		-1.0f, -1.0f, 0.0f, //vertex 3-1  center-top
		-1.0f, -1.0f, 0.0f, //vertex 3-2  bottom-right
		-1.0f, -1.0f, 0.0f //vertex 3-3  bottom-left
	};
	for (size_t i = 0; i < 3; i++)
	{
		cout << "Insert the position of triangle #" << i+1 << ": ";
		cin >> position[9 * i+0] >> position[9 * i+1] >> position[9 * i+2];
		if (position[9 * i + 0] > 1 || position[9 * i + 0] < -1 ||
			position[9 * i + 1] > 1 || position[9 * i + 1] < -1 ||
			position[9 * i + 2] > 1 || position[9 * i + 2] < -1)
		{
			cout << "ERROR: Out of viewport, relocate the triangle!" << endl;
			--i;
			continue;
		}
		cout << "Insert the size of triangle #" << i+1 << ": ";
		cin >> sizeOfTriangle;
		position[9*i+3] = position[9*i+0] + sizeOfTriangle;
		position[9*i+6] = position[9*i+0] + sizeOfTriangle / 2.0f;
		position[9*i+7] = position[9*i+1] + sizeOfTriangle;
		
		if (position[9 * i + 3] > 1 || position[9 * i + 3] < -1 ||
			position[9 * i + 6] > 1 || position[9 * i + 6] < -1 || 
			position[9 * i + 7] > 1 || position[9 * i + 7] < -1)
		{
			cout << "ERROR: Out of viewport, resize the triangle!" << endl;
			--i;
		}
	}

	float color[] = {
		1.0f, 0.0f, 0.0f, //vertex 1-1 : RED (1,0,0)
		0.0f, 1.0f, 0.0f, //vertex 1-2 : GREEN (0,1,0) 
		0.0f, 0.0f, 1.0f, //vertex 1-3 : BLUE (0,0,1)
		1.0f, 0.0f, 0.0f, //vertex 2-1 : RED (1,0,0)
		0.0f, 1.0f, 0.0f, //vertex 2-2 : GREEN (0,1,0) 
		0.0f, 0.0f, 1.0f, //vertex 2-3 : BLUE (0,0,1)
		1.0f, 0.0f, 0.0f, //vertex 3-1 : RED (1,0,0)
		0.0f, 1.0f, 0.0f, //vertex 3-2 : GREEN (0,1,0) 
		0.0f, 0.0f, 1.0f  //vertex 3-3 : BLUE (0,0,1)
	};



	//#2
	//Copy vertex datum by creating Vertex Buffer Object(VBO)
	glGenBuffers(1, &trianglePositionVertexBufferObjectID);
	glBindBuffer(GL_ARRAY_BUFFER, trianglePositionVertexBufferObjectID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(position), position, GL_STATIC_DRAW);

	glGenBuffers(1, &triangleColorVertexBufferObjectID);
	glBindBuffer(GL_ARRAY_BUFFER, triangleColorVertexBufferObjectID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(color), color, GL_STATIC_DRAW);



	//#6
	glGenVertexArrays(1, &triangleVertexArrayObject);
	glBindVertexArray(triangleVertexArrayObject);


	GLint positionAttribute = glGetAttribLocation(triangleShaderProgramID, "positionAttribute");
	if (positionAttribute == -1) {
		cerr << "failed to get the properties of position" << endl;
		return false;
	}
	glBindBuffer(GL_ARRAY_BUFFER, trianglePositionVertexBufferObjectID);
	glVertexAttribPointer(positionAttribute, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(positionAttribute);

	GLint colorAttribute = glGetAttribLocation(triangleShaderProgramID, "colorAttribute");
	if (colorAttribute == -1) {
		cerr << "failed to get the properties of color" << endl;
		return false;
	}
	glBindBuffer(GL_ARRAY_BUFFER, triangleColorVertexBufferObjectID);
	glVertexAttribPointer(colorAttribute, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(colorAttribute);


	glBindVertexArray(0);


	return true;
}




void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
	//Those two from the first of glViewport parameter are the bottom left coordinates of viewport rectangle.
	//Next two are the width and the height of viewport rectangle.
	//Use framebuffer's width and height into glViewport's parameters.
	glViewport(0, 0, width, height);

}



void errorCallback(int errorCode, const char* errorDescription)
{
	cerr << "Error: " << errorDescription << endl;
}



void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GLFW_TRUE);
}




int main()
{

	glfwSetErrorCallback(errorCallback);


	if (!glfwInit()) {
		cerr << "Error: failed to initialize GLFW" << endl;
		std::exit(EXIT_FAILURE);
	}


	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	glfwWindowHint(GLFW_SAMPLES, 4);


	GLFWwindow* window = glfwCreateWindow(
		800,
		600,
		"OpenGL Example",
		NULL, NULL);
	if (!window) {

		glfwTerminate();
		std::exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(window);


	glfwSetKeyCallback(window, keyCallback);
	glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);


	glewExperimental = GL_TRUE;
	GLenum errorCode = glewInit();
	if (GLEW_OK != errorCode) {

		cerr << "Error: failed to initialize GLEW - " << glewGetErrorString(errorCode) << endl;

		glfwTerminate();
		std::exit(EXIT_FAILURE);
	}


	if (!GLEW_VERSION_3_3) {

		cerr << "Error: OpenGL 3.3 API is not available." << endl;

		glfwTerminate();
		std::exit(EXIT_FAILURE);
	}


	if (!initShaderProgram()) {

		cerr << "Error: failed to generate Shader Program" << endl;

		glfwTerminate();
		std::exit(EXIT_FAILURE);
	}


	if (!defineVertexArrayObject()) {

		cerr << "Error: failed to generate  Shader Program" << endl;

		glfwTerminate();
		std::exit(EXIT_FAILURE);
	}


	glfwSwapInterval(1);


	glUseProgram(triangleShaderProgramID);
	glBindVertexArray(triangleVertexArrayObject);


	while (!glfwWindowShouldClose(window)) {

		glClearColor(0, 0, 0, 1);
		glClear(GL_COLOR_BUFFER_BIT);

		glDrawArrays(GL_TRIANGLES, 0, 3*3);

		glfwSwapBuffers(window);
		glfwPollEvents();

	}


	glUseProgram(0);
	glBindVertexArray(0);


	glDeleteProgram(triangleShaderProgramID);
	glDeleteBuffers(1, &trianglePositionVertexBufferObjectID);
	glDeleteBuffers(1, &triangleColorVertexBufferObjectID);
	glDeleteVertexArrays(1, &triangleVertexArrayObject);
	glfwTerminate();

	std::exit(EXIT_SUCCESS);
}