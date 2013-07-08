
#pragma once

// Libraries/APIs.
#ifdef WIN32			// Additional include on Windows.
#include <Windows.h>
#endif

// Libraries.
#include <SDL.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#include <glut.h>
#include <cml/cml.h>

// 3-d column vector, fixed length, double coordinates:
typedef cml::vector< float, cml::fixed<3> > vector_3d;

class Material
{
public:
	Material(void);
	explicit Material(vector_3d ambient, vector_3d diffuse, vector_3d specular, float shine);
	~Material(void);

	// Methods.
	void setValues();

private:
	vector_3d colorAmbient;
	vector_3d colorDiffuse;
	vector_3d colorSpecular;
	float flShininess;

	
};

