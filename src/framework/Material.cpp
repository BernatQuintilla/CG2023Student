#include "application.h"
#include "mesh.h"
#include "shader.h"
#include "utils.h" 
#include "entity.h"
#include "material.h"
#include <cmath>
#include <iostream>
Material::Material() {}

void Material::Enable() {
	this->shader->Enable();
}

void Material::Disable() {
	this->shader->Disable();
}