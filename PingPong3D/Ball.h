// Ball.h - declaration of Ball class.
// (c) Nikolay Prodanov, Juelich, summer 2013.
#ifndef BALL_H
#define BALL_H

#include "Shape.h"

// Ball is a Shape - hence public inheritance.
class Ball : public Shape
{
public:
	Ball(std::size_t idExt, vector_3d center, float radius, vector_3d velocity, float dv,
		vector_3d ambient, vector_3d diffuse, vector_3d specular, float shine, float alpha);
	~Ball(void);

	// Overriden virtual functions.
	float getSize() const;			// Radius of the ball.
	void draw();					// Render a ball using OGL.
	void move(float deltaTime, 
		vector_3d dr, bool bReset);	// Change center position. bReset - set vCenter to origin.
	//bool collide(Shape *);			// Do nothing for Ball.
	void setVelocity(vector_3d n);	// Reverse the velocity after a collision.
	vector_3d getVelocity() const;
	void setCollisionSpot(vector_3d & spot);// Save the next collision point, used for AI.
	vector_3d getCollisionSpot() const;

private:	// id and vCenter are inherited from the Shape.
	float radius;
	vector_3d vVelocity;
	vector_3d vSpot;
	float flDeltaVel;

	// OpenGL-specific constants.
	const int slices;				// Slices to render a sphere.
	const int stacks;	
};

#endif // BALL_H