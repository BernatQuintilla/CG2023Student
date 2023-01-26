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
	tool.LoadPNG("images/toolbar.png");
	framebuffer.DrawImagePixels(tool,true);

	for (int i = 0; i < 200; i++) {
		framebuffer.A[i].x = rand() % 1280;
		framebuffer.A[i].y = rand() % 660;
		framebuffer.A[i].v = 55 + rand() % (75 - 55);
		framebuffer.A[i].size = rand() % 10;
	}
}

// Render one frame
void Application::Render(void)
{
	for (int i = 0; i < 200; i++) {
		if (framebuffer.A[i].y == 0) {
			framebuffer.SetPixel(framebuffer.A[i].x, framebuffer.A[i].y, Color(0, 0, 0));
		}
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
		case SDLK_SPACE:
			if (mouse_y > 675 && mouse_y < 703) {
				if (mouse_x > 115 && mouse_x < 145) {
					c = Color(0, 0, 0);//Color negro
				}
				else if (mouse_x > 166 && mouse_x < 194) {
					c = Color(255, 0, 0);//Color rojo
				}
				else if (mouse_x > 218 && mouse_x < 244) {
					c = Color(0, 255, 0);//Color verde
				}
				else if (mouse_x > 267 && mouse_x < 294) {
					c = Color(0, 0, 255);//Color azul oscuro
				}
				else if (mouse_x > 317 && mouse_x < 344) {
					c = Color(255, 255, 0);//Color amarillo
				}
				else if (mouse_x > 367 && mouse_x < 394) {
					c = Color(255, 0, 255);//Color rosa
				}
				else if (mouse_x > 417 && mouse_x < 444) {
					c = Color(0, 255, 255);//Color azul claro
				}
				else if (mouse_x > 467 && mouse_x < 494) {
					c = Color(255, 255, 255);//Color blanco
				}
				else if (mouse_x > 15 && mouse_x < 40) {
					//Renovar hoja
					for (int i = 0; i < 1280; i++) {
						for (int j = 0; j < 720; j++) {
							framebuffer.SetPixel(i, j, Color(0, 0, 0));
						}
					}
					c = Color(255, 255, 255); //Volvemos a color predeterminado
					Init();
				}
				else if (mouse_x > 64 && mouse_x < 97) {
					framebuffer.SaveTGA("images/file.tga");
				}

			}break;
		case SDLK_l: framebuffer.DrawLineDDA(mouse_x, mouse_y, mouse_x + 100, mouse_y + 50, c); break;
		case SDLK_b: framebuffer.DrawLineBresenham(mouse_x, mouse_y, mouse_x + 100, mouse_y + 60, c); break;
		case SDLK_c: framebuffer.DrawCircle(mouse_x, mouse_y, 50, c, true); break;
		case SDLK_x: framebuffer.DrawCircle(mouse_x, mouse_y, 50, c, false); break;
		case SDLK_i: printf("%i", mouse_x);
	}
}

void Application::OnMouseButtonDown( SDL_MouseButtonEvent event )
{
	int mouse_x = lround(mouse_position.x);
	int mouse_y = lround(mouse_position.y);
	if (event.button == SDL_BUTTON_LEFT) {
		mouse_pressed.x = mouse_x;
		mouse_pressed.y = mouse_y;
	}
	if (event.button == SDL_BUTTON_RIGHT) {
		mouse_pressed.x = mouse_x;
		mouse_pressed.y = mouse_y;
	}
}

void Application::OnMouseButtonUp( SDL_MouseButtonEvent event )
{
	int mouse_x = lround(mouse_position.x);
	int mouse_y = lround(mouse_position.y);
	if (event.button == SDL_BUTTON_LEFT) {
		framebuffer.DrawLineBresenham(mouse_pressed.x, mouse_pressed.y, mouse_x, mouse_y, c);
	}
	if (event.button == SDL_BUTTON_RIGHT) {
		framebuffer.DrawLineDDA(mouse_pressed.x, mouse_pressed.y, mouse_x, mouse_y, c);
	}
}

void Application::OnMouseMove(SDL_MouseButtonEvent event)
{
	if (event.button == SDL_BUTTON_LEFT) {

	}
}

void Application::OnFileChanged(const char* filename)
{ 
	Shader::ReloadSingleShader(filename);
}