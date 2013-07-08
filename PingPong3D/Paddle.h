// Paddle.h - declaration of the Paddle class.
// (c) Nikolay Prodanov, Juelich, summer 2013.

#pragma once			// Replace by #define for OS portability.

#include "Shape.h"

class Paddle : public Shape
{
public:
	Paddle(std::size_t idExt, vector_3d shiftCenter, vector_3d n, 
				float r, float h, float a, float top, float front,
				vector_3d ambient, vector_3d diffuse, vector_3d specular, float shine);
	~Paddle(void);

	// Overriden virtual funcions.
	float getSize() const;				// Return radius of the cylinder.
	void draw();
	void move(float deltaTime, vector_3d dr, bool bReset);
	void collide(Shape *);

private:
	vector_3d vNormal;
	float flRadius;
	float flHeight;
	const float angle;
	float maxCoordTop;
	float maxCoordFront;

	vector_3d vVelocity;

	// Rendering-related.
	//Material material;

	const int slices;
	const int stacks;

	// Private methods
private:
	bool ptInPaddle(const vector_3d &pt) const;

};

