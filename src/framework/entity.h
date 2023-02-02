#pragma once

#include "main/includes.h"
#include "framework.h"
#include "mesh.h"
class Mesh;

class Entity {
public:
	int modelMatrix[4][4];
	Mesh mesh;
	//Constructors
	Entity();
	Entity(int matrix[4][4], Mesh obj);
	Entity(int matrix[4][4]);
	Entity(Mesh obj);
	//Sets
	void setModelMatrix(int matrix[4][4]);
	void setMesh(Mesh obj);
	//Gets
	int getModelMatrixValue(int r, int c);
	Mesh getMesh();
	//3.2
	void Render(Image* framebuffer, Camera* camera, const Color& c);
};