#include "application.h"
#include "mesh.h"
#include "shader.h"
#include "utils.h" 
#include "entity.h"
#include <cmath>
#include <iostream>

Shader* shader = nullptr;
Shader* gouraud = nullptr;
Mesh* mesh = nullptr;
Texture* texturefruits = nullptr;
Texture* textureFace = nullptr;

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
	Vector3 up = Vector3(0, 1, 0);
	Vector3 eye = Vector3(0, 0, 1);
	Vector3 center = Vector3(0, 0, 0);
	this->camera->LookAt(eye, center, up);
	this->camera->SetPerspective(100, (float)window_width / window_height, 0.01, 100);
	this->camera->type = 0;
	this->entity1 = new Entity(); 
	this->entity1->modelMatrix.SetIdentity();
	this->entity1->modelMatrix.SetTranslation(0,0,0);

	this->light.Id = Vector3(0.2);
	this->light.Is = Vector3(0.5);
	this->light.position = Vector3(0);
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
	entity1->mesh.setMesh(lee);
	textureFace = new Texture();
	textureFace->Load("textures/lee_color_specular.tga", false);
	
	shader = Shader::Get("shaders/raster.vs", "shaders/raster.fs");
	gouraud = Shader::Get("shaders/gouraud.vs", "shaders/gouraud.fs");
	MaterialEntity = new Material();
	MaterialEntity->shader = gouraud;
	MaterialEntity->texture = textureFace;
	MaterialEntity->Ka = 0.9;
	MaterialEntity->Kd = 0.9;
	MaterialEntity->Ks = 0.5;
	MaterialEntity->Shininess = 7;
	entity1->material = MaterialEntity;
}

// Render one frame
void Application::Render(void)
{
	if (flag){ glEnable(GL_DEPTH_TEST);
		/*shader->Enable();
		shader->SetFloat("u_task", task);
		shader->SetTexture("u_texture", textureFace);
		shader->SetMatrix44("u_model", entity1->modelMatrix);
		shader->SetMatrix44("u_viewprojection", this->camera->viewprojection_matrix);
		this->entity1->Render();
		shader->Disable();
		glDisable(GL_DEPTH_TEST);*/

		//glEnable(GL_DEPTH_TEST);

		uniformdata.Ia = this->Ia;
		uniformdata.CameraViewProjection = camera->GetViewProjectionMatrix();
		uniformdata.numLights = 1;
		uniformdata.Light = this->light;
		entity1->Render(uniformdata);

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
		case SDLK_d: if (task == 18) { task = 1; } else { task++; } break;
		case SDLK_a: if (task == 1) { task = 18; } else { task--; } break;
		case SDLK_UP: if (pixelate <= 1) { pixelate += 0.0025; } break;
		case SDLK_DOWN: if (pixelate >= 0) { pixelate -= 0.0025; } break;
		case SDLK_RIGHT: direction = 1; break;
		case SDLK_LEFT: direction = -1; break;
		case SDLK_b: if (flag) { flag = false; } else { flag = true; }
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