#include "model.h"

using namespace std;

SDL_Window *mainWindow;
SDL_GLContext mainContext;

bool voxelize_init()
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		std::cout << "Failed to init SDL\n";
		return false;
	}

	mainWindow = SDL_CreateWindow("PGP-Voxelize", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL);
	mainContext = SDL_GL_CreateContext(mainWindow);

	glewExperimental = GL_TRUE;
	glewInit();

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glEnable(GL_PROGRAM_POINT_SIZE);
	glClearDepth(1.0);
	glDepthRange(1.0, 0.0);

	return true;
}

void Cleanup()
{
	SDL_GL_DeleteContext(mainContext);
	SDL_DestroyWindow(mainWindow);
	SDL_Quit();
}

int main(int argc, char *argv[])
{
	(void)argc;
	
	int resolution = atoi(argv[1]);
	string file(argv[2]);

	if (!voxelize_init())
	{
		cout << "Initialization failed!\n";
		return 1;
	}

	Model m(file, resolution);

	m.voxelize();

	m.display(mainWindow);

	Cleanup();

	return 0;
}