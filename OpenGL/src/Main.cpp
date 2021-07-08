#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include "Shader.h"
#include "Camera.h"
#include "Renderable.h"
#include "SceneCamera3D.h"
#include "stb_image.h"
#include <iostream>
#include <string>
#include <cmath>

//Create Camera. Must be global so that both main and keycallback can access it
Camera myCamera(0, 0, -5, 0, 0, 45, .1, 10);
Camera dummyCamera(0, 2, -5, 0, 20, 45, .1, 10);

//Create variable to determine if we should continue movement foward and backward. Used to prevent the moving then stopping before GLFW realizes we are holding the key
float forward = 0;
float right = 0;

//Variable to control which scene we are rendering (used for testing)
int sceneControl = 0;

//Variable to control moving objects (used for testing)
float move = 0;
float angle = 0;

void error_callback(int error, const char* description)
{
	std::cout << "Error: " << error << "; " << description << std::endl;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	//Press "escape" to exit window
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	}

	//Press "spacebar" to switch between opaque and wireframe rendering
	if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
	{
		int pMode[2];
		glGetIntegerv(GL_POLYGON_MODE, pMode);
		if (pMode[0] == GL_FILL && pMode[1] == GL_FILL)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}
		else if(pMode[0] == GL_LINE && pMode[1] == GL_LINE)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}
	}

	//Press "WASD" to move
	if (key == GLFW_KEY_A && action == GLFW_PRESS)
	{
		right = -.1;
	}
	if (key == GLFW_KEY_D && action == GLFW_PRESS)
	{
		right = .1;
	}
	if (key == GLFW_KEY_W && action == GLFW_PRESS)
	{
		forward = .1;
	}
	if (key == GLFW_KEY_S && action == GLFW_PRESS)
	{
		forward = -.1;
	}

	//Realease "WASD" to stop moving
	if (key == GLFW_KEY_A && action == GLFW_RELEASE)
	{
		right = 0;
	}
	if (key == GLFW_KEY_D && action == GLFW_RELEASE)
	{
		right = 0;
	}
	if (key == GLFW_KEY_W && action == GLFW_RELEASE)
	{
		forward = 0;
	}
	if (key == GLFW_KEY_S && action == GLFW_RELEASE)
	{
		forward = 0;
	}

	if (key == GLFW_KEY_ENTER && action == GLFW_PRESS)
	{
		if (sceneControl == 0)
		{
			sceneControl = 1;
		}
		else
		{
			sceneControl = 0;
		}
	}

	if (key == GLFW_KEY_BACKSPACE && action == GLFW_PRESS)
	{
		move = .1;
	}
	if (key == GLFW_KEY_TAB && action == GLFW_PRESS)
	{
		move = -.1;
	}
	if (key == GLFW_KEY_BACKSPACE && action == GLFW_RELEASE)
	{
		move = 0;
	}
	if (key == GLFW_KEY_TAB && action == GLFW_RELEASE)
	{
		move = 0;
	}

	if (key == GLFW_KEY_Z && action == GLFW_PRESS)
	{
		angle++;
		if (angle > 5) {
			angle = 0;
		}
		std::cout << "angle" << angle << std::endl;
	}
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	//std::cout << yoffset << std::endl;

	//User scrolls away from self
	if (yoffset == 1)
	{
		myCamera.setFOV(myCamera.getFOV() - 1);
	}
	//User scrolls towards self
	else if (yoffset == -1)
	{
		myCamera.setFOV(myCamera.getFOV() + 1);
	}

	std::cout << "fov: " << myCamera.getFOV() << std::endl;
}

void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
	//std::cout << xpos << " " << ypos << std::endl;
	myCamera.setXRot(xpos*2);
	myCamera.setYRot(ypos/2);
	std::cout << "xRot: " << myCamera.getXRot() << " yRot: " << myCamera.getYRot() << std::endl;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

int main()
{
	//Initialize glfw/window stuff
	glfwSetErrorCallback(error_callback);

	if (!glfwInit())
	{
		std::cout << "glfw not able to be initialized" << std::endl;
		return -1;
	}

	GLFWwindow* window = glfwCreateWindow(640, 640, "Test", NULL, NULL);
	if (window == NULL)
	{
		glfwTerminate();
		std::cout << "window failed to be created" << std::endl;
		return -1;
	}

	glfwSetKeyCallback(window, key_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetCursorPosCallback(window, cursor_position_callback);
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);

	if (glewInit() != GLEW_OK)
	{
		glfwTerminate();
		std::cout << "glew not able to be initialized" << std::endl;
		return -1;
	}

	//setup viewport
	glViewport(0, 0, 640, 640);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	//Setup cursor so that it has unlimited movement
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	//Done with initialization



	//Create some data for a 3D cube
	float positions[] =
	{
		//Positions        //color             //texture coords
		-0.5, -0.5, -0.5,   0.0, 0.0, 0.0,
		-0.5, 0.5, -0.5,    0.0, 0.0, 0.0,
		 0.5, -0.5, -0.5,   0.0, 0.0, 0.0,
		 0.5, 0.5, -0.5,    0.0, 0.0, 0.0,

		-0.5, -0.5, 0.5,   0.0, 0.0, 0.0,
		-0.5, 0.5, 0.5,    0.0, 0.0, 0.0,
		 0.5, -0.5, 0.5,   0.0, 0.0, 0.0,
		 0.5, 0.5, 0.5,    0.0, 0.0, 0.0
	};
	
	//Create the element data
	unsigned int indicies[]
	{
		0, 1, 2,	//Front face
		1, 2, 3,

		4, 5, 6,	//Back face
		5, 6, 7,

		1, 3, 5,	//Top face
		3, 5, 7,

		0, 2, 4,	//Bottom face
		2, 4, 6,

		2, 3, 6,	//Right face
		3, 6, 7,

		0, 1, 4,	//Left face
		1, 4, 5,
	};

	//-------BUFFER STUFF:
	/*unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	//Create buffer where we store our data for objects to be rendered. buffer var becomes the id
	unsigned int buffer;
	glGenBuffers(1, &buffer);

	//Select this new buffer as the active one to be used when drawing later
	glBindBuffer(GL_ARRAY_BUFFER, buffer);

	//Specify the actual size and use case of the buffer, and put the data into it
	glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_DYNAMIC_DRAW);

	//Enable the attributes from the previous and specify the layout of the data in the buffer so OpenGL knows what it means
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (void*)0);
	glEnableVertexAttribArray(0);

	//color
	//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)(sizeof(float) * 3));
	//glEnableVertexAttribArray(1);

	//textures
	//glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)(sizeof(float) * 6));
	//glEnableVertexAttribArray(2);

	//Create an element buffer
	unsigned int EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicies), indicies, GL_STATIC_DRAW);

	//texture stuff:
	//Get texture data
	/*stbi_set_flip_vertically_on_load(true);
	int width;
	int heigth;
	int noChannels;
	unsigned char *data = stbi_load("C:\\dev\\OpenGL\\OpenGL\\src\\grass_side.png", &width, &heigth, &noChannels, 4);

	//Create texture
	unsigned int texture;
	glGenTextures(1, &texture);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	//Set the upscaling and downscaling texture filtering to Nearest (it will chose the closest pixel color when sampling the texture. It is not an interpolation)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	//give texture data
	if (data != NULL)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, heigth, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Texture image failed to load: " << stbi_failure_reason() << std::endl;
	}

	//free texture data
	stbi_image_free(data);*/

	//Create Shader
	/*Shader myShader("C:\\dev\\OpenGL\\OpenGL\\src\\vertex_shader.txt", "C:\\dev\\OpenGL\\OpenGL\\src\\fragment_shader.txt");
	myShader.Use();
	//glUniform1i(glGetUniformLocation(myShader.GetProgramID(), "ourTexture"), 0);

	//initialize values of the camera related uniforms
	myShader.setUniformFloat("screenDistance", myCamera.getScreenDistance());
	myShader.setUniformFloat("minDistanceOffset", myCamera.getMinDistanceOffset());
	myShader.setUniformFloat("length", myCamera.getLength());
	myShader.setUniformFloat("cameraX", myCamera.getXPos());
	myShader.setUniformFloat("cameraY", myCamera.getYPos());
	myShader.setUniformFloat("cameraZ", myCamera.getZPos());
	myShader.setUniformFloat("cameraXRot", (myCamera.getXRot()* ((4 * atan(1)) / 180)));
	myShader.setUniformFloat("cameraYRot", (myCamera.getYRot()* ((4 * atan(1)) / 180)));*/
	
	SceneCamera3D scene = SceneCamera3D(&myCamera);
	Renderable cube1 = Renderable(positions, 48, indicies, 36, 0, 0, 0, 0, 0, 0);
	Renderable cube2 = Renderable(positions, 48, indicies, 36, -1, 1, 0, 90, 0, 0);
	Renderable cube3 = Renderable(positions, 48, indicies, 36, 0, -1, 1, 0, 0, 0);
	scene.addObject(&cube1);
	scene.addObject(&cube2);
	scene.addObject(&cube3);

	SceneCamera3D scene3 = SceneCamera3D(&dummyCamera);
	Renderable cube4 = Renderable(positions, 48, indicies, 36, 0, 0, 0, 0, 0, 0);
	scene3.addObject(&cube4);

	//print out the OpenGL version to console
	std::cout << glGetString(GL_VERSION) << std::endl;

	//------------Main loop:
	while (!glfwWindowShouldClose(window))
	{
		//Clear color buffer
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		
		//if (sceneControl == 0)
		//{
			scene.render();
		//}
		if (sceneControl == 1)
		{
			scene3.render();
		}

		if (move != 0)
		{
			if (angle == 0)
			{
				cube1.setXRot(cube1.getXRot() + move * 10);
			}
			else if (angle == 1)
			{
				cube1.setYRot(cube1.getYRot() + move * 10);
			}
			else if (angle == 2)
			{
				cube1.setZRot(cube1.getZRot() + move * 10);
			}
			else if (angle == 3)
			{
				cube1.setXPos(cube1.getXPos() + move);
			}
			else if (angle == 4)
			{
				cube1.setYPos(cube1.getYPos() + move);
			}
			else if (angle == 5)
			{
				cube1.setZPos(cube1.getZPos() + move);
			}
			std::cout << "xrot: " << cube1.getXRot() << std::endl;
			std::cout << "yrot: " << cube1.getYRot() << std::endl;
			std::cout << "zrot: " << cube1.getZRot() << std::endl;
		}

		//Draw triangles from the selected buffer using the selected shader
		//glDrawArrays(GL_TRIANGLES, 0, 3);
		//glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

		//move camera in a circle
		//myCamera.setXPos(2 * sin(glfwGetTime()));
		//myCamera.setYPos(2 * cos(glfwGetTime()));

		//update camera uniforms
		/*myShader.setUniformFloat("screenDistance", myCamera.getScreenDistance());
		myShader.setUniformFloat("minDistanceOffset", myCamera.getMinDistanceOffset());
		myShader.setUniformFloat("length", myCamera.getLength());
		myShader.setUniformFloat("cameraX", myCamera.getXPos());
		myShader.setUniformFloat("cameraY", myCamera.getYPos());
		myShader.setUniformFloat("cameraZ", myCamera.getZPos());
		myShader.setUniformFloat("cameraXRot", (myCamera.getXRot()* ((4 * atan(1))/180) ));
		myShader.setUniformFloat("cameraYRot", (myCamera.getYRot() * ((4 * atan(1)) / 180)));*/
		
		//Move camera
		myCamera.moveForward(forward);
		myCamera.moveRight(right);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	
	//Cleanup gl memory usage
	//glDeleteBuffers(1, &buffer);
	//glDeleteBuffers(1, &EBO);
	//glDeleteVertexArrays(1, &VAO);
	//glDeleteTextures(1, &texture);

	//Cleanup glfw/window stuff
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}