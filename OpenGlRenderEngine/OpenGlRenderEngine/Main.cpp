#include"Model.h"
#include"PointCloud.h"
#include"Framebuffer.h"

const unsigned int width = 800;
const unsigned int height = 800;
float reflectionBoxSize = 2.0f;
float bigBoxSize = 10.0f;
float skyBoxSize = 500;

std::vector<Vertex> vertexList = std::vector<Vertex>();

glm::vec3 PointOnCircle(glm::vec3 center, float radius, float angle)
{
	glm::vec3 pos = glm::vec3(0.0f, 0.0f, 0.0f);
	pos.x = center.x;
	pos.y = center.y + radius * cos(angle * (3.14159 / 180));
	pos.z = center.z + radius * sin(angle * (3.14159 / 180));
	return pos;
}

glm::vec3 PointOnSphere(glm::vec3 center, float radius, float zAngle, float heightAngle)
{
	glm::vec3 pos = glm::vec3(0.0f, 0.0f, 0.0f);
	pos.x = center.x + radius * cos(zAngle * (3.14159 / 180)) * sin(heightAngle * (3.14159 / 180));
	pos.y = center.y + radius * sin(zAngle * (3.14159 / 180)) * sin(heightAngle * (3.14159 / 180));
	pos.z = center.z + radius * cos(heightAngle * (3.14159 / 180));
	return pos;
}

// https://stackoverflow.com/questions/4436764/rotating-a-quaternion-on-1-axis
glm::quat create_from_axis_angle(float xx, float yy, float zz, float a)
{
	// Here we calculate the sin( theta / 2) once for optimization
	float factor = sin(a / 2.0);

	// Calculate the x, y and z of the quaternion
	float x = xx * factor;
	float y = yy * factor;
	float z = zz * factor;

	// Calcualte the w value by cos( theta / 2 )
	float w = cos(a / 2.0);

	return glm::normalize(glm::quat(x, y, z, w));
}


void DrawMagnetEffect(Shader shaderProgram, Camera camera, Mesh baseObj, float currentTime, int maxTime, float radius, float speed, float scale, glm::vec3 startPosition, glm::vec3 directionVector, int effects)
{
	float baseRadius = 0.2f;

	for (int j = 0; j < effects; j++)
	{

		for (int i = 0; i < 361; i++)
		{
			if (currentTime > maxTime)
			{
				float decimals = currentTime - float(int(currentTime));
				currentTime = (int(currentTime) % maxTime) + decimals;
			}

			currentTime += (float(j) / float(effects)) * float(maxTime);

			glm::vec3 translation;

			if (speed > 0)
			{
				translation = PointOnCircle(startPosition, (radius - baseRadius) * (currentTime / float(maxTime)) + baseRadius, float(i));
			}
			else
			{
				translation = PointOnCircle(startPosition, radius * (maxTime*2 - (currentTime / float(maxTime))), float(i));
			}
			glm::vec3 timeTranslation = directionVector * currentTime * speed;
			baseObj.Draw	// bottom
			(
				shaderProgram,
				camera,
				glm::mat4(1.0f),
				translation + timeTranslation,
				glm::quat(1.0f, 0.0f, 0.0f, 0.0f),
				glm::vec3(scale, scale, scale)
			);
		}
	}
}

void DrawSphere(Shader shaderProgram, Camera camera, Mesh baseObj)
{
	float startingPosition = 0.0f;
	float startingScale = 0.1f;
	float scaleIncrement = 0.01f;
	glm::quat startingRotation = glm::quat(0.0f, 1.0f, 0.0f, 0.0f);

	int createdPlanes = 30;

	for (int layer = 0; layer < 360; layer++)
	{

		baseObj.Draw	// bottom
		(
			shaderProgram,
			camera,
			glm::mat4(1.0f),
			glm::vec3(0.0f, startingPosition, 0.0f),
			startingRotation,
			glm::vec3(startingScale, startingScale, startingScale)
		);

		// increment height of starting position
		startingPosition -= 0.1f;

		// increment/decrement scaling
		if (layer < 180)
		{
			startingScale += scaleIncrement;
		}
		else
		{
			startingScale -= scaleIncrement;
		}
	}
}

PointCloud InitializePointCloud()
{
	// creating all the vertex points for the point cloud
	// setting all normals to vec3(0,1,0)
	// setting color to red

	glm::vec3 currCenter = glm::vec3(0, 0, 0);
	glm::vec3 normal = glm::vec3(0, 1, 0);
	glm::vec3 color = glm::vec3(1, 0, 0);
	float radius = 10.0f;

	// initailize vertexList
	if (vertexList.empty()) {
		for (int j = 0; j < 181; j+= 2) {
			for (int i = 0; i < 360; i+=2)
			{
				glm::vec3 position = PointOnSphere(currCenter, radius, i, j);
				vertexList.push_back(
					Vertex{ position, normal, color, glm::vec2((float)i / 360.0f, ((float)j + 90.0f) / 270.0f) }
				);
			}
		}
	}
	std::vector <Texture> worldTex =
	{
		Texture("deathstar2.png", "diffuse", 0)
	};

	PointCloud cloud(vertexList, worldTex);
	return cloud;
}

void DrawBox(Shader shaderProgram, Camera camera, Mesh plane1)
{
	plane1.Draw	// bottom
	(
		shaderProgram,
		camera,
		glm::mat4(1.0f),
		glm::vec3(0.0f, reflectionBoxSize + bigBoxSize, 0.0f),
		glm::quat(0.0f, 0.0f, 0.0f, 1.0f),
		glm::vec3(reflectionBoxSize, reflectionBoxSize, reflectionBoxSize)
	);
	plane1.Draw	// ceiling
	(
		shaderProgram,
		camera,
		glm::mat4(1.0f),
		glm::vec3(0.0f, -reflectionBoxSize + bigBoxSize, 0.0f),
		glm::quat(0.0f, 0.0f, 0.0f, 1.0f),
		glm::vec3(reflectionBoxSize, reflectionBoxSize, reflectionBoxSize)
	);
	plane1.Draw // right wall
	(
		shaderProgram,
		camera,
		glm::mat4(1.0f),
		glm::vec3(0.0f, reflectionBoxSize + bigBoxSize, 2.0f * reflectionBoxSize),
		glm::quat(0.5f, 0.5f, 0.5f, 0.5f),
		glm::vec3(reflectionBoxSize, reflectionBoxSize, reflectionBoxSize)
	);
	plane1.Draw // left wall
	(
		shaderProgram,
		camera,
		glm::mat4(1.0f),
		glm::vec3(0.0f, reflectionBoxSize + bigBoxSize, 0.0f),
		glm::quat(0.5f, 0.5f, 0.5f, 0.5f),
		glm::vec3(reflectionBoxSize, reflectionBoxSize, reflectionBoxSize)
	);
	plane1.Draw // back wall
	(
		shaderProgram,
		camera,
		glm::mat4(1.0f),
		glm::vec3(-2.0f * reflectionBoxSize, reflectionBoxSize + bigBoxSize, 0.0f),
		glm::quat(0.7071f, 0.0f, 0.0f, 0.7071f),
		glm::vec3(reflectionBoxSize, reflectionBoxSize, reflectionBoxSize)
	);
	plane1.Draw // front wall
	(
		shaderProgram,
		camera,
		glm::mat4(1.0f),
		glm::vec3(0.0f, reflectionBoxSize + bigBoxSize, 0.0f),
		glm::quat(0.7071f, 0.0f, 0.0f, 0.7071f),
		glm::vec3(reflectionBoxSize, reflectionBoxSize, reflectionBoxSize)
	);
}
void DrawSkybox(Shader shaderProgram, Camera camera, Mesh right, Mesh left, Mesh top, Mesh bottom, Mesh front, Mesh back)
{
	glm::vec3 center = camera.Position;


	bottom.Draw	// bottom
	(
		shaderProgram,
		camera,
		glm::mat4(1.0f),
		glm::vec3(-center.x, skyBoxSize - skyBoxSize/2 - center.y, -center.z),
		glm::quat(0.0f, 0.0f, 0.0f, 1.0f),
		glm::vec3(skyBoxSize, skyBoxSize, skyBoxSize)
	);
	top.Draw	// ceiling
	(
		shaderProgram,
		camera,
		glm::mat4(1.0f),
		glm::vec3(-center.x, -skyBoxSize - skyBoxSize / 2 - center.y, -center.z),
		glm::quat(0.0f, 0.0f, 0.0f, 1.0f),
		glm::vec3(skyBoxSize, skyBoxSize, skyBoxSize)
	);
	right.Draw // right wall
	(
		shaderProgram,
		camera,
		glm::mat4(1.0f),
		glm::vec3(-center.x, skyBoxSize - skyBoxSize / 2 - center.y, 2.0f * skyBoxSize + -center.z),
		glm::quat(0.5f, 0.5f, 0.5f, 0.5f),
		glm::vec3(skyBoxSize, skyBoxSize, skyBoxSize)
	);
	left.Draw // left wall
	(
		shaderProgram,
		camera,
		glm::mat4(1.0f),
		glm::vec3(-center.x, skyBoxSize - skyBoxSize / 2 - center.y, -center.z),
		glm::quat(0.5f, 0.5f, 0.5f, 0.5f),
		glm::vec3(skyBoxSize, skyBoxSize, skyBoxSize)
	);
	back.Draw // back wall
	(
		shaderProgram,
		camera,
		glm::mat4(1.0f),
		glm::vec3(-center.x -2.0f * skyBoxSize, skyBoxSize - skyBoxSize / 2 - center.y, -center.z),
		glm::quat(0.7071f, 0.0f, 0.0f, 0.7071f),
		glm::vec3(skyBoxSize, skyBoxSize, skyBoxSize)
	);
	front.Draw // front wall
	(
		shaderProgram,
		camera,
		glm::mat4(1.0f),
		glm::vec3(-center.x, skyBoxSize - skyBoxSize / 2 - center.y, -center.z),
		glm::quat(0.7071f, 0.0f, 0.0f, 0.7071f),
		glm::vec3(skyBoxSize, skyBoxSize, skyBoxSize)
	);
}

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

	Vertex planeVertices[] =
	{
		Vertex{glm::vec3(-1.0f, -1.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 0.0f)},
		Vertex{glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 1.0f)},
		Vertex{glm::vec3(1.0f, -1.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 1.0f)},
		Vertex{glm::vec3(1.0f, -1.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 0.0f)},
	};

	GLuint planeIndices[] =
	{
		0, 1, 2,
		0, 2, 3
	};

	GLuint skyboxIndices[] = {
		//Top
		2, 6, 7,
		2, 3, 7,

		//Bottom
		0, 4, 5,
		0, 1, 5,

		//Left
		0, 2, 6,
		0, 4, 6,

		//Right
		1, 3, 7,
		1, 5, 7,

		//Front
		0, 2, 3,
		0, 1, 3,

		//Back
		4, 6, 7,
		4, 5, 7
	};

	Vertex skyboxVertices[] = {
		Vertex{ glm::vec3(-1.0f, -1.0f,  1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 0.0f)  },
		Vertex{ glm::vec3(1.0f, -1.0f,  1.0f),  glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 1.0f) },
		Vertex{ glm::vec3(-1.0f,  1.0f,  1.0f),  glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 1.0f) },
		Vertex{ glm::vec3(1.0f,  1.0f,  1.0f),  glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 0.0f) },
		Vertex{ glm::vec3(-1.0f, -1.0f, -1.0f),  glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 0.0f) },
		Vertex{ glm::vec3(1.0f, -1.0f, -1.0f),  glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 1.0f) },
		Vertex{ glm::vec3(-1.0f,  1.0f, -1.0f),  glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 1.0f) },
		Vertex{ glm::vec3(1.0f,  1.0f, -1.0f),  glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 0.0f) }
	};

	float skyboxVerticesLong[] = {
		// positions          
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f
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


	//Shader reflectShader("default.vert", "reflect.frag");
	// generate Shader object that takes in shader source code
	Shader pointCloud("default.vert", "pointcloud.frag");
	Shader shaderProgram("default.vert", "default.frag");
	shaderProgram.Activate();

	Model model("models/magnet/Magnet Scene.gltf");

	// shader for light cube
	Shader lightShader("light.vert", "light.frag");
	// storing mesh data
	std::vector <Vertex> lightVerts(lightVertices, lightVertices + sizeof(lightVertices) / sizeof(Vertex));	// make vector of vertices with correct memory size
	std::vector <GLuint> lightInd(lightIndices, lightIndices + sizeof(lightIndices) / sizeof(GLuint));		// make vector of indices with correct memory size
	std::vector <Texture> lightTex;

	Mesh light(lightVerts, lightInd, lightTex);

	Shader boxShader("default.vert", "default.frag");
	// storing mesh data
	std::vector <Vertex> boxVerts(planeVertices, planeVertices + sizeof(planeVertices) / sizeof(Vertex));	// make vector of vertices with correct memory size
	std::vector <GLuint> boxInd(planeIndices, planeIndices + sizeof(planeIndices) / sizeof(GLuint));		// make vector of indices with correct memory size
	std::vector <Texture> boxTex =
	{
		Texture("sample.jpg", "diffuse", 0)
	};
	std::vector <Texture> colorTex =
	{
		Texture("colors.jpg", "diffuse", 0)
	};


	Shader skyboxShader("skybox.vert", "skybox.frag");

	// https://learnopengl.com/Advanced-OpenGL/Cubemaps
	std::vector<std::string> faces =
	{
			"right.png",
			"left.png",
			"top.png",
			"bottom.png",
			"front.png",
			"back.png"
	};

	// storing mesh data
	std::vector <Vertex> skyboxVerts(skyboxVertices, skyboxVertices + sizeof(skyboxVertices) / sizeof(Vertex));		// make vector of vertices with correct memory size
	std::vector <GLuint> skyboxInd(skyboxIndices, skyboxIndices + sizeof(skyboxIndices) / sizeof(GLuint));		// make vector of indices with correct memory size
	std::vector <Texture> rightTex =
	{
		Texture("right.png", "diffuse", 0),
	};
	std::vector <Texture> leftTex =
	{
		Texture("left.png", "diffuse", 0)
	};
	std::vector <Texture> topTex =
	{
		Texture("top.png", "diffuse", 0)
	};
	std::vector <Texture> bottomTex =
	{
		Texture("bottom.png", "diffuse", 0)
	};
	std::vector <Texture> frontTex =
	{
		Texture("front.png", "diffuse", 0)
	};
	std::vector <Texture> backTex =
	{
		Texture("back.png", "diffuse", 0)
	};

	Mesh plane1(boxVerts, boxInd, boxTex);
	Mesh plane2(boxVerts, boxInd, boxTex);
	Mesh plane3(boxVerts, boxInd, colorTex);

	Mesh right(boxVerts, boxInd, rightTex);
	Mesh left(boxVerts, boxInd, leftTex);
	Mesh top(boxVerts, boxInd, topTex);
	Mesh bottom(boxVerts, boxInd, bottomTex);
	Mesh front(boxVerts, boxInd, frontTex);
	Mesh back(boxVerts, boxInd, backTex);


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
	boxShader.Activate();
	glUniformMatrix4fv(glGetUniformLocation(boxShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(objectModel));
	glUniform4f(glGetUniformLocation(boxShader.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	glUniform3f(glGetUniformLocation(boxShader.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);


	PointCloud cloud = InitializePointCloud();

	//Framebuffer fbo = Framebuffer::Framebuffer(width, height);

	// Set up shaders and state for second blur pass, and render.

	// enable the depth buffer
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_PROGRAM_POINT_SIZE);


	// enable alpha blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

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

	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

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
		glClearColor(1.0f,1.0f,1.0f, 1.0f);
		// add color to back buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		camera.Inputs(window);
		camera.updateMatrix(45.0f, 0.1f, 6000.0f);

		glDepthMask(GL_FALSE);
		// ... set view and projection matrix
		DrawSkybox(boxShader, camera, right, left, top, bottom, front, back);
		glDepthMask(GL_TRUE);

		/*
		DrawMagnetEffect(shaderProgram, camera, plane2, crntTime, 1, 2.0f, -5.0f, 0.2f, glm::vec3(15.0f, -5.5f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), 3);

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
			glm::mat4(1.0f), 
			glm::vec3(0.0f, 4.0f, 0.0f)
		);
		plane1.Draw	// bottom
		(
			boxShader,
			camera,
			glm::mat4(1.0f),
			glm::vec3(0.0f, bigBoxSize, 0.0f),
			glm::quat(0.0f,0.0f,0.0f,1.0f),
			glm::vec3(bigBoxSize,bigBoxSize,bigBoxSize)
		);
		plane1.Draw	// ceiling
		(
			boxShader,
			camera,
			glm::mat4(1.0f),
			glm::vec3(0.0f, -bigBoxSize, 0.0f),
			glm::quat(0.0f, 0.0f, 0.0f, 1.0f),
			glm::vec3(bigBoxSize, bigBoxSize, bigBoxSize)
		);
		plane1.Draw // right wall
		(
			boxShader,
			camera,
			glm::mat4(1.0f),
			glm::vec3(0.0f, bigBoxSize, 2.0f * bigBoxSize),
			glm::quat(0.5f, 0.5f, 0.5f, 0.5f),
			glm::vec3(bigBoxSize, bigBoxSize, bigBoxSize)
		);
		plane1.Draw // left wall
		(
			boxShader,
			camera,
			glm::mat4(1.0f),
			glm::vec3(0.0f, bigBoxSize, 0.0f),
			glm::quat(0.5f, 0.5f, 0.5f, 0.5f),
			glm::vec3(bigBoxSize, bigBoxSize, bigBoxSize)
		);
		plane1.Draw // back wall
		(
			boxShader,
			camera,
			glm::mat4(1.0f),
			glm::vec3(-2.0f * bigBoxSize, bigBoxSize, 0.0f),
			glm::quat(0.7071f, 0.0f, 0.0f, 0.7071f),
			glm::vec3(bigBoxSize, bigBoxSize, bigBoxSize)
		);

		
		DrawBox(shaderProgram, camera, plane2);
		*/
		cloud.Draw(pointCloud, camera);

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