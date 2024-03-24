#pragma once
#include "glut.h"
#include <gl/GL.h>
#include <gl/GLU.h>
#include "Input.h"
#include <stdio.h>
// Further includes should go here:
#include "SOIL.h"
#include <vector>
#include "Vector3.h"

class Camera
{

public:
	Camera();


	Camera(Input* in);

	void Update();
	void handleInput(float dt, int height, int width, Input* input);
	void setPosition(Vector3);
	void setLookAt(Vector3);
	void setUp(Vector3);
	void setRight(Vector3);
	void setForward(Vector3);
	void setYaw(float nYaw);
	void setPitch(float nPitch);
	void setRoll(float nRoll);

	void handleMouseLook(int x, int y);

	float getYaw();
	float getPitch();
	float getRoll();

	Vector3 getPosition();
	Vector3 getLookAt();
	Vector3 getUp();
	Vector3 getRight();
	Vector3 getForward();

	void moveforward(float dt);

	void movebackwards(float dt);

	void moveleft(float dt);

	void moveright(float dt);

	void moveup(float dt);

	void movedown(float dt);


	float yaw = 0, roll = 0, pitch = 90;
	int camPosx = -10, camPosy = 0, camPosz = 0;
	Vector3 forward, up, lookat, position, right;


	int speed = 50;

	Input* input;
};

