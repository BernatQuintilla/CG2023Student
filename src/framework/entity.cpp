#include "entity.h"
#include "mesh.h"
#include "shader.h"
#include "utils.h" 
#include "image.h"
#include <cmath>
#include <iostream>

//3.2
void Entity::Render(Image* framebuffer, Camera* camera, FloatImage* zBuffer) {
	std::vector<Vector3> vs = mesh.GetVertices();
	std::vector<Vector2> uvs = mesh.GetUVs();
	zBuffer->Fill(99999);
	switch (mode) {
		case 1:
			Wireframe(framebuffer, camera, vs, zBuffer, uvs);
			break;
		case 2:
			WireframeColor(framebuffer, camera, vs);
			break;
		case 3:
			WireframeMulticolor(framebuffer, camera, vs, zBuffer);
			break;
		default:
			break;
	}
}

void Entity::RenderColor(Image* framebuffer, Camera* camera) {
	std::vector<Vector3> vs = mesh.GetVertices();
	WireframeColor(framebuffer, camera, vs);

}

void Entity::RenderMulticolor(Image* framebuffer, Camera* camera, FloatImage* zBuffer) {
	std::vector<Vector3> vs = mesh.GetVertices();
	WireframeMulticolor(framebuffer, camera, vs, zBuffer);

}

void Entity::Update(float seconds_elapsed) {
	modelMatrix.RotateLocal(0.03, (0, 1, 0));
}

void Entity::Wireframe(Image* framebuffer, Camera* camera,std::vector<Vector3> vs, FloatImage* zBuffer, std::vector<Vector2> uvs) {
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
		
		if (ib || jb || kb)
			continue;

		/*framebuffer->DrawLineBresenham(floor(v_clip_i.x), floor(v_clip_i.y), floor(v_clip_j.x), floor(v_clip_j.y), c);
		framebuffer->DrawLineBresenham(floor(v_clip_j.x), floor(v_clip_j.y), floor(v_clip_k.x), floor(v_clip_k.y), c);
		framebuffer->DrawLineBresenham(floor(v_clip_k.x), floor(v_clip_k.y), floor(v_clip_i.x), floor(v_clip_i.y), c);*/

		v1.Set(v_clip_i.x, v_clip_i.y,v_clip_i.z);
		v2.Set(v_clip_j.x, v_clip_j.y,v_clip_j.z);
		v3.Set(v_clip_k.x, v_clip_k.y,v_clip_k.z);

		Vector2 uv0 = uvs[i];
		Vector2 uv1 = uvs[i + 1];
		Vector2 uv2 = uvs[i + 2];
		
		framebuffer->DrawTriangleInterpolated(v1, v2, v3, Color(255,0,0),Color(0,255,0),Color(0,0,255),zBuffer,texture, uv0, uv1, uv2);
	}
}
void Entity::WireframeColor(Image* framebuffer, Camera* camera, std::vector<Vector3> vs) {
	bool ib, jb, kb;
	for (int i = 0; i < vs.size() - 2; i += 3) {
		//from local to world
		Vector3 v_world_i = modelMatrix * vs[i];
		Vector3 v_world_j = modelMatrix * vs[i + 1];
		Vector3 v_world_k = modelMatrix * vs[i + 2];

		//from world to clip
		Vector3 v_clip_i = camera->ProjectVector(v_world_i, ib);
		Vector3 v_clip_j = camera->ProjectVector(v_world_j, jb);
		Vector3 v_clip_k = camera->ProjectVector(v_world_k, kb);

		//from clip to screen (centramos mesh en el framebuffer -> convert range from [-1,1] to [(0,w-1),(0,h-1)])
		v_clip_i.x = (v_clip_i.x + 1) / 2 * framebuffer->width;
		v_clip_i.y = (v_clip_i.y + 1) / 2 * framebuffer->height;
		v_clip_j.x = (v_clip_j.x + 1) / 2 * framebuffer->width;
		v_clip_j.y = (v_clip_j.y + 1) / 2 * framebuffer->height;
		v_clip_k.x = (v_clip_k.x + 1) / 2 * framebuffer->width;
		v_clip_k.y = (v_clip_k.y + 1) / 2 * framebuffer->height;

		if (ib || jb || kb)
			continue;

		/*framebuffer->DrawLineBresenham(floor(v_clip_i.x), floor(v_clip_i.y), floor(v_clip_j.x), floor(v_clip_j.y), c);
		framebuffer->DrawLineBresenham(floor(v_clip_j.x), floor(v_clip_j.y), floor(v_clip_k.x), floor(v_clip_k.y), c);
		framebuffer->DrawLineBresenham(floor(v_clip_k.x), floor(v_clip_k.y), floor(v_clip_i.x), floor(v_clip_i.y), c);*/

		Vector2 v1(v_clip_i.x, v_clip_i.y);
		Vector2 v2(v_clip_j.x, v_clip_j.y);
		Vector2 v3(v_clip_k.x, v_clip_k.y);
		
		framebuffer->DrawTriangle(v1, v2, v3, Color(255, 0, 0));
	}
}
void Entity::WireframeMulticolor(Image* framebuffer, Camera* camera, std::vector<Vector3> vs, FloatImage* zBuffer) {
	bool ib, jb, kb;
	for (int i = 0; i < vs.size() - 2; i += 3) {
		//from local to world
		Vector3 v_world_i = modelMatrix * vs[i];
		Vector3 v_world_j = modelMatrix * vs[i + 1];
		Vector3 v_world_k = modelMatrix * vs[i + 2];

		//from world to clip
		Vector3 v_clip_i = camera->ProjectVector(v_world_i, ib);
		Vector3 v_clip_j = camera->ProjectVector(v_world_j, jb);
		Vector3 v_clip_k = camera->ProjectVector(v_world_k, kb);

		//from clip to screen (centramos mesh en el framebuffer -> convert range from [-1,1] to [(0,w-1),(0,h-1)])
		v_clip_i.x = (v_clip_i.x + 1) / 2 * framebuffer->width;
		v_clip_i.y = (v_clip_i.y + 1) / 2 * framebuffer->height;
		v_clip_j.x = (v_clip_j.x + 1) / 2 * framebuffer->width;
		v_clip_j.y = (v_clip_j.y + 1) / 2 * framebuffer->height;
		v_clip_k.x = (v_clip_k.x + 1) / 2 * framebuffer->width;
		v_clip_k.y = (v_clip_k.y + 1) / 2 * framebuffer->height;

		if (ib || jb || kb)
			continue;

		/*framebuffer->DrawLineBresenham(floor(v_clip_i.x), floor(v_clip_i.y), floor(v_clip_j.x), floor(v_clip_j.y), c);
		framebuffer->DrawLineBresenham(floor(v_clip_j.x), floor(v_clip_j.y), floor(v_clip_k.x), floor(v_clip_k.y), c);
		framebuffer->DrawLineBresenham(floor(v_clip_k.x), floor(v_clip_k.y), floor(v_clip_i.x), floor(v_clip_i.y), c);*/

		v1.Set(v_clip_i.x, v_clip_i.y, v_clip_i.z);
		v2.Set(v_clip_j.x, v_clip_j.y, v_clip_j.z);
		v3.Set(v_clip_k.x, v_clip_k.y, v_clip_k.z);

		framebuffer->DrawTriangleInterpolatedMulticolor(v1, v2, v3, Color(255, 0, 0), Color(0, 255, 0), Color(0, 0, 255), zBuffer);
	}
}