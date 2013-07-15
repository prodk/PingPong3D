// Paddle.h - declaration of the Paddle class.
// (c) Nikolay Prodanov, Juelich, summer 2013.

#pragma once			// Replace by #define for OS portability.

#include "Shape.h"

class Paddle : public Shape
{
public:
	Paddle(std::size_t idExt, vector_3d shiftCenter, vector_3d n, 
				float r, float h, float a, float top, float front,
				vector_3d ambient, vector_3d diffuse, vector_3d specular, 
				float shine, float alpha);
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
	bool bDrawSpot;
	vector_3d vSpot;

	GLUquadricObj *quadratic;

	const int slices;
	const int stacks;

	// Private methods
private:
	bool ptInPaddle(const vector_3d &pt) const;
	void drawSpot();

// Change this function later:
// Save vertices to an array and maybe use a display list.
// Add more points to the array.
// Maybe make it nonmember because it is also used in Wall.
void drawCircle(float radius)
{
	const float DEG2RAD = 3.14159/180;

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glBegin(GL_POLYGON); 
	for (float i=0.; i < 360.; i+=0.5)
	{
		float degInRad = i*DEG2RAD;
		glNormal3f(0.0, 0.0, 1.0);
		glVertex3f(cos(degInRad)*radius, sin(degInRad)*radius, 0.0);
	}

	glEnd();
}

};	// End Paddle.

