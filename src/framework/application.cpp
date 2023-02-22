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
	this->zBuffer.Resize(w, h);
	this->zBuffer.Fill(99999);
	this->camera = new Camera();
	this->camera->fov = 100;
	this->camera->near_plane = 0.01;
	this->camera->far_plane = 100;
	this->camera->aspect = framebuffer.width / framebuffer.height;
	this->camera->type = 0;
	this->camera->SetPerspective(camera->fov, camera->aspect, camera->near_plane, camera->far_plane);
	Mesh mesh1 = Mesh();
	mesh1.LoadOBJ("meshes/lee.obj");
	Mesh mesh2 = Mesh();
	mesh2.LoadOBJ("meshes/anna.obj");
	Mesh mesh3 = Mesh();
	mesh3.LoadOBJ("meshes/cleo.obj");

	this->entity1 = new Entity(mesh1, c); 
	this->entity2 = new Entity(mesh2, Color(255, 0, 0));
	this->entity3 = new Entity(mesh3, Color(0, 0, 255));

	entity1->modelMatrix.TranslateLocal(0, 0, -0.8);
	entity1->texture = new Image();
	entity1->texture->LoadTGA("textures/lee_color_specular.tga", true);

	entity2->texture = new Image();
	entity2->modelMatrix.TranslateLocal(-0.55, -0.2, -0.8);
	//entity2->texture = nullptr;
	entity2->texture->LoadTGA("textures/anna_color_specular.tga", true);
	Vector3 v2 = Vector3(0, -0.5, 0);
	entity2->modelMatrix.RotateLocal(1, v2);

	entity3->texture = new Image();
	entity3->modelMatrix.TranslateLocal(0.55, -0.2, -0.8);
	//entity3->texture = nullptr;
	entity3->texture->LoadTGA("textures/cleo_color_specular.tga", true);
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
	printf("type of projection: Perspective\n");
}

// Render one frame
void Application::Render(void)
{
	entity1->Render(&framebuffer, camera,&zBuffer);
	entity2->Render(&framebuffer, camera, &zBuffer);
	entity3->Render(&framebuffer, camera, &zBuffer);
	framebuffer.Render();
}

// Called after render
void Application::Update(float seconds_elapsed)
{
	/*entity1->Render(&framebuffer, camera, Color(0, 0, 0));
	entity1->modelMatrix.RotateLocal(0.03, up1);*/

	/*entity2->Render(&framebuffer, camera, Color(0, 0, 0));
	entity2->modelMatrix.RotateLocal(0.03, up2);

	entity3->Render(&framebuffer, camera, Color(0, 0, 0));
	entity3->modelMatrix.RotateLocal(0.03, up3);*/
}

//keyboard press event 
void Application::OnKeyPressed( SDL_KeyboardEvent event )
{
	// KEY CODES: https://wiki.libsdl.org/SDL2/SDL_Keycode
	switch(event.keysym.sym) {
		case SDLK_ESCAPE: exit(0); break; // ESC key, kill the app
		case SDLK_o: camera->type = 1; framebuffer.Fill(Color(0, 0, 0)); this->camera->SetOrthographic(camera->left, camera->right, camera->top, camera->bottom, camera->near_plane, camera->far_plane); printf("type of projection: Orthographic\n"); break;
		case SDLK_p: camera->type = 0; framebuffer.Fill(Color(0, 0, 0)); this->camera->SetPerspective(camera->fov, camera->aspect, camera->near_plane, camera->far_plane); printf("type of projection: Perspective\n"); break;
		case SDLK_w: if (camera->type == 0) { camera->fov += 10; framebuffer.Fill(Color(0, 0, 0)); camera->SetPerspective(camera->fov, camera->aspect, camera->near_plane, camera->far_plane); }; break;
		case SDLK_s: if (camera->type == 0) { camera->fov -= 10;  framebuffer.Fill(Color(0, 0, 0)); camera->SetPerspective(camera->fov, camera->aspect, camera->near_plane, camera->far_plane); }; break;
		case SDLK_c: entity1->texture = nullptr; break;
		case SDLK_t: entity1->texture = new Image(); entity1->texture->LoadTGA("textures/lee_color_specular.tga"); break;
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
}

void Application::OnMouseButtonUp( SDL_MouseButtonEvent event )
{
	int mouse_x = lround(mouse_position.x);
	int mouse_y = lround(mouse_position.y);
	if (event.button == SDL_BUTTON_LEFT) {
		/*if (camera->type == 0) {
			if (mouse_x - mouse_pressed.x < mouse_x) {
				camera->right += (mouse_x - mouse_pressed.x);
				this->camera->SetPerspective(camera->fov, camera->aspect, camera->near_plane, camera->far_plane);
			}
			else if (mouse_x - mouse_pressed.x > mouse_x) {
				camera->left += (mouse_x - mouse_pressed.x);
				this->camera->SetPerspective(camera->fov, camera->aspect, camera->near_plane, camera->far_plane);
			}
			if (mouse_y - mouse_pressed.y < mouse_y) {
				camera->top += (mouse_y - mouse_pressed.y);
				this->camera->SetPerspective(camera->fov, camera->aspect, camera->near_plane, camera->far_plane);
			}
			else if (mouse_y - mouse_pressed.y > mouse_y) {
				camera->bottom += (mouse_y - mouse_pressed.y);
				this->camera->SetPerspective(camera->fov, camera->aspect, camera->near_plane, camera->far_plane);
			}
		}
		camera->center.v[0] += (mouse_x - mouse_pressed.x);
		camera->center.v[1] += (mouse_y - mouse_pressed.y);*/
	}
}

void Application::OnMouseMove(SDL_MouseButtonEvent event)
{
	if (event.button == SDL_BUTTON_LEFT) {
		
	}
}

void Application::OnWheel(SDL_MouseWheelEvent event)
{
	float dy = event.preciseY;
	if (dy > 0) {
		if (camera->type == 0) {
			if (camera->fov > 58) {
				framebuffer.Fill(Color(0, 0, 0));
				camera->SetPerspective(camera->fov-3, camera->aspect, camera->near_plane, camera->far_plane);
				SetWindowSize(framebuffer.width, framebuffer.height);
			}
		}else if(camera->type == 1){
			if (camera->near_plane > -35) {
				framebuffer.Fill(Color(0, 0, 0));
				camera->SetOrthographic(camera->left, camera->right, camera->top, camera->bottom, camera->near_plane - 0.4, camera->far_plane - 1);
				SetWindowSize(framebuffer.width, framebuffer.height);
			
			}
		}
	}
	else if(dy<0){
		if (camera->type == 0) {
			if (camera->fov < 169) {
				framebuffer.Fill(Color(0, 0, 0));
				camera->SetPerspective(camera->fov + 3, camera->aspect, camera->near_plane, camera->far_plane);
				SetWindowSize(framebuffer.width, framebuffer.height);
			}
		}
		else if (camera->type == 1) {
			framebuffer.Fill(Color(0, 0, 0));
			camera->SetOrthographic(camera->left, camera->right, camera->top, camera->bottom, camera->near_plane + 0.4, camera->far_plane + 1);
			SetWindowSize(framebuffer.width, framebuffer.height);
		}
	}
}

void Application::OnFileChanged(const char* filename)
{ 
	Shader::ReloadSingleShader(filename);
}