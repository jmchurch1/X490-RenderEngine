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

void VAO::ScreenQuad()
{
	float vertices[] =
	{
		-1.0f, 1.0f, 0.0f, 1.0f,
		-1.0f, -1.0f, 0.0f, 0.0f,
		1.0f, -1.0f, 1.0f, 0.0f,

		-1.0f, 1.0f, 0.0f, 1.0f,
		1.0f, -1.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 1.0f, 1.0f,

	};

	glGenVertexArrays(1, &quadVAO);
	glGenBuffers(1, &quadVBO);
	glBindVertexArray(quadVAO);
	glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

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