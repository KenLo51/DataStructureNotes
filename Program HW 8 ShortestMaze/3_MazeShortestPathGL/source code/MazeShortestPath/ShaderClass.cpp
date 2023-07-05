#include "ShaderClass.h"

// read all contents from file
std::string Shader::get_file_contents(const char* filename) {
	std::ifstream ifs(filename, std::ifstream::in);
	if (!ifs.good()) {
		std::cout << "failed to opening \""<< filename << '\"' << std::endl;
		exit(1);
	}

	std::string fileContents;
	// getting file size
	ifs.seekg(0, std::ios_base::end);
	fileContents.resize(ifs.tellg());
	// reading file content
	ifs.seekg(0, std::ios_base::beg);
	ifs.read(&fileContents[0], fileContents.size());
	// close file
	ifs.close();

	return fileContents;
}

// Shader destructor
Shader::Shader(){
	Shader::ID = 0;
	Shader::vertexShader_ID = 0;
	Shader::tessContShader_ID = 0;
	Shader::tessEvalShader_ID = 0;
	Shader::geometryShader_ID = 0;
	Shader::fragmentShader_ID = 0;
	Shader::computeShader_ID = 0;
}
Shader::Shader(const char* vertexShaderFile, const char* fragmentShaderFile) {
	Shader::ID = glCreateProgram();

	//read shader file
	setVertexShaderSource(vertexShaderFile);
	setFragmentShaderSource(fragmentShaderFile);

	glLinkProgram(Shader::ID);

	// check if link success
	GLint success;
	glGetProgramiv(Shader::ID, GL_LINK_STATUS, &success);
	if (!success) {
		char infoLog[512];
		glGetProgramInfoLog(Shader::ID, sizeof(infoLog), NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
		exit(1);
	}
}
Shader::Shader(const char* vertexFile, const char* tessellationControlFile, const char* tessellationEvaluationFile, const char* geometryFile, const char* fragmentFile) {
	Shader::ID = glCreateProgram();

	//read shader file
	setShaderSource(vertexFile, GL_VERTEX_SHADER);
	if(tessellationControlFile != NULL)
		setShaderSource(tessellationControlFile, GL_TESS_CONTROL_SHADER);
	if (tessellationEvaluationFile != NULL)
		setShaderSource(tessellationEvaluationFile, GL_TESS_EVALUATION_SHADER);
	if (geometryFile != NULL)
		setShaderSource(geometryFile, GL_GEOMETRY_SHADER);
	setShaderSource(fragmentFile, GL_FRAGMENT_SHADER);

	glLinkProgram(Shader::ID);

	// check if link success
	GLint success;
	glGetProgramiv(Shader::ID, GL_LINK_STATUS, &success);
	if (!success) {
		char infoLog[512];
		glGetProgramInfoLog(Shader::ID, sizeof(infoLog), NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
		exit(1);
	}
}



// GL_COMPUTE_SHADER, GL_VERTEX_SHADER, GL_TESS_CONTROL_SHADER, GL_TESS_EVALUATION_SHADER, GL_GEOMETRY_SHADER, or GL_FRAGMENT_SHADER
void Shader::setShaderSource(const char* shaderFile, GLenum shaderType) {
	if (shaderFile == NULL)
		return;

	// check shaderType
	GLuint* shader_ID = NULL;
	if (shaderType == GL_COMPUTE_SHADER) shader_ID = &computeShader_ID;
	else if(shaderType == GL_VERTEX_SHADER) shader_ID = &vertexShader_ID;
	else if (shaderType == GL_TESS_CONTROL_SHADER) shader_ID = &tessContShader_ID;
	else if (shaderType == GL_TESS_EVALUATION_SHADER) shader_ID = &tessEvalShader_ID;
	else if (shaderType == GL_GEOMETRY_SHADER) shader_ID = &geometryShader_ID;
	else if (shaderType == GL_FRAGMENT_SHADER) shader_ID = &fragmentShader_ID;

	if(shader_ID == NULL) {
		std::cout << "Invalid shader type\n";
		exit(1);
	}

	// if hader has been loaded(compiled)
	if (Shader::loadedShaders.count(shaderFile) > 0) {
		Shader::vertexShader_ID = Shader::loadedShaders[shaderFile];
	}
	else { 
		// load and compile new shader
		// read shader file content
		std::string vertexShaderSource = get_file_contents(shaderFile);
		// create vertex shader
		*shader_ID = glCreateShader(shaderType);

		// put shader code to opengl
		const char* vertexShaderSourcePtr = vertexShaderSource.c_str();
		glShaderSource(*shader_ID, 1, &vertexShaderSourcePtr, NULL);

		// compile Vertex Shader
		glCompileShader(*shader_ID);

		// check if compile success
		GLint success;
		glGetShaderiv(*shader_ID, GL_COMPILE_STATUS, &success);
		if (!success) {
			char infoLog[512];
			glGetShaderInfoLog(*shader_ID, sizeof(infoLog), NULL, infoLog);
			std::cout << "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
			exit(1);
		}

		// add to cache
		Shader::loadedShaders.insert(std::pair<std::string, GLuint>(std::string(shaderFile), *shader_ID));
	}

	// add vertex shader to curr shader program
	glAttachShader(Shader::ID, *shader_ID);
}

// read and complile VertexShader file
void Shader::setVertexShaderSource(const char* vertexShaderFile) {
	setShaderSource(vertexShaderFile, GL_VERTEX_SHADER);
}

// read and complile FragmentShader file
void Shader::setFragmentShaderSource(const char* fragmentShaderFile) {
	setShaderSource(fragmentShaderFile, GL_FRAGMENT_SHADER);
}

// update a pair of new shader
void Shader::setShadersSource(const char* vertexShaderFile, const char* fragmentShaderFile) {
	glDetachShader(Shader::ID, Shader::fragmentShader_ID);
	glDetachShader(Shader::ID, Shader::vertexShader_ID);
	setVertexShaderSource(vertexShaderFile);
	setFragmentShaderSource(fragmentShaderFile);
	glLinkProgram(Shader::ID);

	// check if link success
	GLint success;
	glGetProgramiv(Shader::ID, GL_LINK_STATUS, &success);
	if (!success) {
		char infoLog[512];
		glGetProgramInfoLog(Shader::ID, sizeof(infoLog), NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
		exit(1);
	}
}

// delete all loaded shader
void Shader::clearShaderCache() {
	for (const std::pair<std::string, GLuint>& vertexShader : Shader::loadedShaders) {
		glDeleteShader(vertexShader.second);
	}
	Shader::loadedShaders.clear();

	Shader::vertexShader_ID = 0;
	Shader::fragmentShader_ID = 0;
}

// delete a loaded shader
void Shader::removeShader(const char* fileName) {
	if (Shader::computeShader_ID == Shader::loadedShaders[fileName]) Shader::computeShader_ID = 0;
	else if (Shader::vertexShader_ID == Shader::loadedShaders[fileName]) Shader::vertexShader_ID = 0;
	else if (Shader::tessContShader_ID == Shader::loadedShaders[fileName]) Shader::tessContShader_ID = 0;
	else if (Shader::tessEvalShader_ID == Shader::loadedShaders[fileName]) Shader::tessEvalShader_ID = 0;
	else if (Shader::geometryShader_ID == Shader::loadedShaders[fileName]) Shader::geometryShader_ID = 0;
	else if (Shader::fragmentShader_ID == Shader::loadedShaders[fileName]) Shader::fragmentShader_ID = 0;
	glDeleteShader(Shader::loadedShaders[fileName]);
	Shader::loadedShaders.erase(fileName);
}

// utility uniform functions
void Shader::setUniform(const char* name, bool const value) {
	glUniform1i(glGetUniformLocation(Shader::ID, name), int(value));
}
void Shader::setUniform(const char* name, int const value) {
	glUniform1i(glGetUniformLocation(Shader::ID, name), value);
}
void Shader::setUniform(const char* name, unsigned int const value) {
	glUniform1ui(glGetUniformLocation(Shader::ID, name), value);
}
void Shader::setUniform(const char* name, float const value) {
	glUniform1f(glGetUniformLocation(Shader::ID, name), value);
}
void Shader::setUniform(const char* name, glm::vec2& value) {
	glUniform2fv(glGetUniformLocation(Shader::ID, name), 1, glm::value_ptr(value));
}
void Shader::setUniform(const char* name, glm::ivec2& value) {
	glUniform2iv(glGetUniformLocation(Shader::ID, name), 1, glm::value_ptr(value));
}
void Shader::setUniform(const char* name, glm::vec3& value) {
	glUniform3fv(glGetUniformLocation(Shader::ID, name),1, glm::value_ptr(value));
}
void Shader::setUniform(const char* name, glm::vec4& value) {
	glUniform4fv(glGetUniformLocation(Shader::ID, name), 1, glm::value_ptr(value));
}
void Shader::setUniform(const char* name, glm::mat3& value) {
	glUniformMatrix3fv(glGetUniformLocation(Shader::ID, name), 1, GL_FALSE, glm::value_ptr(value));
}
void Shader::setUniform(const char* name, glm::mat4& value) {
	glUniformMatrix4fv(glGetUniformLocation(Shader::ID, name), 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::setUniform(const GLint location, bool const value) {
	glUniform1i(location, int(value));
}
void Shader::setUniform(const GLint location, int const value) {
	glUniform1i(location, value);
}
void Shader::setUniform(const GLint location, unsigned int const value) {
	glUniform1ui(location, value);
}
void Shader::setUniform(const GLint location, float const value) {
	glUniform1f(location, value);
}
void Shader::setUniform(const GLint location, glm::vec2& value) {
	glUniform2fv(location, 1, glm::value_ptr(value));
}
void Shader::setUniform(const GLint location, glm::vec3& value) {
	glUniform3fv(location, 1, glm::value_ptr(value));
}
void Shader::setUniform(const GLint location, glm::vec4& value) {
	glUniform4fv(location, 1, glm::value_ptr(value));
}
void Shader::setUniform(const GLint location, glm::mat3& value) {
	glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(value));
}
void Shader::setUniform(const GLint location, glm::mat4& value) {
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
}

// use shader
void Shader::Activate() {
	glUseProgram(Shader::ID);
}

// delete shader
void Shader::Delete() {
	clearShaderCache();
	glDeleteProgram(Shader::ID);
	Shader::ID = 0;
}