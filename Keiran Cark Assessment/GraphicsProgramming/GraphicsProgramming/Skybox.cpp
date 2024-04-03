#include "Skybox.h"
#include <glut.h>
#include "SOIL.h"


Skybox::Skybox()
{

	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	// Initialise scene variables
	skybox = SOIL_load_OGL_texture(
		"gfx/skybox.png",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_NTSC_SAFE_RGB |
		SOIL_FLAG_COMPRESS_TO_DXT);
	glBindTexture(GL_TEXTURE_2D, skybox);

}

void Skybox::update()
{

	// Roll, Pitch and Yall are variables stored by the camera
		// handle rotation
		// Only want to calculate these values once, when rotation changes, not every frame.
	cosY = cosf(Yaw * 3.1415 / 180);
	cosP = cosf(Pitch * 3.1415 / 180);
	cosR = cosf(Roll * 3.1415 / 180);
	sinY = sinf(Yaw * 3.1415 / 180);
	sinP = sinf(Pitch * 3.1415 / 180);
	sinR = sinf(Roll * 3.1415 / 180);

	forward.x = sinY * cosP;
	forward.y = sinP;
	forward.z = cosP * -cosY;

	lookat = position + forward;

	up.x = -cosY * sinR - sinY * sinP * cosR;
	up.y = cosP * cosR;
	up.z = -sinY * sinR - sinP * cosR * -cosY;

}

void Skybox::render()
{

	float size = 0.5;

	glPushMatrix();
	glTranslatef(getPosition().x, getPosition().y, getPosition().z); // camera position
	glDisable(GL_DEPTH_TEST);

	glBindTexture(GL_TEXTURE_2D, skybox);
	glBegin(GL_QUADS);
	// front face

	glColor3f(1.0f, 1.0f, 1.0f);
	glNormal3f(0.0f, 0.0f, -1.0f);
	glTexCoord2f(0.25, 0.25);
	glVertex3f(-size, size, size);
	glTexCoord2f(0.50, 0.25);
	glVertex3f(size, size, size);
	glTexCoord2f(0.50, 0.50);
	glVertex3f(size, -size, size);
	glTexCoord2f(0.25, 0.50);
	glVertex3f(-size, -size, size);
	// back face


	glNormal3f(0.0f, 0.0f, 1.0f);
	glTexCoord2f(1, 0.25);
	glVertex3f(-size, size, -size);
	glTexCoord2f(0.75, 0.25);
	glVertex3f(size, size, -size);
	glTexCoord2f(0.75, 0.50);
	glVertex3f(size, -size, -size);
	glTexCoord2f(1, 0.50);
	glVertex3f(-size, -size, -size);
	// right face


	glNormal3f(-1.0f, 0.0f, 0.0f);
	glTexCoord2f(0.50, 0.25);
	glVertex3f(size, size, size);
	glTexCoord2f(0.75, 0.25);
	glVertex3f(size, size, -size);
	glTexCoord2f(0.75, 0.50);
	glVertex3f(size, -size, -size);
	glTexCoord2f(0.50, 0.50);
	glVertex3f(size, -size, size);
	// left face


	glNormal3f(1.0f, 0.0f, 0.0f);
	glTexCoord2f(0.25, 0.25);
	glVertex3f(-size, size, size);
	glTexCoord2f(0, 0.25);
	glVertex3f(-size, size, -size);
	glTexCoord2f(0, 0.50);
	glVertex3f(-size, -size, -size);
	glTexCoord2f(0.25, 0.50);
	glVertex3f(-size, -size, size);


	// bottom face


	glNormal3f(0.0f, 1.0f, 0.0f);
	glTexCoord2f(0.25, 0.75);
	glVertex3f(size, -size, size);
	glTexCoord2f(0.25, 0.50);
	glVertex3f(size, -size, -size);
	glTexCoord2f(0.50, 0.50);
	glVertex3f(-size, -size, -size);
	glTexCoord2f(0.50, 0.75);
	glVertex3f(-size, -size, size);
	// top face


	glNormal3f(0.0f, 1.0f, 0.0f);
	glTexCoord2f(0.50, 0.25);
	glVertex3f(size, size, size);
	glTexCoord2f(0.50, 0);
	glVertex3f(size, size, -size);
	glTexCoord2f(0.25, 0);
	glVertex3f(-size, size, -size);
	glTexCoord2f(0.25, 0.25);
	glVertex3f(-size, size, size);

	glEnd();
	glEnable(GL_DEPTH_TEST);
	glPopMatrix();
	glPopMatrix();

}

Vector3 Skybox::getForward()
{
	return forward;
}


Vector3 Skybox::getUp()
{
	return up;
}

Vector3 Skybox::getLookat()
{
	return lookat;
}



Vector3 Skybox::getPosition()
{
	return position;
}


void Skybox::setForward(Vector3 newforward)
{
	forward = newforward;
}

void Skybox::setUp(Vector3 newUp)
{
	up = newUp;
}

void Skybox::setLookat(Vector3 newLookat)
{
	lookat = newLookat;
}

void Skybox::setPosition(Vector3 newPostion)
{
	position = newPostion;
}


float Skybox::getYaw()
{
	return Yaw;
}
float Skybox::getPitch()
{
	return Pitch;
}
float Skybox::getRoll()
{
	return Roll;
}

void Skybox::setYaw(float newYaw)
{
	Yaw = newYaw;
}
void Skybox::setPitch(float newPitch)
{
	Pitch = newPitch;
}
void Skybox::setRoll(float newRoll)
{
	Roll = newRoll;
}