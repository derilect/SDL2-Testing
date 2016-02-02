// SDL2 Testing.cpp : Defines the entry point for the console application.
//

#include<iostream>
#include <SDL.h>
#include <gl/glew.h>

GLuint gProgramId = 0;
GLuint gVertexPos2DLocation = -1;
GLuint gVBO = 0;
GLuint gIBO = 0;

int CreatePrograms()
{
	gProgramId = glCreateProgram();
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

	const GLchar *vertexShaderSource[] =
	{
		"#version 140\n in vec2 LVertexPos2D; void main() { gl_Position = vec4(LVertexPos2D.x, LVertexPos2D.y, 0, 1); }"
	};

	glShaderSource(vertexShader, 1, vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	GLint vShaderCompiled = GL_FALSE;
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &vShaderCompiled);

	if (vShaderCompiled != GL_TRUE)
	{
		std::cout << "Error: Unable to compile vertex shader" << std::endl;
		return 0;
	}
	else
	{
		glAttachShader(gProgramId, vertexShader);
		GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

		const GLchar *fragmentShaderSource[] =
		{
			"#version 140\n out vec4 LFragment; void main() { LFragment = vec4(1.0,1.0,1.0,1.0); }"
		};

		glShaderSource(fragmentShader, 1, fragmentShaderSource, NULL);
		glCompileShader(fragmentShader);

		GLint fShaderCompiled = GL_FALSE;
		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &fShaderCompiled);
		if (fShaderCompiled != GL_TRUE)
		{
			std::cout << "Error: Unable to compile fragment shader" << std::endl;
			return 0;
		}
		else
		{
			glAttachShader(gProgramId, fragmentShader);
			glLinkProgram(gProgramId);

			GLint programSuccess = GL_TRUE;
			glGetProgramiv(gProgramId, GL_LINK_STATUS, &programSuccess);
			if (programSuccess != GL_TRUE)
			{
				std::cout << "Error: Unable to link shader program" << std::endl;
				return 0;
			}

			else
			{
				gVertexPos2DLocation = glGetAttribLocation(gProgramId, "LVertexPos2D");
				if (gVertexPos2DLocation == -1)
				{
					std::cout << "Error: LVertexPos2DLocation is not a valid GLSL program var" << std::endl;
					return 0;
				}
				else
				{
					glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
					GLfloat vertexData[] =
					{
						-0.5f, -0.5f,
						0.5f, -0.5f,
						0.5f, 0.5f,
						-0.5f, 0.5f
					};

					GLuint indexData[] = { 0,1,2,3 };
					glGenBuffers(1, &gVBO);
					glBindBuffer(GL_ARRAY_BUFFER, gVBO);
					glBufferData(GL_ARRAY_BUFFER, 2 * 4 * sizeof(GLfloat), vertexData, GL_STATIC_DRAW);

					glGenBuffers(1, &gIBO);
					glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gIBO);
					glBufferData(GL_ELEMENT_ARRAY_BUFFER, 4 * sizeof(GLuint), indexData, GL_STATIC_DRAW);
				}
			}
		}
	}

	return 1;

}

void render()
{
	glClear(GL_COLOR_BUFFER_BIT);
	
	glUseProgram(gProgramId);
	glEnableVertexAttribArray(gVertexPos2DLocation);
	glBindBuffer(GL_ARRAY_BUFFER, gVBO);
	glVertexAttribPointer(gVertexPos2DLocation, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), NULL);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gIBO);
	glDrawElements(GL_TRIANGLE_FAN, 4, GL_UNSIGNED_INT, NULL);

	glDisableVertexAttribArray(gVertexPos2DLocation);
	glUseProgram(NULL);

}

int main(int argc, char *argv[])
{ //Test
	if(SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
		return 1;
	}
	SDL_Window *window = SDL_CreateWindow("Hello World", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_OPENGL);

	if (window == nullptr)
	{
		std::cout << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl; 
		return 1;
	}

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	SDL_GLContext mainContext = SDL_GL_CreateContext(window);

	if (mainContext == nullptr)
	{
		std::cout << "SDL Error: " << SDL_GetError() << std::endl;
		return 1;
	}

	SDL_GL_SetSwapInterval(1);

	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK)
	{
		std::cout << "GLEW Error: Unable to initialize" << std::endl;
		return 1;
	}


	SDL_GL_SetSwapInterval(1);

	if (CreatePrograms() == 0)
	{
		std::cout << "Error: CreatePrograms failed to complete" << std::endl;
		return 1;
	}


	bool isRunning = true;
	while (isRunning)
	{

		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
				isRunning = false;

			if (event.type == SDL_KEYDOWN)
			{
				switch (event.key.keysym.sym)
				{
				case SDLK_ESCAPE:
					isRunning = false;
					break;

				default:
					break;
				}
			}
		}

		render();

	}

	SDL_GL_DeleteContext(mainContext);
	SDL_DestroyWindow(window);
	SDL_Quit();
    return 0;
}

