#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>

#include"shaderClass.h"
#include"VAO.h"
#include"VBO.h"
#include"EBO.h"

const unsigned int width = 800;
const unsigned int height = 800;


int main()
{
	glfwInit();

	// OpenGL doesn't know what version it is, so we are
	// giving it a hint to what version it is
	// we are using OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// use GL float since it is safer to use the same floats as OpenGL
	// create a list of vertices, every three floats will be a coordinate
	GLfloat vertices[] =
	{ //	  COORDINATES	   /	     COLORS		//
		-0.5f, 0.0f, 0.5f,		0.8f, 0.3f, 0.02f,	// lower left corner
		-0.5f, 0.0f, -0.5f,		0.8f, 0.3f, 0.02f,	// lower right corner
		0.5f, 0.0f, -0.5f,		1.0f, 0.6f, 0.32f,	// upper corner
		0.5f, 0.0f, 0.5f,		0.9f, 0.45f, 0.17f,	// inner left
		0.0f, 0.8f, 0.0f,		0.9f, 0.45f, 0.17f,	// inner right
	};

	GLuint indices[] =
	{
		0, 1, 2, 
		0, 2, 3, 
		0, 1, 4,
		1, 2, 4,
		2, 3, 4,
		3, 0, 4
	};


	// create a window for rendering
	// input values:
	// width: int
	// height: int
	// title: string
	// monitor: ---
	// share ---
	GLFWwindow* window = glfwCreateWindow(width, height, "Test Window", NULL, NULL);
	// make sure window was made, if not terminate glfw
	if (window == NULL)
	{
		std::cout << "failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	
	// tell glfw that the window we created wants to be used
	// the context is an object that holds stuff (idrk yet)
	glfwMakeContextCurrent(window);

	// load GLAD so that we are able to configure OpenGL with it
	gladLoadGL();

	// specify the viewport of the OpenGL window
	glViewport(0, 0, width, height);

	// generate Shader object that takes in shader source code
	Shader shaderProgram("default.vert", "default.frag");

	// create vertex array object and binds it
	VAO VAO1;
	VAO1.Bind();

	// creates the vertex buffer object, links it to vertices
	VBO VBO1(vertices, sizeof(vertices));
	// creates the element buffer object and links it to indices
	EBO EBO1(indices, sizeof(indices));

	// link the VBO to the VAO
	VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 6 * sizeof(float), (void*)0);
	VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	// unbind the VAO, VBO, and EBO to prevent further modification
	VAO1.Unbind();
	VBO1.Unbind();
	EBO1.Unbind();

	GLuint uniID = glGetUniformLocation(shaderProgram.ID, "scale");

	float rotation = 0.0f;
	double prevTime = glfwGetTime();

	// we need to have a while loop, like a game loop
	// if there is no while loop the window will immediately die
	while (!glfwWindowShouldClose(window))
	{
		// clear the color every frame
		glClearColor(.5f, .3f, .7f, 1.0f);
		// add color to back buffer
		glClear(GL_COLOR_BUFFER_BIT);


		double crntTime = glfwGetTime();
		if (crntTime - prevTime >= 1 / 60)
		{
			rotation += 0.05f;
			prevTime = crntTime;
		}

		shaderProgram.Activate();

		// make matrices of 1s
		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 view = glm::mat4(1.0f);
		glm::mat4 proj = glm::mat4(1.0f);

		model = glm::rotate(model, glm::radians(rotation), glm::vec3(0.0f, 1.0f, 0.0f));
		view = glm::translate(view, glm::vec3(0.0, -0.5f, -2.0f));
		proj = glm::perspective(glm::radians(45.0f), (float)(width / height), 0.1f, 100.0f);
		
		int modelLoc = glGetUniformLocation(shaderProgram.ID, "model");
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		int viewLoc = glGetUniformLocation(shaderProgram.ID, "view");
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		int projLoc = glGetUniformLocation(shaderProgram.ID, "proj");
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(proj));
		
		glUniform1f(uniID, 0.5f);
		// show OpenGL that we want to use this VAO
		VAO1.Bind();
	
		// draw the elements on the screen, it takes the type, the amount of
		// indices, the data type of the indices, and the index of the start of the indices
		glDrawElements(GL_TRIANGLES, sizeof(indices)/sizeof(int), GL_UNSIGNED_INT, 0);

		// swap the front and back buffers of the window
		glfwSwapBuffers(window);

		// process events such as window appearing, resizing, deleting
		glfwPollEvents();
	}

	// delete all shader related objects created to clean up
	VAO1.Delete();
	VBO1.Delete();
	EBO1.Delete();
	shaderProgram.Delete();

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}