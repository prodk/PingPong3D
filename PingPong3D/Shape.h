
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

class Shape
{
public:
	Shape(void);
	virtual ~Shape(void);
	virtual void draw() = 0;	// Pure virtual function, all childs have to implement it.
	virtual void move(float deltaTime);		// Virtual function, default: do nothing.
	virtual void collide(Shape * s);
	virtual vector_3d getCenter() const = 0;
	virtual float getSize() const = 0;		// Returns characteristic size.
	virtual void setVelocity(vector_3d n);	// Change the direction of the velocity.
};

