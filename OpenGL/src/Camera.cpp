#include "Camera.h"

//A setter for screenDistance
void Camera::setScreenDistance()
{
	screenDistance = (1/(tan((fov*(PI/180))/2)));
}

//A setter for minDistanceOffset
void Camera::setMinDistanceOffset()
{
	minDistanceOffset = minDistance * minDistance + minDistance * length;
}

//Constructor that takes in position, fov, minDistance, and length
Camera::Camera(float x, float y, float z, float xr, float yr, float f, float m, float l)
{
	PI = 4*atan(1);
	xPos = x;
	yPos = y;
	zPos = z;
	prevYRotInput = 0;
	setXRot(xr);
	setYRot(yr);
	fov = f;
	minDistance = m;
	length = l;
	setScreenDistance();
	setMinDistanceOffset();
}

//A setter for xPos. Should only be used to teleport camera.
void Camera::setXPos(float x)
{
	xPos = x;
}

//A setter for yPos. Should only be used to teleport camera.
void Camera::setYPos(float y)
{
	yPos = y;
}

//A setter for zPos. Should only be used to teleport camera.
void Camera::setZPos(float z)
{
	zPos = z;
}

//A getter for xPos
float Camera::getXPos()
{
	return xPos;
}

//A getter for yPos
float Camera::getYPos()
{
	return yPos;
}

//A getter for zPos
float Camera::getZPos()
{
	return zPos;
}

//A setter for xRot. Should take in the x position of the mouse.
void Camera::setXRot(float x)
{
	while (x >= 360)
	{
		x = x - 360;
	}
	while (x < 0)
	{
		x = x + 360;
	}

	xRot = x;
}

//A setter for yRot. Should take in the y position of the mouse.
void Camera::setYRot(float y)
{
	if (y > prevYRotInput && (yRot + (y - prevYRotInput)) < 90)
	{
		yRot = yRot + (y - prevYRotInput);
	}
	else if (y < prevYRotInput && (yRot - (prevYRotInput - y)) > -90)
	{
		yRot = yRot - (prevYRotInput - y);
	}

	prevYRotInput = y;
}

//A getter for xRot
float Camera::getXRot()
{
	return xRot;
}

//A getter for yRot
float Camera::getYRot()
{
	return yRot;
}

//Moves the camera Forward distance units based on the direction it is facing
void Camera::moveForward(float distance)
{
	//Move in the X and Z direction
	xPos = xPos + (distance * sin(xRot * (PI / 180)) * cos(yRot * (PI / 180)));
	zPos = zPos + (distance * cos(xRot * (PI / 180)) * cos(yRot * (PI / 180)));

	//Move in the yDirection. +Y is looking down and -Y is looking up so we need to negate the yRot
	yPos = yPos + (distance * sin(-1.0 * yRot * (PI / 180)));
}

//Moves the camera Sidways distance units based on the direction it is facing. Positive distance moves right. Negative moves left
void Camera::moveRight(float distance)
{
	//Save the current value of our X and Y Rotation
	float previousXRot = xRot;
	float previousYRot = yRot;

	//rotate the camera 90 degrees so that it is facing to the right.
	setXRot(xRot + 90);

	//Set the yRot to be 0 so that we do not move up or down
	yRot = 0;

	//Move the camera
	moveForward(distance);

	//Restore previous value of X and Y Rotation. We don't use the setYRot() function because we do not want to change the prevYRotInput variable that is used to rotate the camera in the y direction
	setXRot(previousXRot);
	yRot = previousYRot;
}

//A setter for fov. 1 <= fov <= 179 (fov could be between 0 and 1 or between 179 and 180 but it is limited like this to avoid rounding errors causing it to end up at 180 or 0)
void Camera::setFOV(float f)
{
	if (f >= 1 && f <= 179)
	{
		fov = f;
		setScreenDistance();
	}
}

//A getter for fov
float Camera::getFOV()
{
	return fov;
}

//A getter for screenDistance
float Camera::getScreenDistance()
{
	return screenDistance;
}

//A setter for minDistance. min must be >= .1 (actually only needs to be greater than 0 but it checks if it is >= .1 to avoid it accidentially rounding to 0 for small values)
void Camera::setMinDistance(float min)
{
	if (min >= .1)
	{
		minDistance = min;
		setMinDistanceOffset();
	}
}

//A getter for minDistance
float Camera::getMinDistance()
{
	return minDistance;
}

//A getter for minDistanceOffset
float Camera::getMinDistanceOffset()
{
	return minDistanceOffset;
}

//A setter for length. len must be >= .1 (actually only needs to be greater than 0 but it checks if it is >= .1 to avoid it accidentially rounding to 0 for small values)
void Camera::setLength(float len)
{
	if (len >= .1)
	{
		length = len;
		setMinDistanceOffset();
	}
}

//A getter for length
float Camera::getLength()
{
	return length;
}