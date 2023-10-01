#include"Model.h"
#include"Framebuffer.h"

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

	Vertex lightVertices[] =
	{
		Vertex{glm::vec3(-0.1f, -0.1f, 0.1f)},
		Vertex{glm::vec3(-0.1f, -0.1f, -0.1f)},
		Vertex{glm::vec3(0.1f, -0.1f, -0.1f)},
		Vertex{glm::vec3(0.1f, -0.1f, 0.1f)},
		Vertex{glm::vec3(-0.1f, 0.1f, -0.1f)},
		Vertex{glm::vec3(0.1f, 0.1f, -0.1f)},
		Vertex{glm::vec3(0.1f, 0.1f, 0.1f)}
	};

	GLuint lightIndices[]
	{
		0, 1, 2,
		0, 2, 3,
		0, 4, 7,
		0, 7, 3,
		3, 7, 6,
		3, 6, 2,
		2, 6, 5,
		2, 5, 1,
		1, 5, 4,
		1, 4, 0,
		4, 5, 6,
		4, 6, 7
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

	Shader secondPass("secondPass.vert", "secondPass.frag");

	// generate Shader object that takes in shader source code
	Shader shaderProgram("default.vert", "default.frag");
	shaderProgram.Activate();

	Model model("models/magnet/Magnet Scene.gltf");

	// shader for light cube
	Shader lightShader("light.vert", "light.frag");
	// storing mesh data
	std::vector <Vertex> lightVerts(lightVertices, lightVertices + sizeof(lightVertices) / sizeof(Vertex));	// make vector of vertices with correct memory size
	std::vector <GLuint> lightInd(lightIndices, lightIndices + sizeof(lightIndices) / sizeof(GLuint));		// make vector of indices with correct memory size
	std::vector <Texture> tex;

	Mesh light(lightVerts, lightInd, tex);

	glm::vec4 lightColor = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
	glm::vec3 lightPos = glm::vec3(0.0f, 4.0f, 0.0f);
	glm::mat4 lightModel = glm::mat4(1.0f);
	lightModel = glm::translate(lightModel, lightPos);

	glm::vec3 objectPos = glm::vec3(0.5f, 0.5f, 0.5f);
	glm::mat4 objectModel = glm::mat4(1.0f);
	objectModel = glm::translate(objectModel, objectPos);

	lightShader.Activate();
	glUniformMatrix4fv(glGetUniformLocation(lightShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(lightModel));
	glUniform4f(glGetUniformLocation(lightShader.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	shaderProgram.Activate();
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "model"), 1, GL_FALSE, glm::value_ptr(objectModel));
	glUniform4f(glGetUniformLocation(shaderProgram.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);

	//Framebuffer fbo = Framebuffer::Framebuffer(width, height);

	// Set up shaders and state for second blur pass, and render.

	// enable the depth buffer
	glEnable(GL_DEPTH_TEST);

	Camera camera(width, height, glm::vec3(0.0f, 0.0f, 4.0f));

	// Variables that help the rotation of the pyramid
	float rotation = 0.0f;
	double prevTime = glfwGetTime();

	Framebuffer Framebuffer(width, height);	// get the framebuffer for a second pass
	// VAO ScreenQuad;

	float vertices[] =
	{
		-1.0f, 1.0f, 0.0f, 1.0f,
		-1.0f, -1.0f, 0.0f, 0.0f,
		1.0f, -1.0f, 1.0f, 0.0f,

		-1.0f, 1.0f, 0.0f, 1.0f,
		1.0f, -1.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 1.0f, 1.0f,

	};

	// create quad vao
	GLuint quadVAO, quadVBO;
	glGenVertexArrays(1, &quadVAO);
	glGenBuffers(1, &quadVBO);
	glBindVertexArray(quadVAO);
	glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

	// we need to have a while loop, like a game loop
	// if there is no while loop the window will immediately die
	while (!glfwWindowShouldClose(window))
	{
		double crntTime = glfwGetTime();
		if (crntTime - prevTime >= 1 / 60)
		{
			rotation += 0.5f;
			prevTime = crntTime;
		}

		// bind framebuff so it is drawn to
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, Framebuffer.ID);
		glEnable(GL_DEPTH_TEST);

		// clear the color every frame
		glClearColor(.1f, .4f, .2f, 1.0f);
		// add color to back buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		camera.Inputs(window);
		camera.updateMatrix(45.0f, 0.1f, 700.0f);

		model.Draw
		(
			shaderProgram, 
			camera, 
			glm::vec3(0.0f, 0.0f, -0.11f), 
			glm::vec3(0.0f, 0.0f, 0.0f),
			glm::vec3(1.0f, 1.0f, 1.0f)
		);
		light.Draw
		(
			lightShader, 
			camera,
			glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 4.0f, 0.0f))
		);

		// bind back to the default framebuffer
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
		glDisable(GL_DEPTH_TEST);

		// clear the color buffer, it will be behind the rendered quad
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// activate second pass shader
		// bind vertex array
		
		secondPass.Activate();											// activate the second pass shader
		glBindVertexArray(quadVAO);										// bind the vertex array object with quad vertices
		glBindTexture(GL_TEXTURE_2D, Framebuffer.textureColorBuffer);	// bind the color buffer
		glDrawArrays(GL_TRIANGLES, 0, 6);								// draw the quad

		// bind texture color buffer
		// draw the quad

	

		// swap the front and back buffers of the window
		glfwSwapBuffers(window);

		// process events such as window appearing, resizing, deleting
		glfwPollEvents();
	}

	// delete all shader related objects created to clean up
	shaderProgram.Delete();
	secondPass.Delete();
	lightShader.Delete();

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}