#ifndef VAO_CLASS_H
#define VAO_CLASS_H

#include<glad/glad.h>
#include"VBO.h"

class VAO
{
public:
	// ID reference for Vertex Array Object
	GLuint ID;
	// Constructor for Vertex Array Object
	VAO();

	// Link the VBO to the VAO using a layout
	void LinkVBO(VBO VBO, GLuint layout);
	// Bind the VAO
	void Bind();
	// Unbind the VAO
	void Unbind();
	// Delete the VAO
	void Delete();
};

#endif