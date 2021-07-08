#ifndef SCENECAMERA3D_H
#define SCENECAMERA3D_H

#include <GL\glew.h>
#include "Shader.h"
#include "Camera.h"
#include "Renderable.h"
#include <vector>

//TODO:
//- Fix wonkyness with having same renderable object in multiple scenes
//- Textures

//When adding more renderable types to this class:
//- Add required variables
//- Update constructor to initialize those variables
//- Update destructor to destroy those variables
//- Update addObject function
//- Update createBuffers function
//- Update render function

//A class to hold/handle information about 3D scenes that use a camera that can be rendered
class SceneCamera3D
{
	private:
		//A pointer to the camera object
		Camera* camera = nullptr;

		//The OpenGL objects needed to render our camera based renderables
		unsigned int VAO;
		unsigned int dataBuffer;
		unsigned int EBO;

		//The shader object that cooresponds to this scene
		Shader cameraShader = Shader("C:\\dev\\OpenGL\\OpenGL\\src\\vertex_shader.txt", "C:\\dev\\OpenGL\\OpenGL\\src\\fragment_shader.txt");

		//A list of renderables that use the camera
		std::vector<Renderable*> renderables;

		//The amount of verticies to be rendered each render call (should be the same as the number of items in all of the indicies arrays in the renderables vector)
		int numVerticies;

		//Whether or not somthing has been added to the renderables vector without it being added to the OpenGL dataBuffer and EBO buffers. In other words this will be true if we need to update those buffers.
		bool dirty;

		//Creates or updates all the buffers for all the types of renderables (if necessary)
		void updateBuffers();

		//Creates the corresponding databuffer with the cooresponding data
		void createDataBuffer(unsigned int buffer, std::vector<Renderable*> r);

		//Creates the corresponding indexbuffer with the cooresponding data
		void createIndexBuffer(unsigned int buffer, std::vector<Renderable*> r);

		//Updates the databuffer at the cooresponding location with the data from the cooresponding renderable
		void updateDataBuffer(int location, Renderable* renderable);

	public:
		//Constructor for the SceneCamera3D object that takes in a pointer to the camera object that is to be used for rendering
		SceneCamera3D(Camera* c);

		//Destructor for the SceneCamera3D object
		~SceneCamera3D();

		//Adds an object to the Scene
		void addObject(Renderable* r);

		//Render all objects in this scene
		void render();
};

#endif