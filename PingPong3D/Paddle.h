// Paddle.h - declaration of the Paddle class.
// (c) Nikolay Prodanov, Juelich, summer 2013.

#pragma once

#include "Shape.h"

class Paddle : public Shape
{
public:
	Paddle(std::size_t idExt, vector_3d shiftCenter, vector_3d n, 
				float r, float h, float a, float top, float front);
	~Paddle(void);

	// Overriden virtual funcions.
	std::size_t getId() const;
	vector_3d getCenter() const;
	float getSize() const;				// Return radius of the cylinder.
	void draw();
	void move(float deltaTime, vector_3d dr, bool bReset);
	void collide(Shape *);

private:
	std::size_t id;
	vector_3d vCenter;
	vector_3d vNormal;
	float flRadius;
	float flHeight;
	const float angle;

	float maxCoordTop;
	float maxCoordFront;

	const int slices;
	const int stacks;

	vector_3d vVelocity;

	// Private methods
private:
	bool ptInPaddle(const vector_3d &pt) const;
};

