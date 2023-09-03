#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>

#include"shaderClass.h"
#include"VAO.h"
#include"VBO.h"
#include"EBO.h"




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
	{
		-0.9f, -0.7f, 0.0f, // lower left corner
		0.6f, -0.9f, 0.0f, // lower right corner
		0.1f, 0.8f, 0.0f, // upper corner
		-0.3f, -0.1f, 0.0f, // inner left
		0.3f, -0.1f, 0.0f, // inner right
		0.1f, -0.8f, 0.0f // inner down
	};

	GLuint indices[] =
	{
		0, 3, 5, // lower left triangle
		3, 2, 4, // lower right triangle
		5, 4, 1 // upper triangle
	};


	// create a window for rendering
	// input values:
	// width: int
	// height: int
	// title: string
	// monitor: ---
	// share ---
	GLFWwindow* window = glfwCreateWindow(800, 800, "Test Window", NULL, NULL);
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
	glViewport(0, 0, 800, 800);

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
	VAO1.LinkVBO(VBO1, 0);
	// unbind the VAO, VBO, and EBO to prevent further modification
	VAO1.Unbind();
	VBO1.Unbind();
	EBO1.Unbind();

	// we need to have a while loop, like a game loop
	// if there is no while loop the window will immediately die
	while (!glfwWindowShouldClose(window))
	{
		// clear the color every frame
		glClearColor(.5f, .3f, .7f, 1.0f);
		// add color to back buffer
		glClear(GL_COLOR_BUFFER_BIT);

		shaderProgram.Activate();
		// show OpenGL that we want to use this VAO
		VAO1.Bind();
	
		// draw the elements on the screen, it takes the type, the amount of
		// indices, the data type of the indices, and the index of the start of the indices
		glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, 0);

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