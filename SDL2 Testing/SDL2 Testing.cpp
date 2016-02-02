// SDL2 Testing.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include<iostream>
#include <SDL.h>
#include <gl/glew.h>


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

	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);
	SDL_GL_SwapWindow(window);

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
				
				case SDLK_r:
					std::cout << "here" << std::endl;
					glClearColor(1.0, 0.0, 0.0, 1.0);
					glClear(GL_COLOR_BUFFER_BIT);
					SDL_GL_SwapWindow(window);
					break;
				
				default:
					break;
				}
			}
		}

	}

	SDL_GL_DeleteContext(mainContext);
	SDL_DestroyWindow(window);
	SDL_Quit();
    return 0;
}

