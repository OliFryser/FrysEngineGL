#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

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

float deltaTime = 0.0f;	// Time between the current frame and last frame
float lastFrame = 0.0f; // Time of last frame

glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

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
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	glViewport(0, 0, WIDTH, HEIGHT);
	glfwSetFramebufferSizeCallback(window, FramebufferSizeCallback);

	// init camera enabled
	ToggleCamera(window);

	// Init geometry
	constexpr float cubeVertices[] = {
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
	};

	const VertexArrayObject VAO;
	const VertexBufferObject VBO;

	VAO.Bind();

	VBO.Bind();
	VertexBufferObject::SetBufferData(cubeVertices, sizeof(cubeVertices), GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), static_cast<void *>(nullptr));
	glEnableVertexAttribArray(0);

	// UV attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), reinterpret_cast<void *>(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// unbind VAO and VBO and EBO
	VertexArrayObject::Unbind();
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	VertexArrayObject lightVAO;
	lightVAO.Bind();
	// we only need to bind to the VBO, the container's VBO's handle already contains the data.
	VBO.Bind();

	// set the vertex attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), static_cast<void *>(nullptr));
	glEnableVertexAttribArray(0);

	// unbind
	VertexArrayObject::Unbind();
	VertexBufferObject::Unbind();

	// Init shader
	const Shader lightingShader("shaders/default.vert", "shaders/default.frag");
	const Shader lightSourceShader("shaders/default.vert", "shaders/lightSource.frag");

	auto cubeWorldMatrix = glm::mat4(1.0f);

	lightingShader.Use();
	lightingShader.SetMat4("ModelMatrix", cubeWorldMatrix);
	lightingShader.SetVec3("ObjectColor", glm::vec3(1.0f, 0.5f, 0.3f));
	lightingShader.SetVec3("LightColor", glm::vec3(1.0f, 1.0f, 1.0f));

	auto lightModel = glm::mat4(1.0f);
	lightModel = glm::translate(lightModel, lightPos);
	lightModel = glm::scale(lightModel, glm::vec3(0.2f));

	glEnable(GL_DEPTH_TEST);

	while (!glfwWindowShouldClose(window))
	{
		const auto currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// process input
		ProcessInput(window, deltaTime);

		// update
		glm::mat4 view = camera.GetViewMatrix();
		glm::mat4 projection = glm::perspective(glm::radians(camera.m_zoom), aspectRatio, 0.1f, 100.0f);

		// render
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Uncomment to use wireframe mode
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		auto lightOffset = glm::vec3(0.0f, sin(currentFrame), 0.0f);
		auto rotation = glm::rotate(cos(currentFrame) * deltaTime, glm::vec3(0.0f, 1.0f, 0.0f));
		auto translation = glm::translate(lightOffset * deltaTime);
		lightModel = rotation * translation * lightModel;
		lightPos = glm::vec3(lightModel[3][0], lightModel[3][1], lightModel[3][2]);

		lightingShader.Use();
		lightingShader.SetMat4("ViewMatrix", view);
		lightingShader.SetMat4("ProjectionMatrix", projection);
		lightingShader.SetVec3("LightPos", lightPos);
		lightingShader.SetVec3("ViewPos", camera.m_position);
		VAO.Bind();
		glDrawArrays(GL_TRIANGLES, 0, 36);

		lightSourceShader.Use();
		lightSourceShader.SetMat4("ModelMatrix", lightModel);
		lightSourceShader.SetMat4("ViewMatrix", view);
		lightSourceShader.SetMat4("ProjectionMatrix", projection);
		lightVAO.Bind();
		glDrawArrays(GL_TRIANGLES, 0, 36);
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
	glfwSetCursorPosCallback(window, cameraEnabled ? MouseCallback : NULL);
	glfwSetScrollCallback(window, cameraEnabled ? ScrollCallback : NULL);
}

void FramebufferSizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
	aspectRatio = width / (float)height;
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