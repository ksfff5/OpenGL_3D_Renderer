#ifndef RENDERABLE_H
#define RENDERABLE_H

#include <string>
#include <iostream>
#include <fstream>
#include <vector>

//TODO:
//- Textures

//Note you should not change the size of the object data (data or index) after creation

class Renderable
{
	private:
		//The object data required for rendering
		int vSize;
		float* verticies;
		int iSize;
		unsigned int* indicies;

		//The location of the object in space
		float xPos;
		float yPos;
		float zPos;

		//The roration of the object in space
		float xRot;
		float yRot;
		float zRot;

		//Whether or not this object's data has been modified after creation
		bool dirty;

	public:
		//Constructor that takes in the positional data as well as the data needed to describe the object
		Renderable(float* v, int vSize, unsigned int* i, int iSize, float x, float y, float z, float xr, float yr, float zr);

		//Constructor that takes in the positonal data as well as a file path to a .OBJ file that describes the object
		//Renderable(std::string filePath, float x, float y, float z);

		//Destructor
		~Renderable();

		//Getter for vSize
		int getVSize();

		//Getter for the verticies data
		float* getVerticies();

		//Getter for iSize
		int getISize();

		//Getter for the indicies data
		unsigned int* getIndicies();

		//Getter for the Position variables
		float getXPos();
		float getYPos();
		float getZPos();

		//Setter for the Position variables
		void setXPos(float x);
		void setYPos(float y);
		void setZPos(float z);

		//Getter for the Rotation variables
		float getXRot();
		float getYRot();
		float getZRot();

		//Setter for the Rotation variables
		void setXRot(float x);
		void setYRot(float y);
		void setZRot(float z);

		//Getter for the Dirty variable
		bool isDirty();

		//Setter for the Dirty variable
		void setDirty(bool d);
};

#endif