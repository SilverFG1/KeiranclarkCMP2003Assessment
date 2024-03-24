#ifndef MODEL_H
#define MODEL_H

#include <glut.h>
#include <fstream>
#include <gl/gl.h>
#include <gl/glu.h>
#include "Vector3.h"
#include <vector>
#include <string>
#include "SOIL.h"
using namespace std;


// Class definition for Model
class Model
{
public:
    // Public member variables
    int vertexCount; // Number of vertices in the model

    // Public member functions
    bool load(char* modelFilename, char* textureFilename); // Function to load model and texture
    void render(); // Function to render the model

    // Public member vectors to store sorted data
    vector<float> sortedvertex; // Sorted vertex data
    vector<float> sortednormals; // Sorted normal data
    vector<float> texC; // Texture coordinates data

    void renderSkybox();

private:
    // Private member functions
    void loadTexture(char*); // Function to load texture
    bool loadModel(char*); // Function to load model data

    // Private member variables
    GLuint texture; // OpenGL texture ID for the model's texture

    // Storage for unsorted data
    vector<float> vertex, normals, texCoords; // Original vertex, normal, and texture coordinates data
};
#endif
