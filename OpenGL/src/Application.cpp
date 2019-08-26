#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <assimp/include/assimp/scene.h>
#include <assimp/include/assimp/Importer.hpp>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <cmath>
#include "Model.h"
#include "Camera.h"
#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "VertexBufferLayout.h"
#include "Texture.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

constexpr auto screenHeight = 960;
constexpr auto screenWidth = 960;
const double pi = 3.14159265358979323846;

void shiftX(float newPositionOffset, float* positions, float* raw_positions)
{
	int offset = 4;
	for (int i = 0; i < 4; ++i)
	{
		positions[i * offset] = raw_positions[i * offset] + screenWidth * newPositionOffset;
	}
}

float degToRad(float deg)
{
	return deg * (pi / 180.0f);
}
void rotateZ(float deg, glm::mat4& dst)
{
	float rad = degToRad(deg);
	glm::vec4 x{ cos(rad), -1 * sin(rad), 0, 0 };
	glm::vec4 y{ sin(rad), cos(rad), 0, 0 };
	glm::vec4 z{ 0, 0, 1, 0 };
	glm::vec4 w{ 0, 0, 0, 1 };
	glm::mat4 transform{ x, y, z, w };
	dst = transform * dst;
}

int main(void)
{
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	/* Create a windowed mode window and its OpenGL context */

	window = glfwCreateWindow(screenHeight, screenWidth, "Hello World", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// sets the current opengl profile to be the 'core' profile. Default is the compatibility profile, which is the latest version
	// you have in your graphics drivers
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	/* Make the window's context current */
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);
	

	if (glewInit() != GLEW_OK)
	{
		std::cout << "Error!" << std::endl;
	}
	std::cout << glGetString(GL_VERSION) << std::endl;

	//float positions[] = {
	//0.5f, -0.5f, -0.5f,  1.0f, 0.0f, // 1 , 0
	//-0.5f, -0.5f, -0.5f,  0.0f, 0.0f, // 0, 1
	// -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // 3, 2
	// 0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // 2, 3
	//
	//-0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // 7, 4
	// 0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // 4, 5
	// 0.5f,  0.5f,  0.5f, 0.0f, 1.0f, // 5, 6
	//-0.5f,  0.5f,  0.5f,  1.0f, 1.0f, // 6, 7

	//0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // 4, 8
	//0.5f, -0.5f, -0.5f,  0.0f, 0.0f, // 1, 9
	// 0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // 2, 10
	// 0.5f,  0.5f,  0.5f,  1.0f, 1.0f, // 5, 11

	// -0.5f, -0.5f, -0.5f,  1.0f, 0.0f, // 0, 12
	// -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // 7, 13
	//-0.5f,  0.5f,  0.5f,  0.0f, 1.0f, // 6, 14
	// -0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // 3, 15

	//-0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // 6, 16
	//0.5f,  0.5f,  0.5f,  0.0f, 0.0f, // 5, 17
	// 0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // 2, 18
	//-0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // 3, 19

	//-0.5f, -0.5f, -0.5f,  1.0f, 0.0f, // 0, 20
	//0.5f, -0.5f, -0.5f,  0.0f, 0.0f, // 1, 21
	//0.5f, -0.5f,  0.5f,  0.0f, 1.0f, // 4, 22
	//-0.5f, -0.5f,  0.5f,  1.0f, 1.0f, // 7, 23
	//};

	float positions[] = {
0.5f, -0.5f, -0.5f,  
-0.5f, -0.5f, -0.5f, 
 -0.5f,  0.5f, -0.5f,
 0.5f,  0.5f, -0.5f, 

-0.5f, -0.5f,  0.5f, 
 0.5f, -0.5f,  0.5f, 
 0.5f,  0.5f,  0.5f, 
-0.5f,  0.5f,  0.5f, 

0.5f, -0.5f,  0.5f,  
0.5f, -0.5f, -0.5f,  
 0.5f,  0.5f, -0.5f, 
 0.5f,  0.5f,  0.5f, 

 -0.5f, -0.5f, -0.5f,
 -0.5f, -0.5f,  0.5f,
-0.5f,  0.5f,  0.5f, 
 -0.5f,  0.5f, -0.5f,

-0.5f,  0.5f,  0.5f, 
0.5f,  0.5f,  0.5f,  
 0.5f,  0.5f, -0.5f, 
-0.5f,  0.5f, -0.5f, 

-0.5f, -0.5f, -0.5f, 
0.5f, -0.5f, -0.5f,  
0.5f, -0.5f,  0.5f,  
-0.5f, -0.5f,  0.5f, 
	};

float diffuseVertices[] = {
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, //bottom back left 0
		0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, // bottom back right 1
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, // top back right 2
	-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, // top back left 3 

	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f, // bottom front left 4
		0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f, // bottom front right 5
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f, // top front right 6
	-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f, // top front left 7

	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f, // top front left 8
	-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f, // top back left 9
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f, // bottom back left 10
	-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f, // bottom front left 11

		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f, // top front right 12
		0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f, // top back right 13
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f, // bottom back right 14
		0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f, // bottom front right 15

	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, // bottom back left 16
		0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, // bottom back right 17
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, // bottom front right 18
	-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, // bottom front left 19

	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, // top back left 20
		0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, // top back right 21
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, // top front right 22
	-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, // top front left 23
};


	unsigned int indices[] =
	{
		3, 2, 1, // top plane
		1, 0, 3,
		4, 5, 6, // bottom plane
		6, 7, 4,
		11, 8, 9,	// front plane
		9, 10, 11,
		12, 13, 14, // right plane
		14, 15, 12,
		16, 17, 18, // back plane
		18, 19, 16,
		20, 21, 22, // left plane
		22, 23, 20
	};

	unsigned int diffuseIndices[] =
	{
		20, 23, 22, // top plane
		22, 21, 20,
		19, 16, 17, // bottom plane
		17, 18, 19,
		7, 4, 5,	// front plane
		5, 6, 7,
		12, 15, 14, // right plane
		14, 13, 12,
		2, 1, 0, // back plane
		0, 3, 2,
		13, 14, 15, // left plane
		15, 12, 13
	};


	GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
	GLCall(glEnable(GL_BLEND));

	// GL data structures used to render the lamp cube
	IndexBuffer ib(diffuseIndices, 36);
	VertexArray va;
	VertexBuffer vb(diffuseVertices, sizeof(float) * 6 * 24);
	VertexBufferLayout layout;
	layout.Push<float>(3);
	// The size of each vertex is still 3 position floats + 3 normal vector floats
	// our lamp will not use the normal vectors. By updating the stride but not adding a new
	// layout, the VertexArray object will not add an attrib pointer for the normal vectors
	// when binding the VertexBuffer, but will still set the correct vertex size when setting
	// the vertex attrib pointer for the position vectors.
	layout.AddToStride(sizeof(float) * 3);
	va.AddBuffer(vb, layout);

	//GL data structures used to render the lamp cube
	IndexBuffer diffuseIb(diffuseIndices, 36);
	VertexArray diffuseVa;
	// VertexBuffer diffuseVb(diffuseVertices, sizeof(float) * 6 * 24);
	VertexBufferLayout diffuseLayout;
	diffuseLayout.Push<float>(3);
	diffuseLayout.Push<float>(3);
	diffuseVa.AddBuffer(vb, diffuseLayout);

	

	//glm::mat4 proj = glm::ortho(-2.0f, 2.0f, -1.5f, 1.5f, -1.0f, 1.0f);
	//glm::mat4 proj = glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f);
	//glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(-100, 0, 0));
	// TODO: Better understand what glViewport does
	glViewport(0, 0, screenWidth, screenHeight);

	// binds our shader
	//Shader shader("res/shaders/Basic.shader");
	// shader.Bind();

	//Texture texture("res/images/starlight.png");
	//texture.Bind();
	//// Specify the slot of the texture that we want to use in our shader program
	//shader.SetUniform1i("u_Texture", 0);
	////shader.SetUniform3f("lightColor", 1.0f, 1.0f, 1.0f);
	//shader.Unbind();
	
	// Uniforms are differentiated by how many items they contain and what type of items they contain.
	// 4f: 4 floats
	// GLCall(glUniform4f(location, 0.5f, 0.0f, 0.5f, 1.0f));
	/* Loop until the user closes the window */
	float red = 1.0f;
	float green = 0.3f;
	float blue = 0.8f;
	float inc = 0.05f;

	// Represents the source coordinate of our lamp's light
	glm::vec3 lightPos(1.0, 1.0, -3.0);

	GLCall(Shader lightingShader("res/shaders/Lighting.shader"));
	lightingShader.Bind();
	lightingShader.SetUniform3f("lightColor", 1.0f, 1.0f, 1.0f);
	lightingShader.SetUniform3f("objectColor", 1.0f, 0.0f, 1.0f);
	lightingShader.SetUniform3f("lightPos", lightPos.x, lightPos.y, lightPos.z);
	lightingShader.Unbind();

	GLCall(Shader lampShader("res/shaders/Lamp.shader"));
	lampShader.Bind();
	lampShader.SetUniform3f("lightColor", 1.0f, 1.0f, 1.0f);
	lampShader.SetUniform3f("lightPos", lightPos.x, lightPos.y, lightPos.z);
	lampShader.Unbind();

	Renderer renderer;

	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;

	ImGui::StyleColorsDark();

	// Setup Platform/Renderer bindings
	const char* glsl_version = "#version 150";
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init(glsl_version);

	bool show_demo_window = true;
	bool show_another_window = false;
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
	// Enable z-buffer depth testing
	glEnable(GL_DEPTH_TEST);

	Camera::CreateCameraContext(window);
	std::shared_ptr<Camera> Cam(Camera::CreateCamera(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f)));

	Model m("res/models/zero-suit/source/DolSzerosuitR1.obj");

	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		renderer.Clear();

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		
		Cam->ProcessInput();
		
		static float xDelta = 0.0f;
		static float yDelta = 0.0f;
		static float r = 0.0f;

		// 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
		{
			static int counter = 0;
			ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

			ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
			ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
			ImGui::Checkbox("Another Window", &show_another_window);

			ImGui::SliderFloat("floatX", &xDelta, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
			ImGui::ColorEdit3("clear color", (float*)& clear_color); // Edit 3 floats representing a color
			ImGui::SliderFloat("floatY", &yDelta, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
			ImGui::SliderFloat("rotate", &r, 1.0f, 360.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
			ImGui::End();
		}

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
		//glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(xDelta * screenWidth, yDelta * screenHeight, 0));
		//model = glm::rotate(glm::mat4(1.0f), glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		/*model = glm::translate(model, glm::vec3(500.0f, 250.0f, 0.0f));*/

		//model = glm::translate(model, offset);
		//model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));
		//model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));

		//model = glm::translate(model, glm::vec3(0.0, 0.0, -3.0));

		glm::mat4 view = glm::mat4(1.0f);
		Cam->ApplyViewMatrix(view);

		glm::mat4 projection = glm::mat4(1.0f);
		projection = glm::perspective(glm::radians(45.0f), (float)screenWidth / (float)screenHeight, 0.1f, 100.0f);


		glm::mat4 mvp = projection * view * model;

		lightingShader.Bind();
		lightingShader.SetUniformMat4f("model", model);
		lightingShader.SetUniformMat4f("view", view);
		lightingShader.SetUniformMat4f("projection", projection);
		lightingShader.SetUniformMat4f("u_MVP", mvp);
		renderer.Draw(diffuseVa, 36, diffuseIb, lightingShader);
		lightingShader.Unbind();

		model = glm::translate(model, lightPos);
		model = glm::scale(model, glm::vec3(0.5, 0.5, 0.5));
		lampShader.Bind();
		lampShader.SetUniformMat4f("model", model);
		lampShader.SetUniformMat4f("view", view);
		lampShader.SetUniformMat4f("projection", projection);
		lampShader.SetUniformMat4f("u_MVP", mvp);
		renderer.Draw(va, 36, ib, lampShader);
		lampShader.Unbind();


		
		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	// Cleanup
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwTerminate();
	return 0;
}