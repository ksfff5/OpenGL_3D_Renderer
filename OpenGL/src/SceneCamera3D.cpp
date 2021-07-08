#include "SceneCamera3D.h"

//Constructor for the SceneCamera3D object that takes in a pointer to the camera object that is to be used for rendering
SceneCamera3D::SceneCamera3D(Camera* c)
{
	camera = c;

	//Setup the VAO
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	//Setup the dataBuffer buffer
	glGenBuffers(1, &dataBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, dataBuffer);
	glBufferData(GL_ARRAY_BUFFER, 0, NULL, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (void*)0);
	glEnableVertexAttribArray(0);

	//Setup the EBO
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 0, NULL, GL_DYNAMIC_DRAW);

	//Use the camera shader with the camera VAO
	cameraShader.Use();

	//initialize values of the camera related uniforms
	cameraShader.setUniformFloat("screenDistance", camera->getScreenDistance());
	cameraShader.setUniformFloat("minDistanceOffset", camera->getMinDistanceOffset());
	cameraShader.setUniformFloat("length", camera->getLength());
	cameraShader.setUniformFloat("cameraX", camera->getXPos());
	cameraShader.setUniformFloat("cameraY", camera->getYPos());
	cameraShader.setUniformFloat("cameraZ", camera->getZPos());
	cameraShader.setUniformFloat("cameraXRot", (camera->getXRot() * ((4 * atan(1)) / 180)));
	cameraShader.setUniformFloat("cameraYRot", (camera->getYRot() * ((4 * atan(1)) / 180)));

	//Unbind this VAO
	glBindVertexArray(0);

	numVerticies = 0;

	//Set dirty to true so that the buffers will be created when the scene is first rendered
	dirty = true;
}

//Destructor for the SceneCamera3D object
SceneCamera3D::~SceneCamera3D()
{
	//Delete camera Buffers
	glDeleteBuffers(1, &dataBuffer);
	glDeleteBuffers(1, &EBO);
	glDeleteVertexArrays(1, &VAO);
}

//Updates all the buffers for all the types of renderables
void SceneCamera3D::updateBuffers()
{
	//If the scene is dirty
	if (dirty)
	{
		//recreate the buffers
		createDataBuffer(dataBuffer, renderables);
		createIndexBuffer(EBO, renderables);
		
		//Reset the dirty bit
		dirty = false;

		//Reset the dirty bits of all the renderables as they were just recreated from scratch, so there is no point to updating them
		for (int i = 0; i < renderables.size(); i++)
		{
			renderables[i]->setDirty(false);
		}
	}
	//If the scene wasn't dirty
	else
	{
		//A variable to keep track of the location in the data buffer that the i renderable begins at
		int location = 0;

		//Loop through all the renderables
		for (int i = 0; i < renderables.size(); i++)
		{
			if (renderables[i]->isDirty())
			{
				//Update any dirty renderables
				updateDataBuffer(location, renderables[i]);

				//reset their dirty bit
				renderables[i]->setDirty(false);
			}

			//Update location
			location += renderables[i]->getVSize();
		}
	}
}

//Updates the corresponding databuffer with the cooresponding data
void SceneCamera3D::createDataBuffer(unsigned int buffer, std::vector<Renderable*> r)
{
	//Create a vector to hold  all the vertex data from all the renderables in r
	std::vector<float> verticies;

	//Loop through all the renderables in r
	for (int i = 0; i < r.size(); i++)
	{
		//Loop through the vertex data of i
		for (int j = 0; j < r[i]->getVSize(); j += 6)
		{
			//Grab the object data
			float x0 = r[i]->getVerticies()[j];
			float y0 = r[i]->getVerticies()[j + 1];
			float z0 = r[i]->getVerticies()[j + 2];

			//Add Position data:
			verticies.push_back(x0);
			verticies.push_back(y0);
			verticies.push_back(z0);

			//Add Extra currently unused Data:
			verticies.push_back(r[i]->getVerticies()[j+3]);
			verticies.push_back(r[i]->getVerticies()[j+4]);
			verticies.push_back(r[i]->getVerticies()[j+5]);
		}
	}

	//An array to hold the same data as the verticies vector
	float* verticies2 = new float[verticies.size()];

	//Translate the verticies vector into an array
	for (int i = 0; i < verticies.size(); i++)
	{
		verticies2[i] = verticies[i];
	}

	//Put the data into the OpenGL buffers
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * verticies.size(), verticies2, GL_DYNAMIC_DRAW);

	//delete the dynamically allocated verticies2
	delete[] verticies2;
}

//Updates the corresponding indexbuffer with the cooresponding data
void SceneCamera3D::createIndexBuffer(unsigned int buffer, std::vector<Renderable*> r)
{
	//Create a vector to hold  all the index data from all the renderables in r
	std::vector<unsigned int> indicies;

	//Loop through all the renderables in r
	for (int i = 0; i < r.size(); i++)
	{
		//Loop through all precedding renderables to calculate offset that is caused by the concatination of vertex data
		int offset = 0;
		for (int j = 0; j < i; j++)
		{
			offset += (r[j]->getVSize() / 6);
		}

		//Loop through the index data of r
		for (int j = 0; j < r[i]->getISize(); j++)
		{
			indicies.push_back(r[i]->getIndicies()[j] + offset);
		}
	}

	//An array to hold the same data as the verticies vector
	unsigned int* indicies2 = new unsigned int[indicies.size()];

	//Translate the verticies vector into an array
	for (int i = 0; i < indicies.size(); i++)
	{
		indicies2[i] = indicies[i];
	}

	//Put the data into the OpenGL buffers
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indicies.size(), indicies2, GL_DYNAMIC_DRAW);

	//delete the dynamically allocated indicies2
	delete[] indicies2;

	//Update numVerticies to the correct number of verticies
	numVerticies = indicies.size();
}

//Updates the databuffer at the cooresponding location with the data from the cooresponding renderable
void SceneCamera3D::updateDataBuffer(int location, Renderable* renderable)
{
	//Create an array of floats to hold the vertex data from the renderable
	float* verticies = new float[renderable->getVSize()];

	//Copy the data into verticies, but add in the x, y, and z positional values
	for (int i = 0; i < renderable->getVSize(); i += 6)
	{
		//Grab the object data
		float x0 = renderable->getVerticies()[i];
		float y0 = renderable->getVerticies()[i + 1];
		float z0 = renderable->getVerticies()[i + 2];

		//Add Position data:
		verticies[i] = x0;
		verticies[i+1] = y0;
		verticies[i+2] = z0;
		verticies[i+3] = renderable->getVerticies()[i+3];
		verticies[i+4] = renderable->getVerticies()[i+4];
		verticies[i+5] = renderable->getVerticies()[i+5];
	}

	//Update the databuffer
	glBindBuffer(GL_ARRAY_BUFFER, dataBuffer);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(float) * location, sizeof(float) * renderable->getVSize(), verticies);

	//Delete our array
	delete[] verticies;
}

//Adds an object to the Scene
void SceneCamera3D::addObject(Renderable* r)
{
	renderables.push_back(r);
	dirty = true;
}

//Render all objects in this scene
void SceneCamera3D::render()
{
	//Update any buffers that need it
	updateBuffers();

	//update camera uniforms
	cameraShader.Use();
	cameraShader.setUniformFloat("screenDistance", camera->getScreenDistance());
	cameraShader.setUniformFloat("minDistanceOffset", camera->getMinDistanceOffset());
	cameraShader.setUniformFloat("length", camera->getLength());
	cameraShader.setUniformFloat("cameraX", camera->getXPos());
	cameraShader.setUniformFloat("cameraY", camera->getYPos());
	cameraShader.setUniformFloat("cameraZ", camera->getZPos());
	cameraShader.setUniformFloat("cameraXRot", (camera->getXRot() * ((4 * atan(1)) / 180)));
	cameraShader.setUniformFloat("cameraYRot", (camera->getYRot() * ((4 * atan(1)) / 180)));
	
	//Render camera objects
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, numVerticies, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}