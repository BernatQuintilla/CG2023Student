#include "application.h"
#include "mesh.h"
#include "shader.h"
#include "utils.h" 
#include "entity.h"
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
	this->camera = new Camera();
	Mesh mesh1 = Mesh();
	Mesh mesh2 = Mesh();
	Mesh mesh3 = Mesh();
	mesh1.LoadOBJ("meshes/lee.obj");
	mesh2.LoadOBJ("meshes/anna.obj");
	mesh3.LoadOBJ("meshes/cleo.obj");

	entity1 = new Entity(mesh1, c); 
	entity2 = new Entity(mesh2, Color(255,0,0));
	entity3 = new Entity(mesh3, Color(0, 0, 255));

	entity2->modelMatrix.TranslateLocal(-0.55, -0.2, 0);
	Vector3 v2 = Vector3(0, -0.5, 0);
	entity2->modelMatrix.RotateLocal(1, v2);

	entity3->modelMatrix.TranslateLocal(0.55, -0.2, 0);
	Vector3 v3 = Vector3(0, 0.5, 0);
	entity3->modelMatrix.RotateLocal(1, v3);
}

Application::~Application()
{
	SDL_DestroyWindow(window);
}

void Application::Init(void)
{
	std::cout << "Initiating app..." << std::endl;
	entity1->Render(&framebuffer, camera, entity1->c);
	entity2->Render(&framebuffer, camera, entity2->c);
	entity3->Render(&framebuffer, camera, entity3->c);
}

// Render one frame
void Application::Render(void)
{
	framebuffer.Render();
}

// Called after render
void Application::Update(float seconds_elapsed)
{

}

//keyboard press event 
void Application::OnKeyPressed( SDL_KeyboardEvent event )
{
	// KEY CODES: https://wiki.libsdl.org/SDL2/SDL_Keycode
	switch(event.keysym.sym) {
		case SDLK_ESCAPE: exit(0); break; // ESC key, kill the app
	}
}

void Application::OnMouseButtonDown( SDL_MouseButtonEvent event )
{

}

void Application::OnMouseButtonUp( SDL_MouseButtonEvent event )
{

}

void Application::OnMouseMove(SDL_MouseButtonEvent event)
{
	if (event.button == SDL_BUTTON_LEFT) {

	}
}

void Application::OnWheel(SDL_MouseWheelEvent event)
{
	float dy = event.preciseY;

	// ...
}

void Application::OnFileChanged(const char* filename)
{ 
	Shader::ReloadSingleShader(filename);
}