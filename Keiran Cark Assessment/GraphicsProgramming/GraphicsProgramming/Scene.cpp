#include "glut.h"
#include <vector>
#include <GL/gl.h>
#include <GL/glu.h>
//#include <SOIL/SOIL.h>  // Include SOIL for image loading
//#include <GL/glew.h>    // Include GLEW for OpenGL extensions
#include <iostream>
#include "Scene.h"
#include "Camera.h"
#include "Model.h"
#include "Skybox.h"

//Spot light
GLfloat Light_Diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat Light_Position[] = { 0.0f, 7.0f, 0.0f, 1.0f };
GLfloat Light_Ambient[] = { 0.1f, 0.1f, 0.1f, 1.0f };
GLfloat Spot_Direction[] = { -1.0f, 1.0f, 0.0f };

GLuint skyboxTextureID;
GLfloat skyboxVertices[];

bool CameraSwap = false;
bool wireframeMode = false;

const int Slices = 50;
const int Stacks = 50;
const float radius = 1.0f;


// Scene constructor, initializes OpenGL
// You should add further variables that need to be initialized.

Scene::Scene(Input* in)
{
	// Store pointer for input class
	input = in;
	initialiseOpenGL(); // Call function to initialize OpenGL

	// Other OpenGL/render settings should be applied here.

	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_LIGHTING);

	// Initialize scene variables
	myModel.load("models/lowpolyrat.obj", "gfx/grass.png"); // Load 3D model with texture

	// Enable texture and set texture environment mode
	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	// Load the texture using SOIL (Simple OpenGL Image Library)
	myTexture = SOIL_load_OGL_texture
	(
		"gfx/grass.png",   // Replace with the actual path to your texture
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
	);

	 //Load the texture using SOIL (Simple OpenGL Image Library)
	myFloor = SOIL_load_OGL_texture
	(
		"gfx/grass.png",   // Replace with the actual path to your texture
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
	);

	if (myTexture == 0)
	{
		// Handle texture loading error
		std::cerr << "Error loading texture." << std::endl;
	}
	else
	{
		glBindTexture(GL_TEXTURE_2D, myTexture); // Bind the loaded texture
	}
}





void Scene::handleInput(float dt)
{
	// Handle user input

	// Hide the cursor and warp it to the center of the window
	glutSetCursor(GLUT_CURSOR_NONE);
	glutWarpPointer(width / 2, height / 2);

	// Check if CameraSwap is false (use default camera) or true (use alternative camera)
	if (!CameraSwap)
	{
		// Handle input and mouse look for the default camera
		camera.handleInput(dt, height, width, input);
		camera.handleMouseLook(input->getMouseX() - (width / 2), input->getMouseY() - (height / 2));
	}
	else
	{
		// Handle input and mouse look for the alternative camera (camera23)
		camera23.handleInput(dt, height, width, input);
		camera23.handleMouseLook(input->getMouseX() - (width / 2), input->getMouseY() - (height / 2));
	}

	// Check if the 'f' key is pressed to swap cameras
	if (input->isKeyDown('f') && !CameraSwap) {
		input->setKeyUp('f');
		CameraSwap = true; // Switch to alternative camera
	}
	if (input->isKeyDown('f') && CameraSwap) {
		input->setKeyUp('f');
		CameraSwap = false; // Switch back to default camera
	}

	// Check if the 'R' key is pressed to toggle wireframe mode
	if (input->isKeyDown('R') && !wireframeMode) {
		input->setKeyUp('R');
		wireframeMode = true; // Enable wireframe mode
	}
	if (input->isKeyDown('R') && wireframeMode) {
		input->setKeyUp('R');
		wireframeMode = false; // Disable wireframe mode
	}
}


void Scene::update(float dt)
{
	// Update scene-related variables.

	// Check if the default camera is active
	if (!CameraSwap)
	{
		// Update the position and look-at direction of the default camera
		camera.Update();
		camera.position = camera.getPosition();
		camera.lookat = camera.getLookAt();
		skybox.position = camera.getPosition();
	}
	else
	{
		// Update the position and look-at direction of the alternative camera (camera23)
		camera23.Update();
		camera23.position = camera23.getPosition();
		camera23.lookat = camera23.getLookAt();
		skybox.position = camera23.getPosition();
	}

	// Toggle wireframe mode on when the 'r' key is pressed and it's not already toggled
	if (input->isKeyDown('r') && !WireframeToggle)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // Set wireframe rendering mode
		WireframeToggle = true; // Set the toggle state to true
		input->setKeyUp('r'); // Mark the 'r' key as released
	}
	// Toggle wireframe mode off when the 'r' key is pressed and it's already toggled
	else if (input->isKeyDown('r') && WireframeToggle)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // Set filled rendering mode
		WireframeToggle = false; // Set the toggle state to false
		input->setKeyUp('r'); // Mark the 'r' key as released
	}

	// Calculate FPS for output
	calculateFPS();
	skybox.update();
}



void Scene::render()
{

	// Clear Color and Depth Buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Reset transformations
	glLoadIdentity();

	// Set the skybox
	gluLookAt(skybox.getPosition().x, skybox.getPosition().y, skybox.getPosition().z, skybox.getLookat().x, skybox.getLookat().y, skybox.getLookat().z, skybox.getUp().x, skybox.getUp().y, skybox.getUp().z);

	glLoadIdentity();
	// Set the camera based on CameraSwap state

	// If CameraSwap is false, use the default camera settings

	if (!CameraSwap)
	{
		gluLookAt(camera.getPosition().x, camera.getPosition().y, camera.getPosition().z, camera.getLookAt().x, camera.getLookAt().y, camera.getLookAt().z, camera.getUp().x, camera.getUp().y, camera.getUp().z);
	}
	// If CameraSwap is true, use the alternative camera (camera23) settings
	else
	{
		gluLookAt(camera23.getPosition().x, camera23.getPosition().y, camera23.getPosition().z, camera23.getLookAt().x, camera23.getLookAt().y, camera23.getLookAt().z, camera23.getUp().x, camera23.getUp().y, camera23.getUp().z);
	}

	// Render geometry/scene here -------------------------------------

	skybox.render();

	// Enable depth test after rendering the skybox
	glEnable(GL_DEPTH_TEST);

	GLfloat Light_Ambient[] = { 0.3f, 0.3f, 0.3f, 1.0f };	//Sets ambient light intensity to a dim gray (0.3, 0.3, 0.3).
	GLfloat Light_Diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };	//Sets diffuse light intensity to white (1.0, 1.0, 1.0).
	GLfloat Light_Position[] = { 4.0f, -4.0f, 4.0f, 1.0f };	//Places the light source at coordinates (4, -4, 4).
	GLfloat Spot_Direction[] = { 1.0f, -1.0f, 0.0f };

	// Set light properties for Light 0
	glLightfv(GL_LIGHT0, GL_DIFFUSE, Light_Diffuse);        // Set diffuse light color
	glLightfv(GL_LIGHT0, GL_POSITION, Light_Position);      // Set light position
	glLightfv(GL_LIGHT0, GL_AMBIENT, Light_Ambient);        // Set ambient light color
	glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, Spot_Direction); // Set spotlight direction
	glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 250.0f);            // Set spotlight cutoff angle
	glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, 60.0f);           // Set spotlight exponent
	glEnable(GL_LIGHT0);                                 // Enable Light 0

	// Define properties of the blue light
	GLfloat Blue_Light_Ambient[] = { 0.0f, 0.0f, 0.3f, 1.0f };    // Ambient light color (blue)
	GLfloat Blue_Light_Diffuse[] = { 0.0f, 0.0f, 1.0f, 1.0f };    // Diffuse light color (blue)
	GLfloat Blue_Light_Position[] = { 3.0f, 4.0f, 0.0f, 1.0f };  // Position of the blue light (above the pyramid)
	GLfloat Blue_Spot_Direction[] = { 0.0f, -1.0f, 0.0f };         // Direction of the spotlight (pointing downwards)

	// Set light properties for the blue light
	glLightfv(GL_LIGHT1, GL_DIFFUSE, Blue_Light_Diffuse);        // Set diffuse light color for the blue light
	glLightfv(GL_LIGHT1, GL_POSITION, Blue_Light_Position);      // Set light position for the blue light
	glLightfv(GL_LIGHT1, GL_AMBIENT, Blue_Light_Ambient);        // Set ambient light color for the blue light
	glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, Blue_Spot_Direction);// Set spotlight direction
	glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 45.0f);                  // Set spotlight cutoff angle
	glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 60.0f);                // Set spotlight exponent
	glEnable(GL_LIGHT1);

	GLfloat floorpos[] = { -20, -2.0f, 20, -20, -2.0f, -20, 20, -1.0f, -20, 20, -2.0f , 20 };	//Defines a square floor with vertices at (-20, -1, -20), (20, -1, -20), (20, -1, 20), and (-20, -1, 20).

	// Generate a shadow matrix for the floor
	shadow.generateShadowMatrix(shadowmatrix, Light_Position, floorpos);


	glDisable(GL_DEPTH_TEST);

	// Define vertices for a pyramid
	GLfloat pyramidVertices[] =
	{
	-1.0f, -1.0f, -1.0f, // Base left-back vertex
	 1.0f, -1.0f, -1.0f, // Base right-back vertex
	 1.0f, -1.0f,  1.0f, // Base right-front vertex
	-1.0f, -1.0f,  1.0f, // Base left-front vertex

	 0.0f,  1.0f,  0.0f, // Apex vertex
	};

	// Define indices for drawing triangles
	GLubyte pyramidIndices[] =
	{
		// Base
		0, 1, 2,
		0, 2, 3,

		// Sides
		0, 4, 1,
		1, 4, 2,
		2, 4, 3,
		3, 4, 0,
	};

	// Draw a blank triangle
	glColor3f(0.0f, 0.0f, 0.0f);
	glRotatef(180, 1, 0, 0);
	glEnable(GL_DEPTH_TEST);
	glPushMatrix();  // Push the current matrix onto the stack
	glTranslatef(0.0f, 0.f, 0.1f);
	glBegin(GL_TRIANGLES);
	glEnd();
	glPopMatrix();
	glDisable(GL_DEPTH_TEST);

	// Render model with shadow
	glPushMatrix(); // Push matrix for model with shadow
	// Shadow generation
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);
	shadow.generateShadowMatrix(shadowmatrix, Light_Position, floorpos);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);
	glPushMatrix(); // Push matrix for shadow rendering
	glMultMatrixf((GLfloat*)shadowmatrix);
	// Translate to floor and draw shadow
	glTranslatef(0.0f, 0.0f, 0.1f);
	glScalef(0.2f, 0.2f, 0.2f);
	// Render model
	glPopMatrix(); // Pop matrix for shadow rendering
	glColor3f(1.0f, 1.0f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);
	glPushMatrix(); // Push matrix for model rendering
	glTranslatef(0.0f, 0.0f, 0.0f);
	glScalef(0.2f, 0.2f, 0.2f);
	// Render model
	glPopMatrix(); // Pop matrix for model rendering
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);
	glPopMatrix(); // Pop matrix for model with shadow


	// Render the floor using glBegin and glEnd
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, myFloor); // Bind the loaded texture

	glEnable(GL_DEPTH_TEST);
	glBegin(GL_QUADS);
	for (float i = 20; i > -20; i = i - 0.1f)
	{
		for (float j = 20; j > -20; j = j - 0.1f)
		{
			// Set the normal vector for the floor (pointing upwards)
			glNormal3f(0.0f, 1.0f, 0.0f);

			// Define vertices and texture coordinates for a quad
			glTexCoord2f(0.0f, 0.0f); glVertex3f(i, 0.0f, j);
			glTexCoord2f(1.0f, 0.0f); glVertex3f(i, 0.0f, j - 0.1f);
			glTexCoord2f(1.0f, 1.0f); glVertex3f(i - 0.1f, 0.0f, j - 0.1f);
			glTexCoord2f(0.0f, 1.0f); glVertex3f(i - 0.1f, 0.0f, j);
		}
	}
	glEnd();
	glDisable(GL_TEXTURE_2D);

	// Draw the pyramid with red color
	glEnable(GL_DEPTH_TEST);
	glPushMatrix();  // Push the current matrix onto the stack
	glTranslatef(3.0f, 3.0f, 0.0f);  // Translate along the y-axis
	glBegin(GL_TRIANGLES);
	glColor3f(1.0f, 0.0f, 0.0f); // Red color
	for (int i = 0; i < 24; i++)
	{
		glVertex3fv(&pyramidVertices[pyramidIndices[i] * 3]);
	}
	glEnd();
	glPopMatrix();
	glDisable(GL_DEPTH_TEST);


	// Define vertices for a cube
	GLfloat cubeVertices[] =
	{
		// Front face
		-1.0f, -1.0f,  1.0f, // Bottom left
		 1.0f, -1.0f,  1.0f, // Bottom right
		 1.0f,  1.0f,  1.0f, // Top right
		-1.0f,  1.0f,  1.0f, // Top left

		// Back face
		-1.0f, -1.0f, -1.0f, // Bottom left
		 1.0f, -1.0f, -1.0f, // Bottom right
		 1.0f,  1.0f, -1.0f, // Top right
		-1.0f,  1.0f, -1.0f, // Top left
	};

	// Define indices for drawing triangles
	GLubyte cubeIndices[] =
	{
		// Front face
		0, 1, 2,
		2, 3, 0,

		// Right face
		1, 5, 6,
		6, 2, 1,

		// Back face
		7, 6, 5,
		5, 4, 7,

		// Left face
		4, 0, 3,
		3, 7, 4,

		// Top face
		3, 2, 6,
		6, 7, 3,

		// Bottom face
		4, 5, 1,
		1, 0, 4,
	};

	// Draw the cube with yellow color
	glEnable(GL_DEPTH_TEST);
	glPushMatrix();  // Push the current matrix onto the stack
	glTranslatef(3.0f, 1.0f, 0.0f);  // Translate along the y-axis
	glColor3f(1.0f, 1.0f, 0.0f); // Yellow color
	glBegin(GL_TRIANGLES);
	for (int i = 0; i < 36; i++)
	{
		glVertex3fv(&cubeVertices[cubeIndices[i] * 3]);
	}
	glEnd();
	glPopMatrix();
	glDisable(GL_DEPTH_TEST);

	// Draw the cube with yellow color
	glEnable(GL_DEPTH_TEST);
	glPushMatrix();  // Push the current matrix onto the stack
	glTranslatef(3.0f, 1.0f, 0.0f);  // Translate along the y-axis
	glColor3f(1.0f, 1.0f, 0.0f); // Yellow color
	glBegin(GL_TRIANGLES);
	for (int i = 0; i < 36; i++)
	{
		glVertex3fv(&cubeVertices[cubeIndices[i] * 3]);
	}
	glEnd();
	glPopMatrix();
	glDisable(GL_DEPTH_TEST);


	float xOffset = -10.0f;
	float yOffset = 1.0f;
	float zOffset = -1.5f;

	// Bind the texture
	glBindTexture(GL_TEXTURE_2D, myTexture);
	// Enable texture mapping
	glEnable(GL_TEXTURE_2D);

	glBegin(GL_QUADS);

	for (int i = 0; i < Slices; ++i)
	{
		float theta1 = i * (2 * 3.14) / Slices;
		float theta2 = (i + 1) * (2 * 3.14) / Slices;
		for (int j = 0; j < Stacks; ++j)
		{
			float phi1 = j * 3.14 / Stacks;
			float phi2 = (j + 1) * 3.14 / Stacks;

			// Vertices of the quad
			float x1 = (radius * sin(theta1) * sin(phi1)) + xOffset;
			float y1 = (radius * cos(phi1)) + yOffset;
			float z1 = (radius * cos(theta1) * sin(phi1)) + zOffset;

			float x2 = (radius * sin(theta1) * sin(phi2)) + xOffset;
			float y2 = (radius * cos(phi2)) + yOffset;
			float z2 = (radius * cos(theta1) * sin(phi2)) + zOffset;

			float x3 = (radius * sin(theta2) * sin(phi2)) + xOffset;
			float y3 = (radius * cos(phi2)) + yOffset;
			float z3 = (radius * cos(theta2) * sin(phi2)) + zOffset;

			float x4 = (radius * sin(theta2) * sin(phi1)) + xOffset;
			float y4 = (radius * cos(phi1)) + yOffset;
			float z4 = (radius * cos(theta2) * sin(phi1)) + zOffset;

			// Draw the quad
			glVertex3f(x1, y1, z1);
			glVertex3f(x2, y2, z2);
			glVertex3f(x3, y3, z3);
			glVertex3f(x4, y4, z4);
		}
	}

	glEnd();

	// Disable texture mapping after rendering the quads
	glDisable(GL_TEXTURE_2D);


	// Draw the star
	glBindTexture(GL_TEXTURE_2D, myFloor);
	glEnable(GL_TEXTURE_2D);

	glBegin(GL_TRIANGLES);

	//glLoadIdentity(); // Reset the modelview matrix
	glTranslatef(0.0, 1.0, 1.5); // Translate the shape up by 1 unit along the y-axis

	// Bottom pyramid
	//glColor3f(1.0, 0.0, 0.0);  // Red
	glVertex3f(0.0, -1.0, 0.0);
	glVertex3f(0.5, 0.0, 0.0);
	glVertex3f(-0.5, 0.0, 0.0);

	glVertex3f(0.0, -1.0, 0.0);
	glVertex3f(0.0, 0.0, 0.5);
	glVertex3f(0.0, 0.0, -0.5);

	glVertex3f(0.0, -1.0, 0.0);
	glVertex3f(0.5, 0.0, 0.0);
	glVertex3f(0.0, 0.0, 0.5);

	glVertex3f(0.0, -1.0, 0.0);
	glVertex3f(-0.5, 0.0, 0.0);
	glVertex3f(0.0, 0.0, -0.5);

	glVertex3f(0.5, 0.0, 0.0);
	glVertex3f(-0.5, 0.0, 0.0);
	glVertex3f(0.0, 0.0, 0.5);

	glVertex3f(0.5, 0.0, 0.0);
	glVertex3f(-0.5, 0.0, 0.0);
	glVertex3f(0.0, 0.0, -0.5);

	// Top pyramid
	//glColor3f(0.0, 0.0, 1.0);  // Blue
	glVertex3f(0.0, 1.0, 0.0);
	glVertex3f(0.5, 0.0, 0.0);
	glVertex3f(-0.5, 0.0, 0.0);

	glVertex3f(0.0, 1.0, 0.0);
	glVertex3f(0.0, 0.0, 0.5);
	glVertex3f(0.0, 0.0, -0.5);

	glVertex3f(0.0, 1.0, 0.0);
	glVertex3f(0.5, 0.0, 0.0);
	glVertex3f(0.0, 0.0, 0.5);

	glVertex3f(0.0, 1.0, 0.0);
	glVertex3f(-0.5, 0.0, 0.0);
	glVertex3f(0.0, 0.0, -0.5);

	glVertex3f(0.5, 0.0, 0.0);
	glVertex3f(-0.5, 0.0, 0.0);
	glVertex3f(0.0, 0.0, 0.5);

	glVertex3f(0.5, 0.0, 0.0);
	glVertex3f(-0.5, 0.0, 0.0);
	glVertex3f(0.0, 0.0, -0.5);


	glEnd();

	// Disable texture mapping
	glDisable(GL_TEXTURE_2D);

	glFlush();

	// Bind the texture
	glBindTexture(GL_TEXTURE_2D, myTexture);
	// Enable texture mapping
	glEnable(GL_TEXTURE_2D);

	glBegin(GL_QUAD_STRIP);

	// Front face
	glVertex3f(6.0f, 0.0f, 0.0f); // Bottom left
	glVertex3f(10.0f, 0.0f, 0.0f); // Bottom right
	glVertex3f(6.0f, 2.0f, 0.0f); // Top left
	glVertex3f(10.0f, 2.0f, 0.0f); // Top right

	// Right face
	glVertex3f(10.0f, 0.0f, 0.0f); // Bottom front
	glVertex3f(10.0f, 0.0f, -4.0f); // Bottom back
	glVertex3f(10.0f, 2.0f, 0.0f); // Top front
	glVertex3f(10.0f, 2.0f, -4.0f); // Top back

	// Back face
	glVertex3f(10.0f, 0.0f, -4.0f); // Bottom right
	glVertex3f(6.0f, 0.0f, -4.0f); // Bottom left
	glVertex3f(10.0f, 2.0f, -4.0f); // Top right
	glVertex3f(6.0f, 2.0f, -4.0f); // Top left

	// Left face
	glVertex3f(6.0f, 0.0f, -4.0f); // Bottom back
	glVertex3f(6.0f, 0.0f, 0.0f); // Bottom front
	glVertex3f(6.0f, 2.0f, -4.0f); // Top back
	glVertex3f(6.0f, 2.0f, 0.0f); // Top front

	// Bottom face
	glVertex3f(6.0f, 0.0f, -4.0f); // Back left
	glVertex3f(10.0f, 0.0f, -4.0f); // Back right
	glVertex3f(6.0f, 0.0f, 0.0f); // Front left
	glVertex3f(10.0f, 0.0f, 0.0f); // Front right

	glEnd();

	// Disable texture mapping after rendering the quad strip
	glDisable(GL_TEXTURE_2D);


		// Set texture parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		// Push the current matrix onto the stack
		glPushMatrix();
		{
			// Enable stencil test
			glEnable(GL_STENCIL_TEST);
			// Disable depth test and lighting
			glDisable(GL_DEPTH_TEST);
			glDisable(GL_LIGHTING);

			// Disable writing to color buffer
			glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
			// Set stencil function and operation for the first pass
			glStencilFunc(GL_ALWAYS, 1, 1);
			glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

			// Draw solid wall
			glEnable(GL_POLYGON_OFFSET_FILL);
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			glBegin(GL_QUADS);
			glVertex2f(-5.0f, -5.0f);
			glVertex2f(5.0f, -5.0f);
			glVertex2f(5.0f, 5.0f);
			glVertex2f(-5.0f, 5.0f);
			glEnd();
			glDisable(GL_POLYGON_OFFSET_FILL);

			// Re-enable depth test and restore color mask
			glEnable(GL_DEPTH_TEST);
			glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
			// Set stencil function and operation for the second pass
			glStencilFunc(GL_EQUAL, 1, 1);
			glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);

			// Draw the model inside the 'mirror'
			glPushMatrix();
			glScalef(0.5, -0.5, 0.5);
			glTranslatef(1, 0, -1);
			glRotatef(180.1f, 180.0f, 1, 0);

			myModel.render();
			glPopMatrix();

			// Disable stencil test
			glDisable(GL_STENCIL_TEST);
			// Enable blending
			glEnable(GL_BLEND);
			glDisable(GL_LIGHTING);

			// Set wall color with transparency
			glColor4f(0.8f, 0.8f, 1.0f, 0.8f); // Wall colour
			// Draw the wall
			glBegin(GL_QUADS); // Wall
			glVertex2f(-15.0f, -15.0f);
			glVertex2f(15.0f, -15.0f);
			glVertex2f(15.0f, 15.0f);
			glVertex2f(-15.0f, 15.0f);
			glEnd();

			// Re-enable lighting and disable blending
			glEnable(GL_LIGHTING);
			glDisable(GL_BLEND);

			// Draw the model outside the 'mirror'
			glPushMatrix();
			glScalef(0.5, -0.5, 0.5); // Flip vertically
			glTranslatef(1, 0, 1);
			glRotatef(180.0f, 180.0f, 1, 0);

			myModel.render();
			glPopMatrix();

			// Pop the matrix from the stack
			glPopMatrix();
		}
		glDisable(GL_BLEND); // Disable blending



		// End render geometry --------------------------------------

		// Render text, should be last object rendered.
		renderTextOutput();

		// Swap buffers, after all objects are rendered.
		glutSwapBuffers();

}
	


void Scene::initialiseOpenGL()
{
	//OpenGL settings
	glShadeModel(GL_SMOOTH);							// Enable Smooth Shading
	glClearColor(0.39f, 0.58f, 93.0f, 1.0f);			// Cornflour Blue Background
	glClearDepth(1.0f);									// Depth Buffer Setup
	glClearStencil(0);									// Clear stencil buffer
	glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
	glDepthFunc(GL_LEQUAL);								// The Type Of Depth Testing To Do
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really Nice Perspective Calculations
	glLightModelf(GL_LIGHT_MODEL_LOCAL_VIEWER, 1);


	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);	// Blending function
}

// Handles the resize of the window. If the window changes size the perspective matrix requires re-calculation to match new window size.
void Scene::resize(int w, int h)
{
		width = w;
		height = h;
		// Prevent a divide by zero, when window is too short
		// (you cant make a window of zero width).
		if (h == 0)
			h = 1;

		float ratio = (float)w / (float)h;
		fov = 45.0f;
		nearPlane = 0.1f;
		farPlane = 100.0f;

		// Use the Projection Matrix
		glMatrixMode(GL_PROJECTION);

		// Reset Matrix
		glLoadIdentity();

		// Set the viewport to be the entire window
		glViewport(0, 0, w, h);

		// Set the correct perspective.
		gluPerspective(fov, ratio, nearPlane, farPlane);

		// Get Back to the Modelview
		glMatrixMode(GL_MODELVIEW);
}

// Calculates FPS
void Scene::calculateFPS()
{
	frame++;
	time = glutGet(GLUT_ELAPSED_TIME);

	if (time - timebase > 1000) {
		sprintf_s(fps, "FPS: %4.2f", frame*1000.0 / (time - timebase));
		timebase = time;
		frame = 0;
	}
}

// Compiles standard output text including FPS and current mouse position.
void Scene::renderTextOutput()
{
	// Render current mouse position and frames per second.
	sprintf_s(mouseText, "Mouse: %i, %i", input->getMouseX(), input->getMouseY());
	displayText(-1.f, 0.96f, 1.f, 0.f, 0.f, mouseText);
	displayText(-1.f, 0.90f, 1.f, 0.f, 0.f, fps);
}

// Renders text to screen. Must be called last in render function (before swap buffers)
void Scene::displayText(float x, float y, float r, float g, float b, char* string) 
{
	// Get Lenth of string
	int j = strlen(string);

	// Swap to 2D rendering
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-1.0, 1.0, -1.0, 1.0, 5, 100);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	// Orthographic lookAt (along the z-axis).
	gluLookAt(0.0f, 0.0f, 10.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);

	// Set text colour and position.
	glColor3f(r, g, b);
	glRasterPos2f(x, y);
	// Render text.
	for (int i = 0; i < j; i++) {
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, string[i]);
	}
	// Reset colour to white.
	glColor3f(1.f, 1.f, 1.f);

	// Swap back to 3D rendering.
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(fov, ((float)width/(float)height), nearPlane, farPlane);
	glMatrixMode(GL_MODELVIEW);
}
