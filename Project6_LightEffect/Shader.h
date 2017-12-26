#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <GL/glew.h>

typedef struct _vertexAttri
{
	GLfloat x, y, z;
	glm::vec3 normal;
}VertexAttribute;

class Shader
{
private:
	std::vector<VertexAttribute> vVertexT;

	template <class Container>
	void split(const std::string& str, Container& cont, char delim = '\n')
	{
		std::stringstream ss(str);
		std::string token;
		while (std::getline(ss, token, delim)) {
			cont.push_back(token);
		}
	}

	void CalculateNormal(std::vector<VertexAttribute>& _vVertex, std::vector<GLuint>& _vIndices)
	{
		std::vector<int> nbSeen;
		nbSeen.resize(_vVertex.size(), 0);
		for (int i = 0; i < _vIndices.size() - 2; i += 3) {
			GLuint ia = _vIndices[i];
			GLuint ib = _vIndices[i + 1];
			GLuint ic = _vIndices[i + 2];
			glm::vec3 normal = glm::normalize(glm::cross(
				glm::vec3(_vVertex[ic].x, _vVertex[ic].y, _vVertex[ic].z) - glm::vec3(_vVertex[ib].x, _vVertex[ib].y, _vVertex[ib].z),
				glm::vec3(_vVertex[ia].x, _vVertex[ia].y, _vVertex[ia].z) - glm::vec3(_vVertex[ib].x, _vVertex[ib].y, _vVertex[ib].z)));

			int v[3];  v[0] = ia;  v[1] = ib;  v[2] = ic;
			for (int j = 0; j < 3; j++) {
				GLushort cur_v = v[j];
				nbSeen[cur_v]++;
				if (nbSeen[cur_v] == 1) {
					_vVertex[cur_v].normal = normal;
				}
				else {
					// The Method of Phong Shading
					_vVertex[cur_v].normal.x = _vVertex[cur_v].normal.x * (1.0 - 1.0 / nbSeen[cur_v]) + normal.x * 1.0 / nbSeen[cur_v];
					_vVertex[cur_v].normal.y = _vVertex[cur_v].normal.y * (1.0 - 1.0 / nbSeen[cur_v]) + normal.y * 1.0 / nbSeen[cur_v];
					_vVertex[cur_v].normal.z = _vVertex[cur_v].normal.z * (1.0 - 1.0 / nbSeen[cur_v]) + normal.z * 1.0 / nbSeen[cur_v];
					_vVertex[cur_v].normal = glm::normalize(_vVertex[cur_v].normal);
				}
			}
		}
	}

public:
	std::vector<VertexAttribute> vFoilVertex, vHubVertex;
	std::vector<GLuint> vFoilIndices, vHubIndices;
	GLfloat vertices[216] =
	{
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
	};

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

		std::vector<std::string> vContainer;
		split(vertexCode, vContainer);

		size_t verticesIndex = 0;
		float firstX = 0.0f, firstY = 0.0f;
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
				vVertexT.reserve(std::stoi(coorSet[0]));
			}
			else
			{
				if (verticesIndex == 0)
				{
					firstX = std::stof(coorSet[0]);
					firstY = std::stof(coorSet[1]);
				}
				double x = std::stof(coorSet[0]) - firstX;
				double y = std::stof(coorSet[1]) - firstY;
				double z = 1.0f;

				coorSet.clear();

				if (verticesIndex++ < vVertexT.capacity())
				{
					VertexAttribute vVA = { x, y, z, glm::vec3(0.0f, 0.0f, 0.0f) };
					vVertexT.push_back(vVA);
				}
			}
		}

		return GL_TRUE;
	}

	void MakeFoil(const GLuint _FOILMAX)
	{
		// Make an airfoil with differentent Z coordinates.
		for each (VertexAttribute var in vVertexT)
		{
			vFoilVertex.push_back(var);
		}
		for (size_t foilNum = 1; foilNum <= _FOILMAX; foilNum++)
		{
			std::vector<VertexAttribute> vVertexAttributeTheOtherZ = vVertexT;
			for (VertexAttribute& var : vVertexAttributeTheOtherZ)
			{
				GLfloat factor = glm::log((GLfloat)foilNum + 2.5f);
				var.x = var.x * factor;
				var.y = var.y * factor;
				var.z = var.z * 2.5f * foilNum;
			}
			for each (VertexAttribute var in vVertexAttributeTheOtherZ)
			{
				vFoilVertex.push_back(var);
			}
			GLuint stride = vVertexAttributeTheOtherZ.size();
			for (size_t ii = vFoilVertex.size() - 1; ii > vFoilVertex.size() - stride; ii--)
			{
				vFoilIndices.push_back((GLuint)ii);
				vFoilIndices.push_back((GLuint)ii - 1);
				vFoilIndices.push_back((GLuint)ii - 1 - stride);

				vFoilIndices.push_back((GLuint)ii);
				vFoilIndices.push_back((GLuint)ii - 1 - stride);
				vFoilIndices.push_back((GLuint)ii - stride);
			}
			vVertexAttributeTheOtherZ.clear();
			vVertexAttributeTheOtherZ.shrink_to_fit();
		}

		CalculateNormal(vFoilVertex, vFoilIndices);
	}

	void MakeHub(const GLfloat _RADIUS)
	{
		for (size_t theta = 0; theta < 360; theta += 10)
		{
			GLfloat x = _RADIUS * glm::cos(theta * 3.14f / 180);
			GLfloat y = _RADIUS * glm::sin(theta * 3.14f / 180);
			VertexAttribute hub = { x, y, 0.0f, glm::vec3(0.0f, 0.0f, 0.0f) };
			vHubVertex.push_back(hub);
			hub = { x, y, 30.0f, glm::vec3(0.0f, 0.0f, 0.0f) };
			vHubVertex.push_back(hub);
		}
		for (size_t i = 0; i < vHubVertex.size() - 1; i += 2)
		{
			vHubIndices.push_back(i + 2);
			vHubIndices.push_back(i + 1);
			vHubIndices.push_back(i);

			vHubIndices.push_back(i + 3);
			vHubIndices.push_back(i + 1);
			vHubIndices.push_back(i + 2);

			if (i == vHubVertex.size() - 2)
			{
				vHubIndices.push_back(0);
				vHubIndices.push_back(i + 1);
				vHubIndices.push_back(i);

				vHubIndices.push_back(1);
				vHubIndices.push_back(i + 1);
				vHubIndices.push_back(0);

			}
		}

		CalculateNormal(vHubVertex, vHubIndices);
	}

	~Shader()
	{
		if (!vVertexT.empty())
		{
			vVertexT.clear();
		}
	}
};

#endif