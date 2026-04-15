#include "Shader.h"

std::string getFileContent(const char* file) {

	std::ifstream in(file, std::ios::binary);

	std::string content;

	if (in) {
		in.seekg(0, std::ios::end);
		content.resize(in.tellg());
		in.seekg(0, std::ios::beg);
		in.read(&content[0], content.size());
		in.close();
		return content;
	}

	throw(errno);
}

Shader::Shader(const char* vertexFile, const char* fragFile) {

	int success;
	char infoLog[512];


	std::string vertexCode = getFileContent(vertexFile);
	std::string fragCode = getFileContent(fragFile);
	const char* vertexSource = vertexCode.c_str();
	const char* fragSource = fragCode.c_str();

	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexSource, NULL);
	glCompileShader(vertexShader);
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	};

	GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragShader, 1, &fragSource, NULL);
	glCompileShader(fragShader);
	glGetShaderiv(fragShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	};

	ID = glCreateProgram();
	glAttachShader(ID, vertexShader);
	glAttachShader(ID, fragShader);

	glLinkProgram(ID);

	glGetProgramiv(ID, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(ID, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}

	glDeleteProgram(vertexShader);
	glDeleteProgram(fragShader);

}

void Shader::Activate() {
	glUseProgram(ID);
}

void Shader::Delete() {
	glDeleteProgram(ID);
}