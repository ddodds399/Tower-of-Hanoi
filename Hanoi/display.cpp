#include "display.h"
#include <iostream>
#include <GL\glew.h>

Display::Display(int width, int height, const std::string& title)
{
	//Initialise SDL and set attributes.
	SDL_Init(SDL_INIT_EVERYTHING);

	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	//Create a window object with openGL context.
	m_window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_OPENGL);
	m_glContext = SDL_GL_CreateContext(m_window);

	//Disable cursor when over window.
	SDL_ShowCursor(SDL_DISABLE);

	//Initialise GLEW.
	GLenum status = glewInit();

	//Error check if GLEW fails to initialise.
	if (status != GLEW_OK) {
		std::cerr << "GLEW failed to initialise!" << std::endl;
	}

	//Set values of initial boolean variables that relate to keyboard actions.
	m_isClosed = false;
	m_isFullscreen = false;
	m_downPress = false;
	m_upPress = false;
	m_rightPress = false;
	m_leftPress = false;
	m_spacePress = false;
	m_keyDown = false;
	m_aPress = false;
	m_dPress = false;
	m_rPress = false;
	Clear(0.0f, 0.15f, 0.3f, 1.0f);

	//Enable OpenGL functions
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glDepthFunc(GL_LESS);
	
	SDL_GL_SwapWindow(m_window);
	
}

//Deconstructor
Display::~Display()
{
	SDL_GL_DeleteContext(m_glContext);
	SDL_DestroyWindow(m_window);
	SDL_Quit();
}

//Clearing background colour
void Display::Clear(float r, float g, float b, float a)
{
	glClearColor(r, g, b, a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

}

///////////// Getters for Keyboard Button Booleans /////////////////
//These keep track of key presses.

bool Display::IsClosed() 
{
	return m_isClosed;
}

bool Display::IsUpPressed()
{
	return m_upPress;
}

bool Display::IsRightPressed()
{
	return m_rightPress;
}

bool Display::IsLeftPressed()
{
	return m_leftPress;
}

bool Display::IsDownPressed()
{
	return m_downPress;
}

bool Display::IsSpacePressed()
{
	return m_spacePress;
}

bool Display::IsDPressed()
{
	return m_dPress;
}

bool Display::IsAPressed()
{
	return m_aPress;
}

bool Display::IsRPressed()
{
	return m_rPress;
}

//Used to check if any key is down.
bool Display::IsKeyDown()
{
	return m_keyDown;
}

//Resets all keyboard booleans to false
void Display::ResetAll()
{
	m_downPress = false;
	m_upPress = false;
	m_rightPress = false;
	m_leftPress = false;
	m_spacePress = false;
	m_keyDown = false;
	m_aPress = false;
	m_dPress = false;
	m_rPress = false;
}

//Display update method that is called in main.cpp
void Display::Update()
{
	SDL_GL_SwapWindow(m_window);

	SDL_Event e;

	//Handling keyboard events
	while (SDL_PollEvent(&e)) 
	{
		//If a key is presed down.
		if (e.type == SDL_KEYDOWN) {
			switch (e.key.keysym.sym) {
			//Check which key is pressed.
			case SDLK_ESCAPE:
				m_isClosed = true;
				break;
			case SDLK_f:
				if (m_isFullscreen == false)
				{
					m_isFullscreen = true;
					SDL_SetWindowFullscreen(m_window, 1);
				}
				else
				{
					m_isFullscreen = false;
					SDL_SetWindowFullscreen(m_window, 0);
				}
				break;
			case SDLK_UP:
				m_upPress = true;
				break;
			case SDLK_RIGHT:
				m_rightPress = true;
				break;
			case SDLK_DOWN:
				m_downPress = true;
				break;
			case SDLK_LEFT:
				m_leftPress = true;
				break;
			case SDLK_SPACE:
				m_spacePress = true;
				break;
			case SDLK_a:
				m_aPress = true;
				break;
			case SDLK_d:
				m_dPress = true;
				break;
			case SDLK_r:
				m_rPress = true;
				break;
			}
		}
		//If a key is released
		if (e.type == SDL_KEYUP) {
			switch (e.key.keysym.sym) {
			//Check which key is released
			case SDLK_UP:
				m_upPress = false;
				break;
			case SDLK_RIGHT:
				m_rightPress = false;
				m_keyDown = false;
				break;
			case SDLK_DOWN:
				m_downPress = false;
				break;
			case SDLK_LEFT:
				m_leftPress = false;
				m_keyDown = false;
				break;
			case SDLK_SPACE:
				m_spacePress = false;
				m_keyDown = false;
				break;
			case SDLK_a:
				m_aPress = false;
				break;
			case SDLK_d:
				m_dPress = false;
				break;
			case SDLK_r:
				m_rPress = false;
				break;
			}
		}

		//If quitting, this will trigger the while loop in main.cpp and close the program
		if (e.type == SDL_QUIT)
		{
			m_isClosed = true;
		}


	}
}
