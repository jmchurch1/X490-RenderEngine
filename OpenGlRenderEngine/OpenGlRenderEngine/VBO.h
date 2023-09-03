#ifndef VBO_CLASS_H
#define VBO_CLASS_H

#include<glad/glad.h>

class VBO
{
	public:
		// Reference ID of the Vertex Buffer Object
		GLuint ID;
		// Constructor that generates a Vertex Buffer Object
		VBO(GLfloat* vertices, GLsizeiptr size);

		// Bind the Vertex Buffer Object
		void Bind();
		// Unbind the Vertex Buffer Object
		void Unbind();
		// Delete the Vertex Buffer Object
		void Delete();
};

#endif