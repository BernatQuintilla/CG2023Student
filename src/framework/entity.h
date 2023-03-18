#pragma once

#include "main/includes.h"
#include "framework.h"
#include "mesh.h"
#include "image.h"
#include "shader.h"
#include "material.h"

class Mesh;

class Entity {
public:
	Matrix44 modelMatrix;
	Mesh mesh;
	Color c;
	Vector3 v1, v2, v3;
	Image* texture;
	int mode = 1;
	Texture* textures = nullptr;
	Material* material;

	//Constructors
	Entity() {
		modelMatrix = Matrix44();
		mesh = Mesh();
		c = Color(255,255,255);
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
	//Methods
	void Render(sUniformData uniformdata);
	void Render2(void);
};