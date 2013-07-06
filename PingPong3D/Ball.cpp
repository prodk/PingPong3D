// Ball.cpp - implementation of the Ball class.
// (c) Nikolay Prodanov, Juelich, summer 2013.
#include "Ball.h"

Ball::Ball(std::size_t idExt, vector_3d center, float r, vector_3d velocity):
		Shape(idExt,center),radius(r),vVelocity(velocity),slices(32),stacks(32)
{
}

Ball::~Ball(void)
{
}

float Ball::getSize() const
{
	return radius;
}

void Ball::draw()
{
	glPushMatrix();		// Save current matrix on the stack.

	//glColor3f (0.0, 1.0, 0.0);	
	glTranslatef(vCenter[0], vCenter[1], vCenter[2]);	// Move tha ball.
	glutSolidSphere (radius, slices, stacks);

	glPopMatrix();		// Restore the matrix stack.
}

void Ball::move(float deltaTime, vector_3d dr, bool bReset)
{
	// Don't use timer for robust movement!
	// Reason: timer has a separate thread, which continues working even when
	// the main GUI threads is paused (e.g. when the window is resized). 
	// This can lead to very big values of deltaTime.
	
	if(bReset)
		vCenter = vector_3d(0., 0., 0.);
	else
		vCenter = vCenter + vVelocity*2;	
}

// If shape is null, then multiply v by -n.
void Ball::collide(Shape * s)
{
	// if(s == 0) v*=-n;
	//glLoadIdentity ();             // Clear the current matrix.
	// x
	//if(vCenter[0]+radius > 1.0f)
		//vVelocity[0] *= -1.0f;
	//if(vCenter[0]-radius < -1.0f)
		//vVelocity[0] *= -1.0f;

	// y
	//if(vCenter[1]+radius > 1.0f)
		//vVelocity[1] *= -1.0f;
	//if(vCenter[1]-radius < -1.0f)
		//vVelocity[1] *= -1.0f;

	// z
	//if(vCenter[2]+radius > 1.0f)
		//vVelocity[2] *= -1.0f;
	//if(vCenter[2]-radius < -1.0f)
		//vVelocity[2] *= -1.0f;
}

void Ball::setVelocity(vector_3d n)
{
	if( std::fabs(n[0]) > 0.0f )
		vVelocity[0] *= -1.0f;
	else if( std::fabs(n[1]) > 0.0f )
		vVelocity[1] *= -1.0f;
	if( std::fabs(n[2]) > 0.0f )
		vVelocity[2] *= -1.0f;
}