#pragma once

#include "Shape.h"
#include <cml/cml.h>

//using namespace cml;

// 3-d column vector, fixed length, double coordinates:
typedef cml::vector< float, cml::fixed<3> > vector_3d;

// Ball is a Shape.
class Ball :
	public Shape
{
public:
	Ball(unsigned int idExt, float radius, vector_3d center, vector_3d velocity);
	~Ball(void);
	std::size_t getId();
	void draw();
	void move(float deltaTime);

private:
	std::size_t id;
	float radius;
	vector_3d vCenter;
	vector_3d vVelocity;

	// OpenGL-specific constants.
	const int slices;
	const int stacks;
};

