#include "entity.h"
#include "mesh.h"
#include "shader.h"
#include "utils.h" 
#include "image.h"
#include <cmath>
#include <iostream>

//Constructors
Entity::Entity() {
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			modelMatrix[i][j] = 0;
		}
	}
	mesh = Mesh();
}
Entity::Entity(int matrix[4][4], Mesh obj) {
	setModelMatrix(matrix);
	setMesh(obj);
}
Entity::Entity(int matrix[4][4]) {
	setModelMatrix(matrix);
	mesh = Mesh();
}
Entity::Entity(Mesh obj) {
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			modelMatrix[i][j] = 0;
		}
	}
	setMesh(obj);
}
//Sets
void Entity::setModelMatrix(int matrix[4][4]) {
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			modelMatrix[i][j] = matrix[i][j];
		}
	}
}
void Entity::setMesh(Mesh obj) {
	mesh = obj;
}
//Gets
int Entity::getModelMatrixValue(int r, int c) {
	return modelMatrix[r][c];
}
Mesh Entity::getMesh() {
	return mesh;
}
//3.2
/*void Render(Image* framebuffer, Camera* camera, const Color& c) {

}*/