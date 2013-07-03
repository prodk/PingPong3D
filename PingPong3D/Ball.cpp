#include "Ball.h"


Ball::Ball(unsigned int idExt, float r, vector_3d center, vector_3d velocity):
		id(idExt),radius(r),vCenter(center),vVelocity(velocity),slices(32),stacks(32)
{
}

Ball::~Ball(void)
{
}

std::size_t Ball::getId()
{
	return id;
}

void Ball::draw()
{
	glPushMatrix();		// Save current matrix on the stack.

	//glMatrixMode (GL_MODELVIEW);	// Switch to the modelview matrix.
	//glLoadIdentity ();				// Clean up the modelview matrix.
	glColor3f (0.0, 1.0, 0.0);
	glTranslatef(vCenter[0], vCenter[1], vCenter[2]);
	glutSolidSphere (radius, slices, stacks);

	glPopMatrix();		// Restore the matrix stack.
}

void Ball::move(float deltaTime)
{
	vCenter = vCenter + vVelocity*deltaTime;
}