#include"shaderClass.h"

std::string get_file_contents(const char* filename)
{
	std::ifstream in(filename, std::ios::binary);
	if (in)
	{
		std::string contents;
		in.seekg(0, std::ios::end);
		contents.resize(in.tellg());
		in.seekg(0, std::ios::beg);
		in.read(&contents[0], contents.size());
		in.close();
		return(contents);
	}
	throw(errno);
}

// Constructor for the shader taking in a vertex and fragment source code
Shader::Shader(const char* vertexFile, const char* fragmentFile)
{
	// read vertex and fragment source
	std::string vertexCode = get_file_contents(vertexFile);
	std::string fragmentCode = get_file_contents(fragmentFile);

	// convert shader source into characters
	const char* vertexSource = vertexCode.c_str();
	const char* fragmentSource = fragmentCode.c_str();

	// OpenGl version of an unsigned int, positive integer
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	// assign the source code of the vertex shader to the variable we created
	glShaderSource(vertexShader, 1, &vertexSource, NULL);
	// compile the source code for the vertex shader into machine code since
	// OpenGL can't understand the uncompiled source code
	glCompileShader(vertexShader);
	compileErrors(vertexShader, "VERTEX");

	// same as for vertex shader
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
	glCompileShader(fragmentShader);
	compileErrors(fragmentShader, "FRAGMENT");
	ID = glCreateProgram();

	// attach the two shaders that we just created to the program
	glAttachShader(ID, vertexShader);
	glAttachShader(ID, fragmentShader);
	// wrap up the shader program
	glLinkProgram(ID);
	compileErrors(ID, "PROGRAM");

	// delete the two shaders that were created since they are in the program
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

// Activate the shader
void Shader::Activate() {
	glUseProgram(ID);
}

// Delete the shader
void Shader::Delete() {
	glDeleteProgram(ID);
}

void Shader::compileErrors(unsigned int shader, const char* type)
{
	GLint hasCompiled;
	char infoLog[1024];
	if (type != "PROGRAM")

	{
		glGetShaderiv(shader, GL_COMPILE_STATUS, &hasCompiled);
		if (hasCompiled == GL_FALSE)
		{
			glGetShaderInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "SHADER_COMPILATION_ERROR for: " << type << "\n" << std::endl;
		}
	}
	else
	{
		glGetProgramiv(shader, GL_COMPILE_STATUS, &hasCompiled);
		if (hasCompiled == GL_FALSE)
		{
			glGetProgramInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "SHADER_LINKING_ERROR for: " << type << "\n" << std::endl;
		}
	}
}