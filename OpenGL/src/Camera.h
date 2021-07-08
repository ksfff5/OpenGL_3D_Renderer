#ifndef CAMERA_H
#define CAMERA_H

#include <cmath>

//TODO:
//- require that you input a pointer to a shader for the constructor for the purposes of initializing the camera related uniform values in said shader

//A class to handle camera information
class Camera
{
	private:
		//PI (calculated using 4tan^-1(1))
		float PI;

		//The x position of the camera
		float xPos;

		//The y position of the camera
		float yPos;

		//The z position of the camera
		float zPos;

		//The x rotation of the camera. Ranges between 0 and 360 inclusive on the 0 side. 0 is +Z; 90 is +-X; 180 is -Z; 270 is +-X;
		float xRot;

		//The y rotation of the camera. ranges between -90 and 90 inclusive. 90 is straight down. -90 is straight up.
		float yRot;

		//The previous value of that was input into the setYRot function. Used to determine whether or not to update the yRot variable. This is needed to stop the yRot from wrapping around the top of the screen.
		float prevYRotInput;

		//The field of view of the camera
		float fov;

		//The distance from the camera to the screen where the points would be "projected onto" if we didn't save the z values
		float screenDistance;

		//The minimum distance in front of the camera an object must be to render it. Must be > 0
		float minDistance;

		//A value that is related to the minDistance and length. It is used to make the minDistance variable work properly.
		float minDistanceOffset;

		//The length of the viewing plane. In other words, this plus the minDistance is the maximum distance an object can be rendered at
		float length;

		//A setter for screenDistance
		void setScreenDistance();

		//A setter for minDistanceOffset
		void setMinDistanceOffset();

	public:
		//Constructor that takes in position, fov, minDistance, and length
		Camera(float x, float y, float z, float xr, float yr, float f, float m, float l);

		//A setter for xPos. Should only be used to teleport camera.
		void setXPos(float x);

		//A setter for yPos. Should only be used to teleport camera.
		void setYPos(float y);

		//A setter for zPos. Should only be used to teleport camera.
		void setZPos(float z);

		//A getter for xPos
		float getXPos();

		//A getter for yPos
		float getYPos();

		//A getter for zPos
		float getZPos();

		//A setter for xRot. Should take in the x position of the mouse.
		void setXRot(float x);

		//A setter for yRot. Should take in the y position of the mouse.
		void setYRot(float y);

		//A getter for xRot
		float getXRot();

		//A getter for yRot
		float getYRot();

		//Moves the camera Forward distance units based on the direction it is facing
		void moveForward(float distance);

		//Moves the camera to the right distance units based on the direction it is facing. Positive distance moves right. Negative moves left
		void moveRight(float distance);

		//A setter for fov. 1 <= fov <= 179 (fov could be between 0 and 1 or between 179 and 180 but it is limited like this to avoid rounding errors causing it to end up at 180 or 0)
		void setFOV(float f);

		//A getter for fov
		float getFOV();

		//A getter for screenDistance
		float getScreenDistance();

		//A setter for minDistance. min must be >= .1 (actually only needs to be greater than 0 but it checks if it is >= .1 to avoid it accidentially rounding to 0 for small values)
		void setMinDistance(float min);

		//A getter for minDistance
		float getMinDistance();

		//A getter for minDistanceOffset
		float getMinDistanceOffset();

		//A setter for length. len must be >= .1 (actually only needs to be greater than 0 but it checks if it is >= .1 to avoid it accidentially rounding to 0 for small values)
		void setLength(float len);

		//A getter for length
		float getLength();
};

#endif