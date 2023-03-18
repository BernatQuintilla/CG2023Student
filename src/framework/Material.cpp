#include "application.h"
#include "mesh.h"
#include "shader.h"
#include "utils.h" 
#include "entity.h"
#include "material.h"
#include <cmath>
#include <iostream>

void Material::Enable(const sUniformData& uniformdata) {
	this->shader->Enable();

	this->shader->SetFloat("u_shininess", this->Shininess);

	this->shader->SetVector3("u_Ka", this->Ka);
	this->shader->SetVector3("u_Kd", this->Kd);
	this->shader->SetVector3("u_Ks", this->Ks);

	this->shader->SetTexture("u_texture", this->texture);
	this->shader->SetTexture("u_normal_texture", this->normal_texture);
	this->shader->SetMatrix44("u_model", uniformdata.ModelMatrix);
	this->shader->SetMatrix44("u_viewprojection", uniformdata.CameraViewProjection);

	this->shader->SetVector3("u_Ia", uniformdata.Ia);
	this->shader->SetVector3("u_Id", uniformdata.Light.Id);
	this->shader->SetVector3("u_Is", uniformdata.Light.Is);
	this->shader->SetVector3("u_lightpos", uniformdata.Light.position);
	this->shader->SetVector3("u_eyepos", uniformdata.eyepos);
	this->shader->SetFloat("u_flag", this->flag);
}

void Material::Disable() {
	this->shader->Disable();
}