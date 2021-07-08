#include "Renderable.h"

//Constructor that takes in the positional data as well as the data needed to describe the object
Renderable::Renderable(float* v, int vs, unsigned int* i, int is, float x, float y, float z, float xr, float yr, float zr)
{
	vSize = vs;
	iSize = is;

	verticies = new float[vSize];
	for (int j = 0; j < vSize; j++)
	{
		verticies[j] = v[j];
	}

	indicies = new unsigned int[iSize];
	for (int j = 0; j < iSize; j++)
	{
		indicies[j] = i[j];
	}

	setXPos(x);
	setYPos(y);
	setZPos(z);

	setXRot(xr);
	setYRot(yr);
	setZRot(zr);

	dirty = false;
}

//Constructor that takes in the positonal data as well as a file path to a .OBJ file that describes the object
/*Renderable::Renderable(std::string filePath, float x, float y, float z)
{
	//Open the file
	std::ifstream fileIn(filePath.c_str());

	//If the file failed to open print out an error message
	if (!fileIn.is_open())
	{
		std::cout << "ERROR opening file" << std::endl;
	}

	//A variable to store data from the file
	char data[256];

	//a variable to store the positional data
	std::vector<float> positions;

	//Loop through the file and extract all necessary data
	while (!fileIn.eof())
	{
		fileIn.getline(data, 256);

		//If this line is vertex data
		if (data[0] == 'v')
		{
			//The start of the current word
			int start = 2;

			//Loop through the line and grab the x, y, and z data
			for (int i = 2; data[i] != '\0' && i < 256; i++)
			{
				if (data[i] == ' ')
				{
					std::string p;
					for (int j = start; j < i; j++)
					{
						p += data[j];
					}
					positions.push_back(std::stod(p));

					start = i + 1;
				}
			}
		}
		//Else if this line is 
		else if ()
		{

		}
	}

}
*/

//Destructor
Renderable::~Renderable()
{
	delete[] verticies;
	delete[] indicies;
}

//Getter for vSize
int Renderable::getVSize()
{
	return vSize;
}

//Getter for the verticies data
float* Renderable::getVerticies()
{
	return verticies;
}

//Getter for iSize
int Renderable::getISize()
{
	return iSize;
}

//Getter for the indicies data
unsigned int* Renderable::getIndicies()
{
	return indicies;
}

//Getter for the Position variables
float Renderable::getXPos()
{
	return xPos;
}

float Renderable::getYPos()
{
	return yPos;
}

float Renderable::getZPos()
{
	return zPos;
}

//Setter for the Position variables
void Renderable::setXPos(float x)
{
	//Move the object and store the info back into verticies
	for (int i = 0; i < vSize; i += 6)
	{
		//Add the amount changed to the x position
		verticies[i] += (x - xPos);
	}

	//Update the x position. This must be done after it has been moved
	xPos = x;

	//Set the dirty bit so the scene knows this object has changed
	dirty = true;
}

void Renderable::setYPos(float y)
{
	//Move the object and store the info back into verticies
	for (int i = 0; i < vSize; i += 6)
	{
		//Add the amount changed to the y position
		verticies[i + 1] += (y - yPos);
	}

	//Update the y position. This must be done after it has been moved
	yPos = y;

	//Set the dirty bit so the scene knows this object has changed
	dirty = true;
}

void Renderable::setZPos(float z)
{
	//Move the object and store the info back into verticies
	for (int i = 0; i < vSize; i += 6)
	{
		//Add the amount changed to the z position
		verticies[i + 2] += (z - zPos);
	}

	//Update the z position. This must be done after it has been moved
	zPos = z;

	//Set the dirty bit so the scene knows this object has changed
	dirty = true;
}

//Getter for the Rotation variables
float Renderable::getXRot()
{
	return xRot;
}

float Renderable::getYRot()
{
	return yRot;
}

float Renderable::getZRot()
{
	return zRot;
}

//Setter for the Rotation variables
void Renderable::setXRot(float x)
{
	//Bounds check the new rotation value
	while (x >= 360)
	{
		x = x - 360;
	}
	while (x < 0)
	{
		x = x + 360;
	}

	//Roate the object and store the data back into verticies
	for (int i = 0; i < vSize; i += 6)
	{
		//Grab the object model data (subtract away the positional data)
		float x0 = verticies[i] - xPos;
		float y0 = verticies[i + 1] - yPos;
		float z0 = verticies[i + 2] - zPos;

		//Rotate along y axis (left/right) by however much the rotation value changed
		float x1 = (cos(-1.0 * (x - xRot) * ((4 * atan(1)) / 180)) * x0) + (sin(-1.0 * (x - xRot) * ((4 * atan(1)) / 180)) * z0);
		float y1 = y0;
		float z1 = (sin(-1.0 * (x - xRot) * ((4 * atan(1)) / 180)) * -1.0 * x0) + (cos(-1.0 * (x - xRot) * ((4 * atan(1)) / 180)) * z0);

		//Add Position data and store the data in verticies
		verticies[i] = x1 + xPos;
		verticies[i + 1] = y1 + yPos;
		verticies[i + 2] = z1 + zPos;
	}

	//Update the actual rotation of the object. This must be done after it has been rotated
	xRot = x;

	//Set the dirty bit so the scene knows this object has changed
	dirty = true;
}

void Renderable::setYRot(float y)
{
	//Bounds check the new rotation value
	while (y >= 360)
	{
		y = y - 360;
	}
	while (y < 0)
	{
		y = y + 360;
	}

	//Roate the object and store the data back into verticies
	for (int i = 0; i < vSize; i += 6)
	{
		//Grab the object model data (subtract away the positional data)
		float x0 = verticies[i] - xPos;
		float y0 = verticies[i + 1] - yPos;
		float z0 = verticies[i + 2] - zPos;

		//Rotate along x axis (up/down) by however much the rotation value changed
		float x1 = x0;
		float y1 = (cos(-1.0 * (y - yRot) * ((4 * atan(1)) / 180)) * y0) - (sin(-1.0 * (y - yRot) * ((4 * atan(1)) / 180)) * z0);
		float z1 = (sin(-1.0 * (y - yRot) * ((4 * atan(1)) / 180)) * y0) + (cos(-1.0 * (y - yRot) * ((4 * atan(1)) / 180)) * z0);

		//Add Position data and store the data in verticies
		verticies[i] = x1 + xPos;
		verticies[i + 1] = y1 + yPos;
		verticies[i + 2] = z1 + zPos;
	}

	//Update the actual rotation of the object. This must be done after it has been rotated
	yRot = y;

	//Set the dirty bit so the scene knows this object has changed
	dirty = true;
}

void Renderable::setZRot(float z)
{
	//Bounds check the new rotation value
	while (z >= 360)
	{
		z = z - 360;
	}
	while (z < 0)
	{
		z = z + 360;
	}

	//Roate the object and store the data back into verticies
	for (int i = 0; i < vSize; i += 6)
	{
		//Grab the object model data (subtract away the positional data)
		float x0 = verticies[i] - xPos;
		float y0 = verticies[i + 1] - yPos;
		float z0 = verticies[i + 2] - zPos;

		//Rotate along z axis (roll) by however much the rotation value changed
		float x1 = (cos(-1.0 * (z - zRot) * ((4 * atan(1)) / 180)) * x0) - (sin(-1.0 * (z - zRot) * ((4 * atan(1)) / 180)) * y0);
		float y1 = (sin(-1.0 * (z - zRot) * ((4 * atan(1)) / 180)) * x0) + (cos(-1.0 * (z - zRot) * ((4 * atan(1)) / 180)) * y0);
		float z1 = z0;

		//Add Position data and store the data in verticies
		verticies[i] = x1 + xPos;
		verticies[i + 1] = y1 + yPos;
		verticies[i + 2] = z1 + zPos;
	}

	//Update the actual rotation of the object. This must be done after it has been rotated
	zRot = z;

	//Set the dirty bit so the scene knows this object has changed
	dirty = true;
}

//Getter for the Dirty variable
bool Renderable::isDirty()
{
	return dirty;
}

//Setter for the Dirty variable
void Renderable::setDirty(bool d)
{
	dirty = d;
}