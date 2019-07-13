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


int main(void)
{
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
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

	float positions[] =
	{
		100.0f, 100.0f, 0.0f, 0.0f,
		200.0f, 100.0f, 1.0f, 0.0f,
		200.0f, 200.0f, 1.0f, 1.0f,
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
	glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(200, 200, 0));

	glm::mat4 mvp = proj * view * model;
	// binds our shader
	Shader shader("res/shaders/Basic.shader");
	shader.Bind();

	Texture texture("res/images/starlight.png");
	texture.Bind();
	// Specify the slot of the texture that we want to use in our shader program
	shader.SetUniform1i("u_Texture", 0);
	shader.SetUniformMat4f("u_MVP", mvp);
	
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

	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		renderer.Clear();
		shader.Bind();
		shader.SetUniform4f("u_Color", red, green, blue, 1.0);
		renderer.Draw(va, ib, shader);
		
		//inc = red <= 0.0f ? 0.5 : 
		//	  red >= 1.0f ? -0.05f : 
		//	  inc;
		//red = red + inc;
		// std::cout << "red: " << red << std::endl;
		// blue = blue < 0.05f ? blue + inc : blue - inc;
		//green = green > 0.95f ? green - inc : green + inc;
		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}