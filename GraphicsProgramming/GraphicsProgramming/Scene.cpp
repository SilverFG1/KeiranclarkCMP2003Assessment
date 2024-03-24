#include "glut.h"
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

	// Load the texture using SOIL (Simple OpenGL Image Library)
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
	glEnable(GL_LIGHT0);                                    // Enable Light 0


	GLfloat floorpos[] = { -20, -2.0f, 20, -20, -2.0f, -20, 20, -1.0f, -20, 20, -2.0f , 20 };	//Defines a square floor with vertices at (-20, -1, -20), (20, -1, -20), (20, -1, 20), and (-20, -1, 20).

	// Generate a shadow matrix for the floor
	shadow.generateShadowMatrix(shadowmatrix, Light_Position, floorpos);


	//// Define vertices for a cube
	//GLfloat cubeVertices[] = 
	//{
	//	-0.5f, -0.5f, -0.5f, // Bottom-left-back vertex
	//	 0.5f, -0.5f, -0.5f, // Bottom-right-back vertex
	//	 0.5f,  0.5f, -0.5f, // Top-right-back vertex
	//	-0.5f,  0.5f, -0.5f, // Top-left-back vertex

	//	-0.5f, -0.5f,  0.5f, // Bottom-left-front vertex
	//	 0.5f, -0.5f,  0.5f, // Bottom-right-front vertex
	//	 0.5f,  0.5f,  0.5f, // Top-right-front vertex
	//	-0.5f,  0.5f,  0.5f, // Top-left-front vertex
	//};

	//// Define indices for drawing triangles
	//GLubyte cubeIndices[] = 
	//{
	//	// Back face
	//	0, 1, 2,
	//	0, 2, 3,

	//	// Front face
	//	4, 5, 6,
	//	4, 6, 7,

	//	// Left face
	//	0, 3, 7,
	//	0, 7, 4,

	//	// Right face
	//	1, 5, 6,
	//	1, 6, 2,

	//	// Bottom face
	//	0, 1, 5,
	//	0, 5, 4,

	//	// Top face
	//	2, 3, 7,
	//	2, 7, 6,
	//};

	//// Draw the cube with texture
	//glEnable(GL_DEPTH_TEST);

	//glPushMatrix();  // Push the current matrix onto the stack
	//glTranslatef(0.0f, -1.5f, 0.0f); // Translate along the y-axis
	//glBindTexture(GL_TEXTURE_2D, myTexture); // Bind our texture

	//// Define texture coordinates for the front face
	//GLfloat frontFaceTextureCoordinates[] =
	//{
	//	0.0f, 0.0f,
	//	1.0f, 0.0f,
	//	1.0f, 1.0f,
	//	0.0f, 1.0f,
	//};

	// Loop through all the cube indices to draw each face
	//for (int i = 0; i < 6; i++) 
	//{
	//	int offset = i * 6;  // Offset for each face
	//	glBegin(GL_QUADS);
	//	for (int j = 0; j < 4; j++) 
	//	{
	//		glTexCoord2fv(&frontFaceTextureCoordinates[i * 2]);
	//		glVertex3fv(&cubeVertices[(4 * 5 + i) * 3]); // Front face vertices start at index 20
	//	}
	//	glEnd();
	//}

	//glPopMatrix();

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
	glTranslatef(0.0f, 0.f, 0.0f);
	glBegin(GL_TRIANGLES);
	glEnd();
	glPopMatrix();
	glDisable(GL_DEPTH_TEST);

	// Render the model
	 
	shadow.generateShadowMatrix(shadowmatrix, Light_Position, floorpos);

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);

	glPushMatrix();
	glMultMatrixf((GLfloat*)shadowmatrix);
	
	//translate to floor and draw shadow, remember to match any transforms on the object
	glTranslatef(0.f, 0.f, 0.f);
	glScalef(0.2f, 0.2f, 0.2f);
	myModel.render();
	glPopMatrix();

	glColor3f(1.0f, 1.0f, 1.0f); // S
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);

	glPushMatrix();
	glTranslatef(0.f, 0.f, 0.f);

	glScalef(0.2f, 0.2f, 0.2f);
	myModel.render();
	glPopMatrix();

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
	glTranslatef(0.0f, 4.0f, 0.0f);  // Translate along the y-axis
	glBegin(GL_TRIANGLES);
	glColor3f(1.0f, 0.0f, 0.0f); // Red color
	for (int i = 0; i < 24; i++)
	{
		glVertex3fv(&pyramidVertices[pyramidIndices[i] * 3]);
	}
	glEnd();
	glPopMatrix();
	glDisable(GL_DEPTH_TEST);


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
