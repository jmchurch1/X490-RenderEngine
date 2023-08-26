#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>


// copy paste shaders, will learn more about shaders later
// these are just for starting setup
const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";
const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(0.8f, 0.3f, 0.02f, 1.0f);\n"
"}\0";

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

	// OpenGl version of an unsigned int, positive integer
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	// assign the source code of the vertex shader to the variable we created
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	// compile the source code for the vertex shader into machine code since
	// OpenGL can't understand the uncompiled source code
	glCompileShader(vertexShader);

	// same as for vertex shader
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	GLuint shaderProgram = glCreateProgram();
	
	// attach the two shaders that we just created to the program
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	// wrap up the shader program
	glLinkProgram(shaderProgram);

	// delete the two shaders that were created since they are in the program
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	// Create reference containers for the vertex array object, vertex buffer object
	// create reference for the index buffer object
	GLuint VAO, VBO, EBO;

	// the VAO needs to be created before the VBO
	glGenVertexArrays(1, &VAO);
	// create a buffer object, normally an array of references
	glGenBuffers(1, &VBO); // use 1 since we only have 1 3D object
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	// binding makes an object the current object
	// GL_ARRAY_BUFFER is the type of buffer we are passing
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// store the vertices in the VBO
	// DRAW means that the vertices are used to draw vertices onto the screen
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// make the EBO current
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	// add the indices that we want to use tot he buffer
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// pass in the vertex attribute that we want to use
	// inputs:
	// the position of the vertex attrib: int
	// values per vertex: int
	// what types of values: GL
	// whether or not input is integer: GL
	// stride of the vertices, data between each vertex: float
	// offset, pointer to where the vertices begin in the array: int
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	// enable the vertex attribute, 0 is the position of the vertex attrib
	glEnableVertexAttribArray(0);

	// make sure we don't change the VAO or VBO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	// make sure to unbind after the VAO so that we are using the EBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);


	// we need to have a while loop, like a game loop
	// if there is no while loop the window will immediately die
	while (!glfwWindowShouldClose(window))
	{
		// clear the color every frame
		glClearColor(.5f, .3f, .7f, 1.0f);
		// add color to back buffer
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shaderProgram);
		// show OpenGL that we want to use this VAO
		glBindVertexArray(VAO);
	
		// draw the elements on the screen, it takes the type, the amount of
		// indices, the data type of the indices, and the index of the start of the indices
		glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, 0);

		// swap the front and back buffers of the window
		glfwSwapBuffers(window);

		// process events such as window appearing, resizing, deleting
		glfwPollEvents();
	}

	// delete all shader related objects created to clean up
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteProgram(shaderProgram);

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}