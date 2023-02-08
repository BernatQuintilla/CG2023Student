#pragma once

#include "main/includes.h"
#include "framework.h"
#include "mesh.h"
#include "image.h"

class Mesh;

class Entity {
public:
	Matrix44 modelMatrix;
	Mesh mesh;
	Color c;
	//Constructors
	Entity() {
		modelMatrix = Matrix44();
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
		modelMatrix = Matrix44();
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
	void Render(Image* framebuffer, Camera* camera, const Color& c);
	void Wireframe(Image* framebuffer, Camera* camera, const Color& c, std::vector<Vector3> vs);
};