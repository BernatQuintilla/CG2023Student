#include "application.h"
#include "mesh.h"
#include "shader.h"
#include "utils.h" 
#include <cmath>
#include <iostream>

Application::Application(const char* caption, int width, int height)
{
	this->window = createWindow(caption, width, height);

	int w,h;
	SDL_GetWindowSize(window,&w,&h);

	this->mouse_state = 0;
	this->time = 0.f;
	this->window_width = w;
	this->window_height = h;
	this->keystate = SDL_GetKeyboardState(nullptr);

	this->framebuffer.Resize(w, h);
}

Application::~Application()
{
	delete keystate;
}

void Application::Init(void)
{
	std::cout << "Initiating app..." << std::endl;
	for (int i = 0; i < 200; i++) {
		framebuffer.A[i].x = rand() % 1280;
		framebuffer.A[i].y = rand() % 720;
		framebuffer.A[i].v = 55 + rand() % (75 - 55);
	}
}

// Render one frame
void Application::Render(void)
{
	for (int i = 0; i < 200; i++) {
		framebuffer.SetPixel(framebuffer.A[i].x, framebuffer.A[i].y, Color(255, 255, 255));
	}
	framebuffer.Render();
}

// Called after render
void Application::Update(float seconds_elapsed)
{
	for (int i = 0; i < 200; i++) {
		framebuffer.SetPixel(framebuffer.A[i].x, framebuffer.A[i].y, Color(0, 0, 0));
		if (framebuffer.A[i].x + framebuffer.A[i].v * seconds_elapsed < 1280){
			if ((framebuffer.A[i].x % 2) == 0) {
				framebuffer.A[i].x += framebuffer.A[i].v * seconds_elapsed;
			}
			else if(framebuffer.A[i].x - framebuffer.A[i].v * seconds_elapsed > 0 ){
				framebuffer.A[i].x -= framebuffer.A[i].v * seconds_elapsed;
			}
		}
		if (framebuffer.A[i].y - framebuffer.A[i].v * seconds_elapsed > 0) {
			framebuffer.A[i].y -= framebuffer.A[i].v * seconds_elapsed;
		}
	}
}

//keyboard press event 
void Application::OnKeyPressed( SDL_KeyboardEvent event )
{
	// KEY CODES: https://wiki.libsdl.org/SDL2/SDL_Keycode
	int mouse_x = lround(mouse_position.x);
	int mouse_y = lround(mouse_position.y);
	switch(event.keysym.sym) {
		case SDLK_ESCAPE: exit(0); break; // ESC key, kill the app
		case SDLK_l: framebuffer.DrawLineDDA(mouse_x, mouse_y, mouse_x + 100, mouse_y + 50, Color(255, 0, 0)); break;
		case SDLK_b: framebuffer.DrawLineBresenham(mouse_x, mouse_y, mouse_x + 100, mouse_y + 60, Color(255, 255, 255)); break;
		case SDLK_c: framebuffer.DrawCircle(mouse_x, mouse_y, 50, Color(255, 255, 255), true); break;
	}
}

void Application::OnMouseButtonDown( SDL_MouseButtonEvent event )
{
	if (event.button == SDL_BUTTON_LEFT) {

	}
}

void Application::OnMouseButtonUp( SDL_MouseButtonEvent event )
{
	if (event.button == SDL_BUTTON_LEFT) {

	}
}

void Application::OnMouseMove(SDL_MouseButtonEvent event)
{
	
}

void Application::OnFileChanged(const char* filename)
{ 
	Shader::ReloadSingleShader(filename);
}