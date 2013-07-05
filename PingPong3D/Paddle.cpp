#include "Paddle.h"


Paddle::Paddle(std::size_t idExt, vector_3d shiftCenter, vector_3d n,
	float r, float h, float a, float top, float front):
id(idExt), vCenter(shiftCenter), vNormal(n), flRadius(r), flHeight(h), angle(a), 
	maxCoordTop(top), maxCoordFront(front),
	slices(32), stacks(32)
{
	vVelocity = vector_3d(0., 0., 0.);
}

Paddle::~Paddle(void)
{
}

std::size_t Paddle::getId() const
{
	return id;
}

vector_3d Paddle::getCenter() const
{ 
	return vCenter;
}

float Paddle::getSize() const
{
	return flRadius;
}

void Paddle::draw()
{
	glPushMatrix();

	glMatrixMode (GL_MODELVIEW);	
	GLUquadricObj *quadratic;
	quadratic = gluNewQuadric();
	//glRotatef(angle, 0.0f, 1.0f, 0.0f);		// Rotate around y (vertical) axis.
	glTranslatef(vCenter[0]-flHeight, vCenter[1], vCenter[2]);	// Translate to the wall.
	//glTranslatef(-0.5*flHeight, vCenter[1], vCenter[2]);
	glRotatef(angle, 0.0f, 1.0f, 0.0f);
	gluCylinder(quadratic, flRadius, flRadius, flHeight, slices, stacks);

	gluDeleteQuadric(quadratic);	// Important: memory leak!

	glPopMatrix();
}

void Paddle::move(float deltaTime, vector_3d dr, bool bReset)
{	
	if( vCenter[1] + flRadius >= maxCoordTop ){		// Top constraint.
		vVelocity = vector_3d(0., 0., 0.);
		vCenter[1] = maxCoordTop - 1.01*flRadius;
	}
	else if( vCenter[1] - flRadius <= -maxCoordTop ){	// Bottom constraint.
		vVelocity = vector_3d(0., 0., 0.);
		vCenter[1] = -maxCoordTop + 1.01*flRadius;
	}
	else if( vCenter[2] + flRadius >= maxCoordFront ){		// Front constraint.
		vVelocity = vector_3d(0., 0., 0.);
		vCenter[2] = maxCoordFront - 1.01*flRadius;
	}
	else if( vCenter[2] - flRadius <= -maxCoordFront ){	// Back constraint.
		vVelocity = vector_3d(0., 0., 0.);
		vCenter[2] = -maxCoordFront + 1.01*flRadius;
	}
	else
	{
		//vVelocity += dr;
		//vCenter += vVelocity;
		vCenter += dr;
	}
}

void Paddle::collide(Shape *s)
{
	// If it is not absorbing, then handle collisions
	vector_3d c = s->getCenter();
	float r = s->getSize();

	vector_3d surfacePoint = vCenter;
	surfacePoint[0] -= 0.5*flHeight*vNormal[0];

	// Find the smallest distance between the wall and the ball.
	float a = cml::dot(surfacePoint, vNormal);
	float b = cml::dot(vNormal*r+c,vNormal);

	if(b > a){	// Collision is probable to occur.
		if( ptInPaddle(c) )				// If the collision point is inside the paddle area.
			s->setVelocity(vNormal);
	}
}

bool Paddle::ptInPaddle(const vector_3d &pt) const
{
	if( (pt[1] >= vCenter[1] - flRadius) && (pt[1] <= vCenter[1] + flRadius) && 
		(pt[2] >= vCenter[2] - flRadius) && (pt[2] <= vCenter[2] + flRadius))
		return true;
	return false;
}