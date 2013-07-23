// Ball.cpp - implementation of the Ball class.
// (c) Nikolay Prodanov, Juelich, summer 2013.
#include "Ball.h"

Ball::Ball(std::size_t idExt, vector_3d center, float r, vector_3d velocity, float dv,
	vector_3d ambient, vector_3d diffuse, vector_3d specular, float shine, float alpha):
		Shape(idExt, center, ambient, diffuse, specular, shine, alpha), 
		radius(r), vVelocity(velocity), flDeltaVel(dv), slices(32), stacks(32)
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

	material.setValues();

	//glColor3f (0.0, 1.0, 0.0);
	GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT1, GL_SPECULAR, light_specular);
	GLfloat light_position[] = { 0.0, 0.0, 0.0, 1.0 };
	GLfloat light_direction[] = { vVelocity[0], vVelocity[1], vVelocity[2], 0.0 };
	

	glTranslatef(vCenter[0], vCenter[1], vCenter[2]);	// Move tha ball.
	glutSolidSphere (radius, slices, stacks);	

	// Set a spotlight movint with the ball.
	glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 50.0);
	glLightfv(GL_LIGHT1, GL_POSITION, light_position);
	glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, light_direction);

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

void Ball::setVelocity(vector_3d n)
{
	if( std::fabs(n[0]) > 0.0f )
		vVelocity[0] *= -1.0f;
	else if( std::fabs(n[1]) > 0.0f )
		vVelocity[1] *= -1.0f;
	if( std::fabs(n[2]) > 0.0f )
		vVelocity[2] *= -1.0f;
}

vector_3d Ball::getVelocity() const
{
	return vVelocity;
}

void Ball::setCollisionSpot(vector_3d & spot)
{
	vSpot = spot;
}

vector_3d Ball::getCollisionSpot() const
{
	return vSpot;
}