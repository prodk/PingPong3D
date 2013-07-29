// Material.cpp - implementation of the Material class.
// (c) Nikolay Prodanov, summer 2013, Juelich, Germany.
#include "Material.h"

// Default constructor.
Material::Material(void) : 
	colorAmbient(0.0, 0.5, 1.0), colorDiffuse(0.0, 0.5, 0.0), colorSpecular(0.0, 0.5, 0.0),
	flShininess(0.0), flAlpha(1.0)
{
	setValues();
}

// Overloaded constructor: specify custom theme.
Material::Material(vector_3d ambient, vector_3d diffuse, vector_3d specular, 
	float shine, float alpha) :
colorAmbient(ambient), colorDiffuse(diffuse), colorSpecular(specular),
	flShininess(shine), flAlpha(alpha)
{
}

void Material::setValues()
{
	GLfloat matAmbient[] = {colorAmbient[0], colorAmbient[1], colorAmbient[2], flAlpha};
	GLfloat matDiffuse[] = {colorDiffuse[0], colorDiffuse[1], colorDiffuse[2], flAlpha};
	GLfloat matSpecular[] = {colorSpecular[0], colorSpecular[1], colorSpecular[2], flAlpha};

	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, matAmbient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, matDiffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, matSpecular);
	glMaterialf(GL_FRONT, GL_SHININESS, flShininess);
}

Material::~Material(void)
{
}