#include "Material.h"

// Default constructor.
// Make a blue material.
Material::Material(void) : 
	colorAmbient(0.0, 0.0, 0.2), colorDiffuse(0.0, 0.0, 1.0), colorSpecular(0.0, 0.0, 1.0),
	flShininess(0.0)
{
	setValues();
}

Material::Material(vector_3d ambient, vector_3d diffuse, vector_3d specular, float shine) :
colorAmbient(ambient), colorDiffuse(diffuse), colorSpecular(specular),
	flShininess(shine)
{
}

void Material::setValues()
{
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, &colorAmbient[0]);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, &colorDiffuse[0]);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, &colorSpecular[0]);
	glMaterialf(GL_FRONT, GL_SHININESS, flShininess);
}

Material::~Material(void)
{
}
