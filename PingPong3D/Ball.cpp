// Ball.cpp - implementation of the Ball class.
// (c) Nikolay Prodanov, summer 2013, Juelich, Germany.
#include "Ball.h"

Ball::Ball(std::size_t idExt, vector_3d center, float r, vector_3d velocity, float dv,
	vector_3d ambient, vector_3d diffuse, vector_3d specular, float shine, float alpha,
	float maxv) :
Shape(idExt, center, ambient, diffuse, specular, shine, alpha), 
	radius(r), vVelocity(velocity), flDeltaVel(dv), slices(32), stacks(32), 
	flMaxVel(maxv)
{
	vVelocityNew = vVelocity;
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

	GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT1, GL_SPECULAR, light_specular);
	GLfloat light_position[] = { 0.0, 0.0, 0.0, 1.0 };
	GLfloat light_direction[] = { vVelocityNew[0], vVelocityNew[1], vVelocityNew[2], 0.0 };
	
	glTranslatef(vCenter[0], vCenter[1], vCenter[2]);	// Move the ball.
	glutSolidSphere (radius, slices, stacks);

	// Set a spotlight moving with the ball.
	glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 50.0);			// Spotlight width.
	glLightfv(GL_LIGHT1, GL_POSITION, light_position);
	glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, light_direction);

	glPopMatrix();		// Restore the matrix stack.
}

void Ball::move(vector_3d dr, bool bReset)
{
	// Don't use timers for movement!
	// Reason: a timer has a separate thread, which continues working even when
	// the main GUI threads is paused (e.g. when the window is resized). 
	// This can lead to very big values of deltaTime.

	// Move the ball to the specified location if bReset is true.
	if(bReset){
		vCenter = dr;
		if( cml::dot(dr, dr) < 1.e-04 )
			// If start new play, then assign random velocity.
			vVelocityNew = vector_3d(
				::generateRandomSignInterval(0.9*flMaxVel, flMaxVel), 
				::generateRandomSignInterval(0.9*flMaxVel, flMaxVel), 
				::generateRandomSignInterval(0.9*flMaxVel, flMaxVel)
			);
	}
	else
		vCenter = vCenter + vVelocityNew;
}

void Ball::setVelocity(vector_3d n, float factor)	// Reflect and rescale the velocity.
{
	// Check which component should be reflected 
	// (depending on the normal to the shape with which collision occurs).
	if( std::fabs(n[0]) > 0.0f )
		vVelocityNew[0] *= -(1. + factor);
	else if( std::fabs(n[1]) > 0.0f )
		vVelocityNew[1] *= -(1. + factor);
	if( std::fabs(n[2]) > 0.0f )
		vVelocityNew[2] *= -(1. + factor);
}

vector_3d Ball::getVelocity() const
{
	return vVelocityNew;
}

void Ball::setCollisionSpot(const vector_3d & spot)
{
	vSpot = spot;
}

vector_3d Ball::getCollisionSpot() const
{
	return vSpot;
}