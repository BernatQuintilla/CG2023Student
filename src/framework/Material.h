#pragma once
#include "main/includes.h"
#include <string.h>
#include <stdio.h>
#include <iostream>
#include "framework.h"
#include "shader.h"
#include "texture.h"
struct sLight {
	Vector3 position;
	Vector3 Id;
	Vector3 Is;
};
struct sUniformData {
	sLight Light;
	Matrix44 ModelMatrix;
	Matrix44 CameraViewProjection;
	Vector3 Ia;
	Vector3 eyepos;
};

class Material {
public:
	//Attributes
	Shader* shader;
	Texture* texture;
	Texture* normal_texture;
	Vector3 Ka;
	Vector3 Kd;
	Vector3 Ks;
	float Shininess;
	float flag;

	//Constructor
	Material() { shader = nullptr; texture = nullptr; Ka = (0, 0, 0); Kd = (0, 0, 0); Ks = (0, 0, 0); Shininess = 0; }

	//Methods
	void Enable(const sUniformData& uniformdata);
	void Disable();

};

