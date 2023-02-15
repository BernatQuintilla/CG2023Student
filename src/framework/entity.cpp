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
		//from local to world
		Vector3 v_world_i = modelMatrix * vs[i];
		Vector3 v_world_j = modelMatrix * vs[i+1];
		Vector3 v_world_k = modelMatrix * vs[i+2];
		
		//from world to clip
		Vector3 v_clip_i = camera->ProjectVector(v_world_i, ib);
		Vector3 v_clip_j = camera->ProjectVector(v_world_j,jb);
		Vector3 v_clip_k = camera->ProjectVector(v_world_k, kb);

		//from clip to screen (centramos mesh en el framebuffer -> convert range from [-1,1] to [(0,w-1),(0,h-1)])
		v_clip_i.x = (v_clip_i.x+1) / 2 * framebuffer->width;
		v_clip_i.y = (v_clip_i.y+1) / 2 * framebuffer->height;
		v_clip_j.x = (v_clip_j.x+1) / 2 * framebuffer->width;
		v_clip_j.y = (v_clip_j.y+1) / 2 * framebuffer->height;
		v_clip_k.x = (v_clip_k.x+1) / 2 * framebuffer->width;
		v_clip_k.y = (v_clip_k.y+1) / 2 * framebuffer->height;
		
		if ((ib || jb || kb)==false) { //negZ
			framebuffer->DrawLineBresenham(floor(v_clip_i.x), floor(v_clip_i.y), floor(v_clip_j.x), floor(v_clip_j.y), c);
			framebuffer->DrawLineBresenham(floor(v_clip_j.x), floor(v_clip_j.y), floor(v_clip_k.x), floor(v_clip_k.y), c);
			framebuffer->DrawLineBresenham(floor(v_clip_k.x), floor(v_clip_k.y), floor(v_clip_i.x), floor(v_clip_i.y), c);
		}
		Vector2 v0(v_clip_i.x, v_clip_i.y);
		Vector2 v1(v_clip_j.x, v_clip_j.y);
		Vector2 v2(v_clip_k.x, v_clip_k.y);
		framebuffer->DrawTriangle(v0, v1, v2, c);
	}
}

void Entity::Update(float seconds_elapsed) {
	modelMatrix.RotateLocal(0.03, (0,1,0));
}