#ifndef FRAMEBUFFER_CLASS_H
#define FRAMEBUFFER_CLASS_H

#include<glad/glad.h>
#include<vector>


class Framebuffer
{
public:
	// ID reference of Elements Buffer Object
	GLuint ID;
	GLuint rbo;
	GLuint textureColorBuffer;
	// Constructor that generates an Elements Buffer Objects and links it to indices
	Framebuffer(float width, float height);

	// Bind the EBO
	void Bind();
	// Unbind the EBO
	void Unbind();
	// Delete the EBO
	void Delete();
};

#endif