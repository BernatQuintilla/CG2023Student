#include <string.h>
#include <stdio.h>
#include <iostream>
#include "framework.h"
#include "shader.h"
#include "texture.h"

class Material {
	//Attributes
	Shader* shader;
	Texture* texture;
	Vector3* colorComponents;
	float Shininess;

	typedef struct sLight {
		float position;
		float Id;
		float Is;
	} sLight;
	//Constructor
	Material();
	//Methods
	void Enable();
	void Disable();
};
