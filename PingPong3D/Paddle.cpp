// Paddle.cpp - implementation of the Paddle class.
// (c) Nikolay Prodanov, Juelich, summer 2013.
#include "Paddle.h"

Paddle::Paddle(std::size_t idExt, vector_3d shiftCenter, vector_3d n,
	float r, float h, float a, float top, float front,
	vector_3d ambient, vector_3d diffuse, vector_3d specular, float shine) : 
	Shape(idExt, shiftCenter, ambient, diffuse, specular, shine), 
	vNormal(n), flRadius(r), flHeight(h), angle(a), maxCoordTop(top), maxCoordFront(front),
	slices(32), stacks(32)
{
	vVelocity = vector_3d(0., 0., 0.);
}

Paddle::~Paddle(void)
{
}

float Paddle::getSize() const
{
	return flRadius;
}

void Paddle::draw()
{
	glPushMatrix();

	//glMatrixMode (GL_MODELVIEW);	
	/*glEnable (GL_DEPTH_TEST);
		glCullFace(GL_BACK);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);*/
	material.setValues();
	

	// !Move quadrics creation to the constructor!
	GLUquadricObj *quadratic;
	quadratic = gluNewQuadric();
	gluQuadricDrawStyle (quadratic, GLU_FILL); 
	//gluQuadricNormals (quadratic, GLU_SMOOTH);
	//glRotatef(angle, 0.0f, 1.0f, 0.0f);		// Rotate around y (vertical) axis.
	glTranslatef(vCenter[0]-0.5*flHeight, vCenter[1], vCenter[2]);	// Translate to the wall.
	//glTranslatef(-0.5*flHeight, vCenter[1], vCenter[2]);
	glRotatef(angle, 0.0f, 1.0f, 0.0f);
	gluCylinder(quadratic, flRadius, flRadius, flHeight, slices, stacks);
	//gluDisk(quadratic, flRadius, flRadius, slices, stacks);

	// Move quadrics deletion to the destructor!
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