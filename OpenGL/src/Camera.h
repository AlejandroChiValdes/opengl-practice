#pragma once
#include "GL/glew.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "GLFW/glfw3.h"
#include <memory>


class Camera
{
private: 
	glm::vec3 cameraPos;
	glm::vec3 viewTarget;
	glm::vec3 upVector;
	float lastMouseX;
	float lastMouseY;
	float mouseOffsetX;
	float mouseOffsetY;
	float pitch;
	float yaw;
	static std::shared_ptr<Camera> currentCamera;
	static GLFWwindow* window;
	float deltaTime;
	float lastFrame;

public:

	// Attaches any environment-level data structures that the Camera class needs to process input.
	static void CreateCameraContext(GLFWwindow* w);

	static std::shared_ptr<Camera> CreateCamera();

	static std::shared_ptr<Camera> CreateCamera(const glm::vec3& cameraPos, const glm::vec3& viewTarget, const glm::vec3& upVector);

	void ApplyViewMatrix(glm::mat4& transform);

	void Translate(const glm::vec3& translation);

	void Rotate(const glm::vec3& viewTargetOffset);

	void ProcessInput();

	Camera();

	Camera(const glm::vec3& cameraPos, const glm::vec3& viewTarget, const glm::vec3& upVector);

	~Camera();

private:

	Camera(const Camera& rhs);

	static void mouse_callback(GLFWwindow* window, double xpos, double ypos);

	void SetInputBindings();
	
};