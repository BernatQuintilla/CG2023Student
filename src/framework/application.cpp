#include "application.h"
#include "mesh.h"
#include "shader.h"
#include "utils.h" 
#include "entity.h"
#include <cmath>
#include <iostream>

Shader* shader = nullptr;
Shader* shader1 = nullptr;
Mesh* mesh = nullptr;
Texture* texture0 = nullptr;
Texture* textureStreet = nullptr;

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
	
	this->entity1 = new Entity(); 
}

Application::~Application()
{
}

void Application::Init(void)
{
	std::cout << "Initiating app..." << std::endl;
	printf("type of projection: Perspective\n");
	mesh = new Mesh();
	mesh->CreateQuad();
	Mesh* lee = new Mesh();
	lee->LoadOBJ("meshes/lee.obj");
	Texture* texture1 = new Texture();
	texture1->Load("textures/lee_color_specular.tga", false);
	entity1->mesh.setMesh(lee);
	entity1->textures = texture1;

	shader1 = Shader::Get("shaders/quad.vs", "shaders/quad.fs");
	texture0 = Texture::Get("/images/fruits.png");
	//Texture* texture2 = new Texture();
	//texture2->Load("images/street.png", false);
	//textureStreet = texture2;
	this->entity1->shaders = Shader::Get("shaders/raster.vs", "shaders/raster.fs");
}

// Render one frame
void Application::Render(void)
{
	// para 3D mallas: glEnable(GL_DEPTH_TEST)
	if (task < 16) {
		shader = shader1;
		shader->Enable();
		shader->SetFloat("u_time", time);
		shader->SetFloat("u_task", task);
		shader->SetTexture("u_texture", texture0);
		//shader->SetTexture("u_texture1", textureStreet);
		shader->SetFloat("u_pixelate", pixelate);
		shader->SetFloat("u_direction", direction);
		mesh->Render();
		shader->Disable();
	}
	else {
		glEnable(GL_DEPTH_TEST);
		shader = entity1->shaders;
		shader->Enable();
		shader->SetFloat("u_task", task);
		shader->SetTexture("u_texture", this->entity1->textures);
		shader->SetMatrix44("u_model", entity1->modelMatrix);
		shader->SetMatrix44("u_viewprojectionmatrix", this->camera->GetProjectionMatrix());
		this->entity1->Render();
		shader->Disable();
		glDisable(GL_DEPTH_TEST);
	}
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
		case SDLK_o: camera->type = 1; framebuffer.Fill(Color(0, 0, 0)); this->camera->SetOrthographic(camera->left, camera->right, camera->top, camera->bottom, camera->near_plane, camera->far_plane); printf("type of projection: Orthographic\n"); break;
		case SDLK_p: camera->type = 0; framebuffer.Fill(Color(0, 0, 0)); this->camera->SetPerspective(camera->fov, camera->aspect, camera->near_plane, camera->far_plane); printf("type of projection: Perspective\n"); break;
		case SDLK_w: if (camera->type == 0) { camera->fov += 10; framebuffer.Fill(Color(0, 0, 0)); camera->SetPerspective(camera->fov, camera->aspect, camera->near_plane, camera->far_plane); }; break;
		case SDLK_s: if (camera->type == 0) { camera->fov -= 10;  framebuffer.Fill(Color(0, 0, 0)); camera->SetPerspective(camera->fov, camera->aspect, camera->near_plane, camera->far_plane); }; break;
		case SDLK_d: if (task == 16) { task = 1; } else { task++; } break;
		case SDLK_a: if (task == 1) { task = 16; } else { task--; } break;
		case SDLK_UP: if (pixelate <= 1) { pixelate += 0.0025; } break;
		case SDLK_DOWN: if (pixelate >= 0) { pixelate -= 0.0025; } break;
		case SDLK_RIGHT: direction = 1; break;
		case SDLK_LEFT: direction = -1; break;
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