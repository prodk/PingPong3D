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
	glTranslatef(vCenter[0], vCenter[1], vCenter[2]);
	glutSolidSphere (radius, slices, stacks);

	glPopMatrix();		// Restore the matrix stack.
}

void Ball::move(float deltaTime)
{
	vCenter = vCenter + vVelocity*10;
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