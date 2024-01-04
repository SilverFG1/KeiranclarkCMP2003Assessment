#pragma once  // Ensure this header is included only once

#include "glut.h"  // OpenGL Utility Toolkit
#include <gl/GL.h>  // OpenGL API
#include <gl/GLU.h>  // OpenGL Utility Library
#include "Input.h"  // Input handling class
#include <stdio.h>  // Standard input/output functions
// Further includes should go here:
#include "SOIL.h"  // Simple OpenGL Image Library for texture loading
#include <vector>  // For storing lists of objects
#include "Vector3.h"  // Custom 3D vector class

class Skybox
{
public:

	// Constructor:
	Skybox();

	// Methods for updating, handling input, and rendering:
	void update();       // Updates the skybox's state
	void handleInput(float dt, int height, int width, Input* input);  // Handles user input
	void render();       // Renders the skybox

	// Setters and getters for skybox properties:
	void setForward(Vector3);
	Vector3 getForward();
	void setUp(Vector3);
	Vector3 getUp();
	void setPosition(Vector3);
	float getYaw();
	float getPitch();
	float getRoll();
	void setYaw(float newYaw);
	void setPitch(float newPitch);
	void setRoll(float newRoll);
	Vector3 getPosition();
	void setLookat(Vector3);
	Vector3 getLookat();




	float cosR, cosP, cosY; //temp values for sin/cos from
	float sinR, sinP, sinY;

	// Skybox properties:
	float Yaw = 0, Roll = 0, Pitch = 90;	// Rotation angles
	int camPosx = -10, camPosy = 0, camPosz = 0;	// Position coordinates
	Vector3 forward, up, lookat, position;		// Direction vectors
	GLuint skybox;		// OpenGL texture ID for the skybox texture

};
