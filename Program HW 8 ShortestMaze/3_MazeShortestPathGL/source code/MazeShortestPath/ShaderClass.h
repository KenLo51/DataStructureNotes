#ifndef SHADER_CLASS_H
#define SHADER_CLASS_H

#include <stdlib.h>
#include <string>
#include <fstream>
#include <iostream>
#include <map>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader {
public:
private:
	// read all contents from file
	std::string get_file_contents(const char* filename);
private:
	// store loaded vertexShaders
	std::map <std::string, GLuint> loadedShaders;
	// current used shader IDs
	GLuint vertexShader_ID, fragmentShader_ID, geometryShader_ID, tessContShader_ID, tessEvalShader_ID, computeShader_ID;

public:
	// program shader ID
	GLuint ID;
	// Shader constructor
	Shader();
	Shader(const char* vertexFile, const char* fragmentFile);
	Shader(const char* vertexFile, const char* tessellationControlFile, const char* tessellationEvaluationFile, const char* geometryFile, const char* fragmentFile);

	void setShaderSource(const char* vertexShaderFile, GLenum shaderType);
	// read and complile VertexShader file
	void setVertexShaderSource(const char* vertexShaderFile);
	// read and complile FragmentShader file
	void setFragmentShaderSource(const char* fragmentShaderFile);
	// update a pair of new shader
	void setShadersSource(const char* vertexShaderFile, const char* fragmentShaderFile);
	// delete all loaded shader
	void clearShaderCache();
	// delete a loaded shader
	void removeShader(const char* fileName);
	// utility uniform functions
	void setUniform(const char* name, bool const value);
	void setUniform(const char* name, int const value);
	void setUniform(const char* name, unsigned int const value);
	void setUniform(const char* name, float const value);
	void setUniform(const char* name, glm::vec2& value);
	void setUniform(const char* name, glm::ivec2& value);
	void setUniform(const char* name, glm::vec3& value);
	void setUniform(const char* name, glm::vec4& value);
	void setUniform(const char* name, glm::mat3& value);
	void setUniform(const char* name, glm::mat4& value);

	void setUniform(const GLint location, bool const value);
	void setUniform(const GLint location, int const value);
	void setUniform(const GLint location, unsigned int const value);
	void setUniform(const GLint location, float const value);
	void setUniform(const GLint location, glm::vec2& value);
	void setUniform(const GLint location, glm::vec3& value);
	void setUniform(const GLint location, glm::vec4& value);
	void setUniform(const GLint location, glm::mat3& value);
	void setUniform(const GLint location, glm::mat4& value);

	// use shader
	void Activate();
	// delete shader
	void Delete();
};

#endif