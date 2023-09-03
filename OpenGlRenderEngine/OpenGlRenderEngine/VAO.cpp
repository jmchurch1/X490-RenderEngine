#include"VAO.h"

// Constructor for the VAO
VAO::VAO()
{
	glGenVertexArrays(1, &ID);
}

// Link a VBO to the VAO
void VAO::LinkAttrib(VBO& VBO, GLuint layout, GLuint numComponents, GLenum type, GLsizeiptr stride, void* offset)
{
	VBO.Bind();
	// pass in the vertex attribute that we want to use
	// inputs:
	// the position of the vertex attrib: int
	// values per vertex: int
	// what types of values: GL
	// whether or not input is integer: GL
	// stride of the vertices, data between each vertex: float
	// offset, pointer to where the vertices begin in the array: int
	glVertexAttribPointer(layout, numComponents, type, GL_FALSE, stride, offset);
	// enable the vertex attribute, 0 is the position of the vertex attrib
	glEnableVertexAttribArray(layout);

	VBO.Unbind();
}

// Bind the VAO
void VAO::Bind()
{
	glBindVertexArray(ID);
}

// Unbind the VAO
void VAO::Unbind()
{
	glBindVertexArray(0);
}


// Delete the VAO
void VAO::Delete()
{
	glDeleteVertexArrays(1, &ID);
}