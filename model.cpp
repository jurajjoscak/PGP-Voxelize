#include "model.h"
#include <cmath>

void bindTransformMatrix(GLuint location, float horizontal, float vertical, float z)
{
	GLfloat sh = sin(horizontal);
	GLfloat ch = cos(horizontal);
	GLfloat sv = sin(vertical);
	GLfloat cv = cos(vertical);

	GLfloat mat[16] =
	{
		ch, 0.0, sh, 0.0,
		-sv*(-sh), cv, -sv*ch,	0.0,
		-sh*cv,	sv, ch*cv, 0.0,
		0.0, 0.0, 0.0, 1.0
	};

	glUniformMatrix4fv(location, 1, GL_FALSE, mat);
}

GLuint CreateShaderProgram(string suffix)
{
	// Create the shaders
	GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
	GLuint GeometryShaderID = glCreateShader(GL_GEOMETRY_SHADER);

	GLint Result = GL_FALSE;
	int InfoLogLength;

	// Compile Vertex Shader
	printf("Compiling shader : vertex\n");

	ifstream t("VERTEX_SHADER" + suffix + ".gls");
	stringstream vbuffer;
	vbuffer << t.rdbuf();
	string vsrc = vbuffer.str();
	char const * VertexSourcePointer = vsrc.c_str();
	const GLint lenvshader = vsrc.length();

	glShaderSource(VertexShaderID, 1, &VertexSourcePointer, &lenvshader);
	glCompileShader(VertexShaderID);

	// Check Vertex Shader
	glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> VertexShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
		printf("%s\n", &VertexShaderErrorMessage[0]);
	}

	// Compile geometry shader
	if (suffix != "")
	{
		printf("Compiling shader : geometry\n");

		ifstream g("GEOMETRY_SHADER" + suffix + ".gls");
		stringstream gbuffer;
		gbuffer << g.rdbuf();
		string gsrc = gbuffer.str();
		char const * GeometrySourcePointer = gsrc.c_str();
		const GLint lengshader = gsrc.length();

		glShaderSource(GeometryShaderID, 1, &GeometrySourcePointer, &lengshader);
		glCompileShader(GeometryShaderID);

		// Check Geometry Shader
		glGetShaderiv(GeometryShaderID, GL_COMPILE_STATUS, &Result);
		glGetShaderiv(GeometryShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
		if (InfoLogLength > 0)
		{
			std::vector<char> GeometryShaderErrorMessage(InfoLogLength + 1);
			glGetShaderInfoLog(GeometryShaderID, InfoLogLength, NULL, &GeometryShaderErrorMessage[0]);
			printf("%s\n", &GeometryShaderErrorMessage[0]);
		}
	}

	// Compile Fragment Shader
	printf("Compiling shader : fragment\n");

	ifstream f("FRAGMENT_SHADER" + suffix + ".gls");
	stringstream fbuffer;
	fbuffer << f.rdbuf();
	string fsrc = fbuffer.str();
	char const * FragmentSourcePointer = fsrc.c_str();
	const GLint lenfshader = fsrc.length();

	glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer, &lenfshader);
	glCompileShader(FragmentShaderID);

	// Check Fragment Shader
	glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> FragmentShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
		printf("%s\n", &FragmentShaderErrorMessage[0]);
	}


	// Link the program
	printf("Linking program\n");
	GLuint shader_program = glCreateProgram();
	glAttachShader(shader_program, VertexShaderID);
	glAttachShader(shader_program, FragmentShaderID);
	if(suffix != "")
		glAttachShader(shader_program, GeometryShaderID);
	glLinkProgram(shader_program);
	// Check the program
	glGetProgramiv(shader_program, GL_LINK_STATUS, &Result);
	glGetProgramiv(shader_program, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> ProgramErrorMessage(InfoLogLength + 1);
		glGetProgramInfoLog(shader_program, InfoLogLength, NULL, &ProgramErrorMessage[0]);
		printf("%s\n", &ProgramErrorMessage[0]);
	}

	glDetachShader(shader_program, VertexShaderID);
	glDetachShader(shader_program, FragmentShaderID);
	if (suffix != "")
	{
		glDetachShader(shader_program, GeometryShaderID);
		glDeleteShader(GeometryShaderID);
	}
	glDeleteShader(VertexShaderID);
	glDeleteShader(FragmentShaderID);

	glUseProgram(shader_program);
	return shader_program;
}

void Control(GLfloat& scale_factor, GLfloat& horizontal, GLfloat& vertical, bool& voxel, bool& quit)
{
	SDL_Event event;
	static bool rotate = false;
	static bool scale = false;
	while (SDL_PollEvent(&event))
	{
		if (event.type == SDL_KEYDOWN)
		{
			switch (event.key.keysym.sym)
			{
			case SDLK_ESCAPE:
				quit = true;
				break;
			case SDLK_r:
				voxel = !voxel;
				break;
			}
		}
		if (event.type == SDL_QUIT)
			quit = true;

		if (event.type == SDL_MOUSEBUTTONDOWN)
		{
			if (event.button.button == SDL_BUTTON_LEFT)
				rotate = true;
			else if (event.button.button == SDL_BUTTON_RIGHT)
				scale = true;
		}
		if (event.type == SDL_MOUSEBUTTONUP)
		{
			if (event.button.button == SDL_BUTTON_LEFT)
				rotate = false;
			else if (event.button.button == SDL_BUTTON_RIGHT)
				scale = false;
		}
		if (event.type == SDL_MOUSEMOTION)
		{
			if (rotate)
			{
				horizontal -= event.motion.xrel*0.005f;
				vertical -= event.motion.yrel*0.005f;
			}
			if (scale)
			{
				scale_factor += event.motion.yrel*0.005f;
			}
		}
	}
}

WPVector::WPVector()
{
	x = 0.0;
	y = 0.0;
	z = 0.0;
}

WPVector::WPVector(float x, float y, float z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}

GLfloat WPVector::length()
{
	return sqrt(x*x + y*y + z*z);
}

WPVector WPVector::normalize()
{
	GLfloat len = this->length();
	x = x / len;
	y = y / len;
	z = z / len;

	return *this;
}

WPVector WPVector::vectorMultiply(WPVector b)
{
	WPVector res;
	res.x = this->y*b.z - this->z*b.y;
	res.y = this->z*b.x - this->x*b.z;
	res.z = this->x*b.y - this->y*b.x;
	return res;
}

WPVector WPVector::operator+(WPVector b)
{
	WPVector res;
	res.x = this->x + b.x;
	res.y = this->y + b.y;
	res.z = this->z + b.z;
	return res;
}

WPVector WPVector::operator-(WPVector b)
{
	WPVector res;
	res.x = this->x - b.x;
	res.y = this->y - b.y;
	res.z = this->z - b.z;
	return res;
}

GLfloat WPVector::operator*(WPVector b)
{
	GLfloat res = this->x * b.x + this->y * b.y + this->z * b.z;
	return res;
}

WPVector WPVector::operator*(float b)
{
	WPVector res(this->x*b, this->y*b, this->z*b);
	return res;
}

Model::Model(string filename, int resolution)
{
	ifstream f((filename).data());
	string line;
	stringstream iss;
	string val;
	float x, y, z;
	int a;
	vector<WPVector*> vertices;
	vector<GLuint> triangles;
	this->resolution = resolution;

	while (getline(f, line))
	{
		iss.str("");
		iss.clear();

		iss.str(line.data());
		iss >> val;
		if (val == "v")
		{
			iss >> val;
			x = (float)atof(val.data());
			iss >> val;
			y = (float)atof(val.data());
			iss >> val;
			z = (float)atof(val.data());

			vertices.push_back(new WPVector(x, y, z));
			if (x < boundbox_min.x)
				boundbox_min.x = x;
			if (y < boundbox_min.y)
				boundbox_min.y = y;
			if (z < boundbox_min.z)
				boundbox_min.z = z;

			if (x > boundbox_max.x)
				boundbox_max.x = x;
			if (y > boundbox_max.y)
				boundbox_max.y = y;
			if (z > boundbox_max.z)
				boundbox_max.z = z;
		}
		else if (val == "f")
		{
			iss >> val;
			a = atoi(val.data()) - 1;
			triangles.push_back(a);
				
			iss >> val;
			a = atoi(val.data()) - 1;
			triangles.push_back(a);

			iss >> val;
			a = atoi(val.data()) - 1;
			triangles.push_back(a);
		}
	}
	f.close();

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	vbo_size = triangles.size();

	data = new GLfloat[vbo_size*3];

	for (unsigned int i = 0; i<vbo_size; i++)
	{
		data[i * 3] = vertices[(triangles[i])]->x;
		data[i * 3 + 1] = vertices[(triangles[i])]->y;
		data[i * 3 + 2] = vertices[(triangles[i])]->z;
	}

	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*vbo_size*3, (GLvoid*)data, GL_STATIC_DRAW);
	delete [] data;

	step = (boundbox_max.x - boundbox_min.x) / resolution;
	grid_size = unsigned int(resolution * ceil((boundbox_max.y - boundbox_min.y)/step) * ceil((boundbox_max.z - boundbox_min.z) / step));
}

void Model::display(SDL_Window *mainWindow)
{
	bool loop = true;
	GLfloat scale_factor = 0.0;
	GLfloat horizontal = 0.0;
	GLfloat vertical = 0.0;
	bool voxel = true;
	bool quit = false;
	GLuint surfaceShader = CreateShaderProgram("");
	GLuint voxelShader = CreateShaderProgram("_VOXEL");
	GLuint ScaleSurfaceLoc = glGetUniformLocation(surfaceShader, "Scale_factor");
	GLuint HorizontalSurfaceLoc = glGetUniformLocation(surfaceShader, "horizontal");
	GLuint VerticalSurfaceLoc = glGetUniformLocation(surfaceShader, "vertical");
	GLuint ScaleVoxelLoc = glGetUniformLocation(voxelShader, "Scale_factor");
	GLuint HorizontalVoxelLoc = glGetUniformLocation(voxelShader, "horizontal");
	GLuint VerticalVoxelLoc = glGetUniformLocation(voxelShader, "vertical");
	GLuint MVPvoxelLoc = glGetUniformLocation(voxelShader, "MVP");
	GLuint stepVoxelLoc = glGetUniformLocation(voxelShader, "step");
	GLuint MVPsurfaceLoc = glGetUniformLocation(surfaceShader, "MVP");

	glUseProgram(surfaceShader);

	while (loop)
	{
		Control(scale_factor, horizontal, vertical, voxel, quit);
		if (quit)
			return;

		
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		if (voxel)
		{
			glBindBuffer(GL_ARRAY_BUFFER, voxel_vbo);
			glUseProgram(voxelShader);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

			glUniform1f(ScaleVoxelLoc, scale_factor);
			glUniform1f(HorizontalVoxelLoc, horizontal);
			glUniform1f(VerticalVoxelLoc, vertical);
			glUniform1f(stepVoxelLoc, step);
			bindTransformMatrix(MVPvoxelLoc, horizontal, vertical, scale_factor);

			glDrawArrays(GL_POINTS, 0, total_voxels);
			glDisableVertexAttribArray(0);
		}
		else
		{
			glBindBuffer(GL_ARRAY_BUFFER, vbo);
			glUseProgram(surfaceShader);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

			glUniform1f(ScaleSurfaceLoc, scale_factor);
			glUniform1f(HorizontalSurfaceLoc, horizontal);
			glUniform1f(VerticalSurfaceLoc, vertical);
			bindTransformMatrix(MVPsurfaceLoc, horizontal, vertical, scale_factor);

			glDrawArrays(GL_TRIANGLES, 0, vbo_size);
			glDisableVertexAttribArray(0);
		}
		SDL_GL_SwapWindow(mainWindow);
	}
}

GLuint loadVoxelizer()
{
	// Create the shader
	GLuint ComputeShaderID = glCreateShader(GL_COMPUTE_SHADER);

	GLint Result = GL_FALSE;
	int InfoLogLength;

	// Compile Compute Shader
	printf("Compiling shader : compute\n");

	ifstream t("COMPUTE_SHADER.gls");
	stringstream cbuffer;
	cbuffer << t.rdbuf();
	string csrc = cbuffer.str();
	char const * ComputeSourcePointer = csrc.c_str();
	const GLint lencshader = csrc.length();

	glShaderSource(ComputeShaderID, 1, &ComputeSourcePointer, &lencshader);
	glCompileShader(ComputeShaderID);

	// Check Compute Shader
	glGetShaderiv(ComputeShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(ComputeShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> ComputeShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(ComputeShaderID, InfoLogLength, NULL, &ComputeShaderErrorMessage[0]);
		printf("%s\n", &ComputeShaderErrorMessage[0]);
	}

	// Link the program
	printf("Linking program\n");
	GLuint shader_program = glCreateProgram();
	glAttachShader(shader_program, ComputeShaderID);
	glLinkProgram(shader_program);
	// Check the program
	glGetProgramiv(shader_program, GL_LINK_STATUS, &Result);
	glGetProgramiv(shader_program, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> ProgramErrorMessage(InfoLogLength + 1);
		glGetProgramInfoLog(shader_program, InfoLogLength, NULL, &ProgramErrorMessage[0]);
		printf("%s\n", &ProgramErrorMessage[0]);
	}

	glDetachShader(shader_program, ComputeShaderID);
	glDeleteShader(ComputeShaderID);
	glUseProgram(shader_program);
	return shader_program;
}

void Model::voxelize()
{
	GLuint compute_program = loadVoxelizer();
	glUseProgram(compute_program);

	GLuint resolutionLoc = glGetUniformLocation(compute_program, "resolution");
	GLuint boxMinLoc = glGetUniformLocation(compute_program, "box_min");
	GLuint boxMaxLoc = glGetUniformLocation(compute_program, "box_max");
	GLuint stepLoc = glGetUniformLocation(compute_program, "step");
	GLuint heightLoc = glGetUniformLocation(compute_program, "height");
	GLuint widthLoc = glGetUniformLocation(compute_program, "width");
	GLuint depthLoc = glGetUniformLocation(compute_program, "depth");


	GLuint grid;
	glGenBuffers(1, &grid);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, grid);
	glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(GLint)*grid_size, NULL, GL_STATIC_DRAW);


	glBindBuffer(GL_SHADER_STORAGE_BUFFER, vbo);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, vbo);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, grid);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, grid);

	int width = resolution;
	int height = int(ceil((boundbox_max.y - boundbox_min.y) / step));
	int depth = int(ceil((boundbox_max.z - boundbox_min.z) / step));

	glUniform1i(resolutionLoc, resolution);
	glUniform3f(boxMinLoc, boundbox_min.x, boundbox_min.y, boundbox_min.z);
	glUniform3f(boxMaxLoc, boundbox_min.x + width*step, boundbox_min.y + height*step, boundbox_min.z + depth*step);
	glUniform1f(stepLoc, step);
	glUniform1i(heightLoc, height);
	glUniform1i(widthLoc, width);
	glUniform1i(depthLoc, depth);
	
	glDispatchCompute(vbo_size + 32-(vbo_size % 32), 1, 1);
		
	glMemoryBarrier(GL_ALL_BARRIER_BITS);

	GLint* voxels = (GLint*)glMapNamedBuffer(grid, GL_READ_WRITE);

	vector<WPVector> points;
		
	unsigned int counter = 0;
	for (int z = 0; z < depth; z++)
	{
		for (int y = 0; y < height; y++)
		{
			for (int x = 0; x < width; x++)
			{
				if (voxels[z*height*width + y*width + x] == 1)
				{
					points.push_back(WPVector(boundbox_min.x + x*step, boundbox_min.y + y*step, boundbox_min.z + z*step));
				}
			}
		}
	}

	total_voxels = points.size();

	cout << "TOTAL # OF SET VOXELS: " << total_voxels << "\n";

	glUnmapNamedBuffer(grid);

	glGenBuffers(1, &voxel_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, voxel_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(WPVector)*points.size(), points.data(), GL_STATIC_DRAW);

	glDeleteBuffers(1, &grid);
}

Model::~Model()
{
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &voxel_vbo);
	glDeleteVertexArrays(1, &vao);
}