// Ball.h - declaration of Ball class.
// (c) Nikolay Prodanov, Juelich, summer 2013.
#pragma once

#include "Shape.h"

// Ball is a Shape - hence public inheritance.
class Ball : public Shape
{
public:
	Ball(std::size_t idExt, vector_3d center, float radius, vector_3d velocity,
		vector_3d ambient, vector_3d diffuse, vector_3d specular, float shine, float alpha);
	~Ball(void);

	// Overriden virtual functions.
	float getSize() const;			// Radius of the ball.
	void draw();					// Render a ball using OGL.
	void move(float deltaTime, 
		vector_3d dr, bool bReset);	// Change center position. bReset - set vCenter to origin.
	void collide(Shape *);			// Do nothing for Ball.
	void setVelocity(vector_3d n);	// Reverse the velocity after a collision.
	vector_3d getVelocity() const;

private:	// id and vCenter are inherited from the Shape.
	float radius;
	vector_3d vVelocity;

	// OpenGL-specific constants.
	const int slices;				// Slices to render a sphere.
	const int stacks;	
};

