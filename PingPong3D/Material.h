// Material.h - declaration of the Material class.
// (c) Nikolay Prodanov, summer 2013, Juelich, Germany.

// Here we also define TEXTURE structure.
// Most of the headers files are included here, because Material.h is included 
// in the Shape.h, and Shape is used in most of the other classes.
#ifndef MATERIAL_H
#define MATERIAL_H

// Libraries/APIs.
#ifdef WIN32			// Additional include on Windows.
#include <Windows.h>
#endif

// Libraries.
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
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
	explicit Material(vector_3d ambient, vector_3d diffuse, vector_3d specular, 
		float shine, float alpha);
	~Material(void);

	void setValues();

private:
	vector_3d colorAmbient;
	vector_3d colorDiffuse;
	vector_3d colorSpecular;
	float flShininess;
	float flAlpha;
};

typedef struct
{
    GLuint id;
    int width, height;
} TEXTURE;

#endif // MATERIAL_H