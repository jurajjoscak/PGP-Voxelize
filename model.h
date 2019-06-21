#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <string>
#include <GL/glew.h>
#include <SDL2/SDL.h>
using namespace std;

class WPVector
{
	public:
		GLfloat x;
		GLfloat y;
		GLfloat z;

		WPVector();
		WPVector(float x, float y, float z);
		WPVector operator+(WPVector b);
		WPVector operator-(WPVector b);
		GLfloat operator*(WPVector b);
		WPVector operator*(float b);
		WPVector vectorMultiply(WPVector);
		GLfloat length();
		WPVector normalize();
};

class Model
{
	public:
		Model(string filename, int resolution);
		GLuint vao;
		GLuint vbo;
		GLuint voxel_vbo;
		unsigned int vbo_size;
		unsigned int grid_size;
		WPVector boundbox_min;
		WPVector boundbox_max;
		GLuint resolution = 70;
		GLfloat step;
		GLfloat* data;
		GLuint total_voxels;
		void display(SDL_Window *mainWindow);
		void voxelize();
		~Model();
};