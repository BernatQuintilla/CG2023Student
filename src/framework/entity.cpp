#include "entity.h"
#include "mesh.h"
#include "shader.h"
#include "utils.h" 
#include "image.h"
#include <cmath>
#include <iostream>

//3.2
void Render(Image* framebuffer, Camera* camera, const Color& c,std::vector<Vector3> vs) {
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
		
		if ((ib || jb || kb) == true) {
			framebuffer->DrawLineBresenham(v_clip_i.x, v_clip_i.y, v_clip_j.x, v_clip_j.y, c);
			framebuffer->DrawLineBresenham(v_clip_j.x, v_clip_j.y, v_clip_k.x, v_clip_k.y, c);
			framebuffer->DrawLineBresenham(v_clip_k.x, v_clip_k.y, v_clip_i.x, v_clip_i.y, c);
		}
	}
}