// Ball.h - declaration of Ball class.
// (c) Nikolay Prodanov, Juelich, summer 2013.
#pragma once

#include "Shape.h"

// Ball is a Shape - hence public inheritance.
class Ball : public Shape
{
public:
	Ball(std::size_t idExt, float radius, vector_3d center, vector_3d velocity);
	~Ball(void);
	std::size_t getId() const;

	// Overriden virtual functions.
	vector_3d getCenter() const;	// Center position.
	float getSize() const;			// Radius of the ball.
	void draw();					// Render a ball using OGL.
	void move(float deltaTime, vector_3d dr, bool bReset);		// Change center position.
	void collide(Shape *);			// Do nothing for Ball.
	void setVelocity(vector_3d n);	// Reverse the velocity after a collision.

private:
	std::size_t id;
	float radius;
	vector_3d vCenter;
	vector_3d vVelocity;

	// OpenGL-specific constants.
	const int slices;				// Slices to render a sphere.
	const int stacks;	
};

