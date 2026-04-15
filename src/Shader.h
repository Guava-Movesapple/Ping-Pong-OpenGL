#ifndef SHADER_H
#define SHADER_H

#include<glad/glad.h>
#include<string>
#include<fstream>
#include<sstream>
#include<iostream>
#include<cerrno>

std::string getFileContent(const char* file);

class Shader {

	public: Shader(const char* vertexFile, const char* fragFile);

	GLuint ID;

	public: void Activate();

	public: void Delete();

};

#endif SHADER_H
