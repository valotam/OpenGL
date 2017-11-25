#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <GL/glew.h>

typedef struct _coordi
{
	GLfloat x;
	GLfloat y;
}Coordinate;

class Shader
{
public:
	GLfloat * Vertices;
	GLuint Indices;
	std::vector<Coordinate> vCoordinates;

	GLuint Program;
	// Constructor generates the shader on the fly
	Shader(const GLchar *vertexPath, const GLchar *fragmentPath)
	{
		// 1. Retrieve the vertex/fragment source code from filePath
		std::string vertexCode;
		std::string fragmentCode;
		std::ifstream vShaderFile;
		std::ifstream fShaderFile;
		// ensures ifstream objects can throw exceptions:
		vShaderFile.exceptions(std::ifstream::badbit);
		fShaderFile.exceptions(std::ifstream::badbit);
		try
		{
			// Open files
			vShaderFile.open(vertexPath);
			fShaderFile.open(fragmentPath);
			std::stringstream vShaderStream, fShaderStream;
			// Read file's buffer contents into streams
			vShaderStream << vShaderFile.rdbuf();
			fShaderStream << fShaderFile.rdbuf();
			// close file handlers
			vShaderFile.close();
			fShaderFile.close();
			// Convert stream into string
			vertexCode = vShaderStream.str();
			fragmentCode = fShaderStream.str();
		}
		catch (std::ifstream::failure e)
		{
			std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
		}
		const GLchar *vShaderCode = vertexCode.c_str();
		const GLchar *fShaderCode = fragmentCode.c_str();
		// 2. Compile shaders
		GLuint vertex, fragment;
		GLint success;
		GLchar infoLog[512];
		// Vertex Shader
		vertex = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex, 1, &vShaderCode, NULL);
		glCompileShader(vertex);
		// Print compile errors if any
		glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(vertex, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
		}
		// Fragment Shader
		fragment = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment, 1, &fShaderCode, NULL);
		glCompileShader(fragment);
		// Print compile errors if any
		glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(fragment, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
		}
		// Shader Program
		this->Program = glCreateProgram();
		glAttachShader(this->Program, vertex);
		glAttachShader(this->Program, fragment);
		glLinkProgram(this->Program);
		// Print linking errors if any
		glGetProgramiv(this->Program, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(this->Program, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
		}
		// Delete the shaders as they're linked into our program now and no longer necessery
		glDeleteShader(vertex);
		glDeleteShader(fragment);

	}
	// Uses the current shader
	void Use()
	{
		glUseProgram(this->Program);
	}

	bool LoadOutFile(const char * _filePath)
	{
		// 1. Retrieve the vertex/fragment source code from filePath
		std::string vertexCode;
		std::ifstream vShaderFile;
		// ensures ifstream objects can throw exceptions:
		vShaderFile.exceptions(std::ifstream::badbit);
		try
		{
			// Open files
			vShaderFile.open(_filePath);
			std::stringstream vShaderStream;
			// Read file's buffer contents into streams
			vShaderStream << vShaderFile.rdbuf();
			// close file handlers
			vShaderFile.close();
			// Convert stream into string
			vertexCode = vShaderStream.str();
		}
		catch (std::ifstream::failure e)
		{
			std::cout << "ERROR::SHADER::.out_FILE_NOT_SUCCESFULLY_READ" << std::endl;
			return GL_FALSE;
		}
		const GLchar *vShaderCode = vertexCode.c_str();

		std::vector<std::string> vContainer;
		split(vertexCode, vContainer);

		size_t verticesIndex = 0;
		for each (std::string var in vContainer)
		{
			std::vector<std::string> coorSet;
			split(var, coorSet, '\t');

			if (coorSet.size() == 1)
			{
				if (verticesIndex != 0)
				{
					std::cout << "ERROR::SHADER::VERTEX_SIZE_CHANGED" << std::endl;
					return GL_FALSE;
				}
				Vertices = new GLfloat[std::stoi(coorSet[0]) * 3];
			}
			else
			{
				//if (verticesIndex == 0)
				//{
				//	std::cout << "ERROR::SHADER::MISSED_VERTEX_COORDINATE" << std::endl;
				//	continue;
				//}

				double x = std::stof(coorSet[0]);	// string to double
				double y = std::stof(coorSet[1]);

				coorSet.clear();

				if (Vertices != NULL)
				{
					Vertices[verticesIndex++] = x;
					Vertices[verticesIndex++] = y;
					Vertices[verticesIndex++] = 1.0f;
				}
			}
		}

		return GL_TRUE;
	}

	template <class Container>
	void split(const std::string& str, Container& cont, char delim = '\n')
	{
		std::stringstream ss(str);
		std::string token;
		while (std::getline(ss, token, delim)) {
			cont.push_back(token);
		}
	}

	~Shader() { 
		if (Vertices)
		{
			delete[] Vertices;
			std::cout << "DONE!" << std::endl;
		}

	}
};

#endif