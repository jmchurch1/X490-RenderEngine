#ifndef VBO_CLASS_H
#define VBO_CLASS_H

#include<glm/glm.hpp>
#include<glad/glad.h>
#include<vector>

struct Vertex
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec3 color;
	glm::vec2 texUV;
};

class VBO
{
	public:
		// Reference ID of the Vertex Buffer Object
		GLuint ID;
		// Constructor that generates a Vertex Buffer Object
		VBO(std::vector<Vertex>& vertices);

		// Bind the Vertex Buffer Object
		void Bind();
		// Unbind the Vertex Buffer Object
		void Unbind();
		// Delete the Vertex Buffer Object
		void Delete();
};

#endif