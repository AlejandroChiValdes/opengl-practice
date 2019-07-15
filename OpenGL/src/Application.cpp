#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

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
constexpr auto screenWidth = 540;;

void shiftX(float newPositionOffset, float* positions, float* raw_positions)
{
	int offset = 4;
	for (int i = 0; i < 4; ++i)
	{
		positions[i * offset] = raw_positions[i * offset] + screenWidth * newPositionOffset;
	}
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


	//float positions[] =
	//{
	//	-0.5f, -0.5f,// 0
	//	 0.5f, -0.5f,// 1
	//	 0.5f, 0.5f,// 2
	//	 -0.5f, 0.5f// 3
	//};

	//float positions[] =
	//{
	//	-0.5f, -0.5f, 0.0f, 0.0f,// 0
	//	 0.5f, -0.5f, 1.0f, 0.0f,// 1
	//	 0.5f, 0.5f, 1.0f, 1.0f,// 2
	//	 -0.5f, 0.5f, 0.0f, 1.0f// 3
	//};

	float raw_positions[] =
	{
		100.0f, 100.0f, 0.0f, 0.0f,
		200.0f, 100.0f, 1.0f, 0.0f,
		200.0f, 200.0f, 1.0f, 1.0f,
		100.0f, 200.0f, 0.0f, 1.0f
	};

	float positions[] =
	{
		100.0f, 100.0f, 0.0f, 0.0f,
		300.0f, 100.0f, 1.0f, 0.0f,
		300.0f, 200.0f, 1.0f, 1.0f,
		100.0f, 200.0f, 0.0f, 1.0f
	};

	unsigned int indices[] =
	{
		0, 1, 2,
		2, 3, 0
	};

	GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
	GLCall(glEnable(GL_BLEND));

	VertexArray va;

	VertexBuffer vb(positions, sizeof(float) * 4 * 4);

	VertexBufferLayout layout;
	layout.Push<float>(2);
	layout.Push<float>(2);
	va.AddBuffer(vb, layout);

	IndexBuffer ib(indices, 6);

	/*glm::mat4 proj = glm::ortho(-2.0f, 2.0f, -1.5f, 1.5f, -1.0f, 1.0f);*/
	glm::mat4 proj = glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f);
	glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(-100, 0, 0));

	// binds our shader
	Shader shader("res/shaders/Basic.shader");
	shader.Bind();

	Texture texture("res/images/starlight.png");
	texture.Bind();
	// Specify the slot of the texture that we want to use in our shader program
	shader.SetUniform1i("u_Texture", 0);
	
	
	// Uniforms are differentiated by how many items they contain and what type of items they contain.
	// 4f: 4 floats
	// GLCall(glUniform4f(location, 0.5f, 0.0f, 0.5f, 1.0f));
	/* Loop until the user closes the window */
	float red = 1.0f;
	float green = 0.3f;
	float blue = 0.8f;
	float inc = 0.05f;

	va.Unbind();
	vb.Unbind();
	ib.Unbind();
	shader.Unbind();

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



	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		renderer.Clear();

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();



		shader.Bind();
		shader.SetUniform4f("u_Color", red, green, blue, 1.0);
		


		static float xDelta = 0.0f;
		static float yDelta = 0.0f;

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

			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
			ImGui::End();
		}

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		//shiftX(f, positions, raw_positions);
		glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(xDelta * screenWidth, yDelta * screenHeight, 0));
		glm::mat4 mvp = proj * view * model;
		shader.SetUniformMat4f("u_MVP", mvp);
		renderer.Draw(va, ib, shader);

		for (int i = 0; i < 16; ++i)
			std::cout << positions[i] << " ";
		std::cout << std::endl;

		
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