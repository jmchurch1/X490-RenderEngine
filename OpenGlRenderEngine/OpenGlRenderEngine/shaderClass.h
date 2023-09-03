#ifndef SHADER_CLASS_H
#define SHADER_CLASS_H

//include statements
#include<glad/glad.h>
#include<string>
#include<fstream>
#include<sstream>
#include<iostream>
#include<cerrno>

std::string get_file_contents(const char* filename);

class Shader
{
public:
	// reference ID of the Shader Program
	GLuint ID;
	// Constructor to build Shader Program from vertex and fragment shaders
	Shader(const char* vertexFile, const char* fragmentFile);

	// Activate the Shader Program
	void Activate();
	// Delete the Shader Program
	void Delete();
};


#endif