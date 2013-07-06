// Shape.h - abstract Shape class declaration.
// (c) Nikolay Prodanov, Juelich, summer 2013.
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

// Maybe move id, vCenter, vNormal to this class, as protected members.
class Shape
{
public:
	Shape(std::size_t idExt, vector_3d c);
	virtual ~Shape(void);

	// Methods that are not supposed to be overridden.
	std::size_t getId() const;
	vector_3d getCenter() const;// Return the coordinates of the Shape's center.

	// Virtual methods.
	virtual void move(float deltaTime, vector_3d dr, bool bReset);		// Virtual function, default: do nothing.
	virtual void collide(Shape * s);		// Detect collision with another Shape.
	virtual void setVelocity(vector_3d n);	// Change the direction of the velocity.

	// Pure virtual methods, all children have to implement them.
	virtual void draw() = 0;				// Draw itself.
	virtual float getSize() const = 0;		// Returns characteristic size.

protected:
	std::size_t id;
	vector_3d vCenter;
};

