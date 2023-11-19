#ifndef POINT_CLOUD_CLASS_H
#define POINT_CLOUD_CLASS_H

#include<string>

#include"VAO.h"
#include"EBO.h"
#include"Camera.h"
#include"Texture.h"

class PointCloud
{
public:
	std::vector <Vertex> vertices;
	std::vector <Texture> textures;

	VAO VAO;

	PointCloud(std::vector <Vertex>& vertices, std::vector <Texture>& textures);

	void Draw
	(
		Shader& shader,
		Camera& camera,
		glm::mat4 matrix = glm::mat4(1.0f),
		glm::vec3 translation = glm::vec3(0.0f, 0.0f, 0.0f)
	);
};



#endif