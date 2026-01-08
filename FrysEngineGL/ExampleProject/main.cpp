#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <print>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/transform.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <windows.h>

#include <frysGL/shader/Shader.h>
#include <frysGL/camera/Camera.h>

#include <frysGL/buffer/VertexArrayObject.h>
#include <frysGL/buffer/VertexBufferObject.h>

constexpr int WIDTH = 1280;
constexpr int HEIGHT = 720;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 5.0f)	);
bool firstMouse = true;
bool cameraEnabled, enablePressed;
float lastX, lastY;
float aspectRatio = WIDTH / static_cast<float>(HEIGHT);

void FramebufferSizeCallback(GLFWwindow* window, int width, int height);
void ProcessInput(GLFWwindow* window, float deltaTime);
void MouseCallback(GLFWwindow* window, double xpos, double ypos);
void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
void ToggleCamera(GLFWwindow* window);
unsigned int LoadTexture(char const* path);

float deltaTime = 0.0f;	// Time between the current frame and last frame
float lastFrame = 0.0f; // Time of last frame

glm::vec3 cubePositions[] = {
	glm::vec3( 0.0f,  0.0f,  0.0f),
	glm::vec3( 2.0f,  5.0f, -15.0f),
	glm::vec3(-1.5f, -2.2f, -2.5f),
	glm::vec3(-3.8f, -2.0f, -12.3f),
	glm::vec3( 2.4f, -0.4f, -3.5f),
	glm::vec3(-1.7f,  3.0f, -7.5f),
	glm::vec3( 1.3f, -2.0f, -2.5f),
	glm::vec3( 1.5f,  2.0f, -2.5f),
	glm::vec3( 1.5f,  0.2f, -1.5f),
	glm::vec3(-1.3f,  1.0f, -1.5f)
};

constexpr unsigned int POINT_LIGHT_AMOUNT = 4;

glm::vec3 pointLightPositions[] = {
	glm::vec3( 0.7f,  0.2f,  2.0f),
	glm::vec3( 2.3f, -3.3f, -4.0f),
	glm::vec3(-4.0f,  2.0f, -12.0f),
	glm::vec3( 0.0f,  0.0f, -3.0f)
};

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// For macOS
	// glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "LearnOpenGL", nullptr, nullptr);
	if (window == nullptr)
	{
		std::println("Failed to create GLFW window");
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
	{
		std::println("Failed to initialize GLAD");
		return -1;
	}

	glViewport(0, 0, WIDTH, HEIGHT);
	glfwSetFramebufferSizeCallback(window, FramebufferSizeCallback);

	// init camera enabled
	ToggleCamera(window);

	// Init geometry
	constexpr float cubeVertices[] = {
        // positions          // normals           // texture coords
		// Back
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
		// Front
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
		// Left
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
		// Right
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
		// Bottom
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
		// Top
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
    };

	const VertexArrayObject cubeVAO;
	const VertexBufferObject VBO;

	cubeVAO.Bind();

	VBO.Bind();
	VertexBufferObject::SetBufferData(cubeVertices, sizeof(cubeVertices), GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), static_cast<void *>(nullptr));
	glEnableVertexAttribArray(0);

	// Normal attribute
	glVertexAttribPointer(1,3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), reinterpret_cast<void *>(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// UV attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), reinterpret_cast<void *>(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	// unbind VAO and VBO and EBO
	VertexArrayObject::Unbind();

	const VertexArrayObject lightCubeVAO;
	lightCubeVAO.Bind();
	// we only need to bind to the VBO, the container's VBO's handle already contains the data.
	VBO.Bind();

	// set the vertex attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), static_cast<void *>(nullptr));
	glEnableVertexAttribArray(0);

	// unbind
	VertexArrayObject::Unbind();
	VertexBufferObject::Unbind();

	// Init shader
	const Shader lightingShader("assets/shaders/default.vert", "assets/shaders/default.frag");
	const Shader lightSourceShader("assets/shaders/default.vert", "assets/shaders/lightSource.frag");

	auto cubeWorldMatrix = glm::mat4(1.0f);

	unsigned int diffuseMap = LoadTexture("assets/textures/container2.png");
	unsigned int specularMap = LoadTexture("assets/textures/container2_specular.png");

	lightingShader.Use();

	lightingShader.SetInt("material.diffuse", 0);
	lightingShader.SetInt("material.specular", 1);
	lightingShader.SetFloat("material.shininess", 64.0f);

	// SpotLight
	lightingShader.SetFloat("spotLight.cutOff",   glm::cos(glm::radians(12.5f)));
	lightingShader.SetFloat("spotLight.outerCutOff",   glm::cos(glm::radians(17.5f)));

	lightingShader.SetVec3("spotLight.ambient", glm::vec3(0.2f));
	lightingShader.SetVec3("spotLight.diffuse",  glm::vec3(0.5f));
	lightingShader.SetVec3("spotLight.specular", glm::vec3(1.0f));

	lightingShader.SetFloat("spotLight.attenuation.constant",  1.0f);
	lightingShader.SetFloat("spotLight.attenuation.linear",    0.09f);
	lightingShader.SetFloat("spotLight.attenuation.quadratic", 0.032f);


	// directional
	lightingShader.SetVec3("directionalLight.direction", glm::vec3(-0.2f, -1.0f, -0.3f));
	lightingShader.SetVec3("directionalLight.ambient", glm::vec3(0.2f));
	lightingShader.SetVec3("directionalLight.diffuse",  glm::vec3(0.5f));
	lightingShader.SetVec3("directionalLight.specular", glm::vec3(1.0f));

	lightingShader.SetFloat("directionalLight.attenuation.constant",  1.0f);
	lightingShader.SetFloat("directionalLight.attenuation.linear",    0.09f);
	lightingShader.SetFloat("directionalLight.attenuation.quadratic", 0.032f);

	for (unsigned int i = 0; i < POINT_LIGHT_AMOUNT; i++)
	{
		std::string prefix = std::format("pointLights[{}]", i);
		lightingShader.SetVec3(prefix + ".position", pointLightPositions[i]);
		lightingShader.SetVec3(prefix + ".ambient", glm::vec3(0.2f, 0.0f, 0.0f));
		lightingShader.SetVec3(prefix + ".diffuse", glm::vec3(0.5f, 0.0f, 0.0f));
		lightingShader.SetVec3(prefix + ".specular", glm::vec3(1.0f));

		lightingShader.SetFloat(prefix + ".attenuation.constant", 1.0f);
		lightingShader.SetFloat(prefix + ".attenuation.linear", 0.09f);
		lightingShader.SetFloat(prefix + ".attenuation.quadratic", 0.032f);
	}

	lightingShader.SetMat4("modelMatrix", cubeWorldMatrix);

	lightSourceShader.Use();
	lightSourceShader.SetVec3("lightColor", glm::vec3(1.0f, 0.0f, 0.0f));


	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	while (!glfwWindowShouldClose(window))
	{
		const auto currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// process input
		ProcessInput(window, deltaTime);

		// update
		glm::mat4 view = camera.GetViewMatrix();
		glm::mat4 projection = glm::perspective(glm::radians(camera.m_Zoom), aspectRatio, 0.1f, 100.0f);

		// render
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Uncomment to use wireframe mode
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		// auto lightOffset = glm::vec3(0.0f, sin(currentFrame), 0.0f);
		// auto rotation = glm::rotate(cos(currentFrame) * deltaTime, glm::vec3(0.0f, 1.0f, 0.0f));
		// auto translation = glm::translate(lightOffset * deltaTime);
		// lightModel = rotation * translation * lightModel;
		// lightPos = glm::vec3(lightModel[3][0], lightModel[3][1], lightModel[3][2]);

		lightingShader.Use();

		lightingShader.SetMat4("viewMatrix", view);
		lightingShader.SetMat4("projectionMatrix", projection);
		lightingShader.SetVec3("viewPosition", camera.m_Position);

		lightingShader.SetVec3("spotLight.position",  camera.m_Position);
		lightingShader.SetVec3("spotLight.direction", camera.m_Front);

		// bind diffuse map
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, diffuseMap);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, specularMap);

		cubeVAO.Bind();

		for(unsigned int i = 0; i < 10; i++)
		{
			auto model = glm::mat4(1.0f);
			model = glm::translate(model, cubePositions[i]);
			float angle = 20.0f * i;
			model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			lightingShader.SetMat4("modelMatrix", model);

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}


		lightSourceShader.Use();
		lightSourceShader.SetMat4("viewMatrix", view);
		lightSourceShader.SetMat4("projectionMatrix", projection);

		for (auto pointLightPosition : pointLightPositions)
		{
			auto lightModel = glm::mat4(1.0f);
			lightModel = glm::translate(lightModel, pointLightPosition);
			lightModel = glm::scale(lightModel, glm::vec3(0.2f));
			lightSourceShader.SetMat4("modelMatrix", lightModel);
			lightCubeVAO.Bind();
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		VertexArrayObject::Unbind();

		// check and call events and swap the buffers
		glfwPollEvents();
		glfwSwapBuffers(window);
	}

	glfwTerminate();
	return 0;
}

void ToggleCamera(GLFWwindow* window)
{
	cameraEnabled = !cameraEnabled;
	glfwSetInputMode(window, GLFW_CURSOR, cameraEnabled ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
	glfwSetCursorPosCallback(window, cameraEnabled ? MouseCallback : nullptr);
	glfwSetScrollCallback(window, cameraEnabled ? ScrollCallback : nullptr);
}

void FramebufferSizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
	aspectRatio = width / static_cast<float>(height);
}

void ProcessInput(GLFWwindow* window, float deltaTime)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	const bool enableCameraPressed = glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS;
	if (enableCameraPressed && !enablePressed)
	{
		ToggleCamera(window);
		firstMouse = true;
	}
	enablePressed = enableCameraPressed;

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);
}

void MouseCallback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	const float xOffset = xpos - lastX;
	const float yOffset = lastY - ypos;
	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xOffset, yOffset);
}

void ScrollCallback(GLFWwindow* window, double xOffset, double yOffset)
{
	camera.ProcessMouseScroll(static_cast<float>(yOffset));
}

// utility function for loading a 2D texture from file
// ---------------------------------------------------
unsigned int LoadTexture(char const * path)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, components;
	unsigned char *data = stbi_load(path, &width, &height, &components, 0);
	if (data)
	{
		GLenum format{};
		if (components == 1)
			format = GL_RED;
		else if (components == 3)
			format = GL_RGB;
		else if (components == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		std::println("Texture failed to load at path: {}", path);
		stbi_image_free(data);
	}

	return textureID;
}