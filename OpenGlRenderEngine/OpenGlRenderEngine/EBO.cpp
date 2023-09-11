#include"EBO.h"

EBO::EBO(std::vector<GLuint> indices)
{
	// generate buffers
	glGenBuffers(1, &ID);

	// binding makes an object the current object
	// GL_ARRAY_BUFFER is the type of buffer we are passing
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
	// store the vertices in the VBO
	// DRAW means that the vertices are used to draw vertices onto the screen
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);
}

// Bind the EBO
void EBO::Bind()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
}

// Unbind the EBO
void EBO::Unbind()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

// Delete the EBO
void EBO::Delete()
{
	glDeleteBuffers(1, &ID);
}