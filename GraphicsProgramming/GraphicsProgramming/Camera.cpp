#include "Camera.h"

// Initialize camera position to (0, 0, 0)
 Camera::Camera() 
{
	position.x = 0;
	position.y = 0;
	position.z = 0;
}

// Initialize the camera input pointer
Camera::Camera(Input* in) 
{
	input = in;
}

void Camera::Update() 
{

	float cosR, cosP, cosY; //temp values for sin/cos from
	float sinR, sinP, sinY;
	// Roll, Pitch and Yall are variables stored by the camera
	// handle rotation
	// Only want to calculate these values once, when rotation changes, not every frame.
	cosY = cosf(yaw * 3.1415 / 180);
	cosP = cosf(pitch * 3.1415 / 180);
	cosR = cosf(roll * 3.1415 / 180);

	sinY = sinf(yaw * 3.1415 / 180);
	sinP = sinf(pitch * 3.1415 / 180);
	sinR = sinf(roll * 3.1415 / 180);

	// Calculate the forward vector
	// This vector points in the direction the camera is facing
	forward.x = sinY * cosP;
	forward.y = sinP;
	forward.z = cosP * -cosY;

	// Calculate the lookat point
	// This is the point the camera is looking at, which is in front of it along the forward vector
	lookat = position + forward;

	// Calculate the up vector
	// This vector points upwards, perpendicular to both the forward and right vectors
	up.x = -cosY * sinR - sinY * sinP * cosR;
	up.y = cosP * cosR;
	up.z = -sinY * sinR - sinP * cosR * -cosY;

	// Calculate the right vector
	// This vector points to the right, perpendicular to both the forward and up vectors
	right = forward.cross(up);

}

void Camera::handleMouseLook(int x, int y) 
{
	yaw -= 0.05 * x;
	pitch -= 0.05 * y;
}

void Camera::moveforward(float dt) 
{
	position.add(forward, 5 * dt);
}

void Camera::movebackwards(float dt) 
{
	position.subtract(forward, 5 * dt);
}


void Camera::moveleft(float dt)
{
	position.subtract(right, 5 * dt);
}


void Camera::moveright(float dt) 
{
	position.add(right, 5 * dt);
}

void Camera::moveup(float dt) 
{
	position.add(up, 5 * dt);
}

void Camera::movedown(float dt) 
{
	position.subtract(up, 5 * dt);
}


void Camera::handleInput(float dt, int height, int width, Input* input) 
{
	if (input->isKeyDown('w')) 
	{
		moveforward(dt);
	}

	if (input->isKeyDown('s')) 
	{
		movebackwards(dt);
	}

	if (input->isKeyDown('a')) 
	{
		moveleft(dt);
	}

	if (input->isKeyDown('d')) 
	{
		moveright(dt);
	}

	if (input->isKeyDown('q')) 
	{
		moveup(dt);
	}

	if (input->isKeyDown('e')) 
	{
		movedown(dt);
	}

}

void Camera::setPosition(Vector3 nPos) 
{
	position = nPos;
}

Vector3 Camera::getLookAt() 
{
	return lookat;
}

void Camera::setUp(Vector3 nUp) 
{
	up = nUp;
}



void Camera::setForward(Vector3 nForward) 
{
	forward = nForward;
}


Vector3 Camera::getPosition() 
{
	return position;
}

void Camera::setLookAt(Vector3 nLookAt) 
{

	lookat = nLookAt;
}

Vector3 Camera::getUp() 
{
	return up;
}



Vector3 Camera::getForward() 
{
	return forward;
}

float Camera::getYaw() 
{
	return yaw;
}

float Camera::getPitch() 
{
	return pitch;
}

float Camera::getRoll() 
{
	return roll;
}


void Camera::setYaw(float nYaw) 
{
	yaw = nYaw;
}

void Camera::setPitch(float nPitch) 
{
	pitch = nPitch;
}


void Camera::setRoll(float nRoll) 
{
	roll = nRoll;
}

