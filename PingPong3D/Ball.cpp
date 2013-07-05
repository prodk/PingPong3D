// Ball.cpp - implementation of the Ball class.
// (c) Nikolay Prodanov, Juelich, summer 2013.
#include "Ball.h"

Ball::Ball(std::size_t idExt, float r, vector_3d center, vector_3d velocity):
		id(idExt),radius(r),vCenter(center),vVelocity(velocity),slices(32),stacks(32)
{
}

Ball::~Ball(void)
{
}

std::size_t Ball::getId() const
{
	return id;
}

vector_3d Ball::getCenter() const
{
	return vCenter;
}

float Ball::getSize() const
{
	return radius;
}

void Ball::draw()
{
	glPushMatrix();		// Save current matrix on the stack.

	glColor3f (0.0, 1.0, 0.0);	
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
	//if( ( dot(vCenter, vCenter) > 1.e-06) && (dot(vCenter, n) < 1e-06 ) )	// if n is 0.
		//vCenter = n;
	//else
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