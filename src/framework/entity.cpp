#include "entity.h"
#include "mesh.h"
#include "shader.h"
#include "utils.h" 
#include "image.h"
#include <cmath>
#include <iostream>

void Entity::Render(sUniformData uniformdata) {
	uniformdata.ModelMatrix = this->modelMatrix;
	this->material->Enable(uniformdata);
	mesh.Render();
	this->material->Disable();
}
void Entity::Render2(void) {
	mesh.Render();
}