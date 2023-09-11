#ifndef EBO_CLASS_H
#define EBO_CLASS_H

#include<glad/glad.h>
#include<vector>

class EBO
{
public:
	// ID reference of Elements Buffer Object
	GLuint ID;
	// Constructor that generates an Elements Buffer Objects and links it to indices
	EBO(std::vector<GLuint> indices);

	// Bind the EBO
	void Bind();
	// Unbind the EBO
	void Unbind();
	// Delete the EBO
	void Delete();
};

#endif