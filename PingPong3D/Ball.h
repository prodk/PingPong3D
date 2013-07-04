#pragma once

#include "Shape.h"

//using namespace cml;

// Ball is a Shape.
class Ball :
	public Shape
{
public:
	Ball(std::size_t idExt, float radius, vector_3d center, vector_3d velocity);
	~Ball(void);
	std::size_t getId() const;

	// Overriden virtual functions.
	vector_3d getCenter() const;
	float getSize() const;			// Return radius of the ball.
	void draw();
	void move(float deltaTime);
	void collide(Shape *);	// If shape is null, then multiply v by -n.
	void setVelocity(vector_3d n);

private:
	std::size_t id;
	float radius;
	vector_3d vCenter;
	vector_3d vVelocity;

	// OpenGL-specific constants.
	const int slices;
	const int stacks;	
};

