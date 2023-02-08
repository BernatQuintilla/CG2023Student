#pragma once

#include "main/includes.h"
#include "framework.h"
#include "mesh.h"
class Mesh;

class Entity {
public:
	Matrix44 modelMatrix;
	Mesh mesh;
	Color c;
	//Constructors
	Entity() {
		modelMatrix = NULL;
		mesh = Mesh();
		c = Color(255,255,255);
	}
	Entity(Matrix44 matrix, Mesh obj,Color co) {
		modelMatrix = matrix;
		mesh = obj;
		c = co;
	}
	Entity(Matrix44 matrix,Color co) {
		modelMatrix = matrix;
		mesh = Mesh();
		c = co;
	}
	Entity(Mesh obj, Color co) {
		mesh = obj;
		c = co;
		modelMatrix = NULL;
	}
	//Sets
	void setModelMatrix(Matrix44 matrix) {
		modelMatrix = matrix;
	}
	void setMesh(Mesh obj) {
		mesh = obj;
	}
	
	Mesh getMesh() {
		return mesh;
	}
	//3.2
	void PreRender(Image* framebuffer, Camera* camera, const Color& c);
	void PreRender(Image* framebuffer, Camera* camera, const Color& c, std::vector<Vector3> vs);
};