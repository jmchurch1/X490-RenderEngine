#include"VBO.h"

// constructor for the VBO
VBO::VBO(std::vector<Vertex>& vertices)
{
	glGenBuffers(1, &ID);


	// binding makes an object the current object
	// GL_ARRAY_BUFFER is the type of buffer we are passing
	glBindBuffer(GL_ARRAY_BUFFER, ID);
	// store the vertices in the VBO
	// DRAW means that the vertices are used to draw vertices onto the screen
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);
}

// bind the VBO
void VBO::Bind()
{
	glBindBuffer(GL_ARRAY_BUFFER, ID);
}

// Unbind the VBO
void VBO::Unbind()
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

// Delete the VBO
void VBO::Delete()
{
	glDeleteBuffers(1, &ID);
}