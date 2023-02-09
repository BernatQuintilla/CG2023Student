#include "entity.h"
#include "mesh.h"
#include "shader.h"
#include "utils.h" 
#include "image.h"
#include <cmath>
#include <iostream>

//3.2
void Entity::Render(Image* framebuffer, Camera* camera, const Color& c) {
	std::vector<Vector3> vs = mesh.GetVertices();
	Wireframe(framebuffer, camera, c, vs);
}


void Entity::Wireframe(Image* framebuffer, Camera* camera, const Color& c,std::vector<Vector3> vs) {
	bool ib,jb,kb;
	for (int i = 0;i < vs.size()-2;i+=3){
		//local -> world
		Vector3 v_world_i = modelMatrix * vs[i];
		Vector3 v_world_j = modelMatrix * vs[i+1];
		Vector3 v_world_k = modelMatrix * vs[i+2];
		
		//world->clip
		Vector3 v_clip_i = camera->ProjectVector(v_world_i, ib);
		Vector3 v_clip_j = camera->ProjectVector(v_world_j,jb);
		Vector3 v_clip_k = camera->ProjectVector(v_world_k, kb);

		//clip->screen
		v_clip_i.x = (v_clip_i.x+1)/2 * framebuffer->width;
		v_clip_i.y = (v_clip_i.y + 1) / 2 * framebuffer->height;
		v_clip_j.x = (v_clip_j.x+1) / 2 * framebuffer->width;
		v_clip_j.y = (v_clip_j.y + 1) / 2 * framebuffer->height;
		v_clip_k.x = (v_clip_k.x + 1) / 2 * framebuffer->width;
		v_clip_k.y = (v_clip_k.y + 1) / 2 * framebuffer->height;
		
		if (!(ib || jb || kb)) {
			framebuffer->DrawLineBresenham(floor(v_clip_i.x), floor(v_clip_i.y), floor(v_clip_j.x), floor(v_clip_j.y), c);
			framebuffer->DrawLineBresenham(floor(v_clip_j.x), floor(v_clip_j.y), floor(v_clip_k.x), floor(v_clip_k.y), c);
			framebuffer->DrawLineBresenham(floor(v_clip_k.x), floor(v_clip_k.y), floor(v_clip_i.x), floor(v_clip_i.y), c);
		}
	}
}

void Entity::Update(float seconds_elapsed) {
	Matrix44 rotation_matrix;
	rotation_matrix.GetRotationOnly();
	for (int i = 0; i < mesh.vertices.size() - 2; i += 3) {
		mesh.vertices[i] = rotation_matrix.RotateVector(mesh.vertices[i]);
	}
}