// Paddle.cpp - implementation of the Paddle class.
// (c) Nikolay Prodanov, Juelich, summer 2013.
#include "Paddle.h"
#include "Ball.h"

Paddle::Paddle(std::size_t idExt, vector_3d shiftCenter, vector_3d n,
	float r, float h, float a, float top, float front,
	vector_3d ambient, vector_3d diffuse, vector_3d specular, float shine, float alpha) : 
	Shape(idExt, shiftCenter, ambient, diffuse, specular, shine, alpha), 
	vNormal(n), flRadius(r), flHeight(h), angle(a), maxCoordTop(top), maxCoordFront(front),
	slices(32), stacks(32)
{
	vVelocity = vector_3d(0., 0., 0.);
	// Red spot-related.
	bDrawSpot = false;
	vSpot = vCenter;

	quadratic = gluNewQuadric();
}

Paddle::~Paddle(void)
{
	gluDeleteQuadric(quadratic);	// Important: memory leak!
}

float Paddle::getSize() const
{
	return flRadius;
}

void Paddle::draw()
{
	glPushMatrix();	

	// !Move quadrics creation to the constructor!
	//GLUquadricObj *quadratic;
	//quadratic = gluNewQuadric();
	gluQuadricDrawStyle (quadratic, GLU_FILL); 
	//gluQuadricNormals (quadratic, GLU_SMOOTH);
	glTranslatef(vCenter[0]+vNormal[0]*0.5*flHeight, vCenter[1], vCenter[2]);	// Translate to the wall.
	//glTranslatef(-0.5*flHeight, vCenter[1], vCenter[2]);
	glRotatef(angle, 0.0f, 1.0f, 0.0f);

	// Draw collision spot.
	if(bDrawSpot){
		drawSpot();
	}

	material.setValues();	
	gluCylinder(quadratic, flRadius, flRadius, flHeight, slices, stacks);
	
	// Inner face of the paddle sylinder.
	glPushMatrix();
	// Move by the cylinder thikness, from point -0.5*h to the point 0.5*h in z(x) direction.
	glTranslatef(0., 0., flHeight);
	drawCircle(flRadius);
	glPopMatrix();

	// Outer face of the paddle sylinder.
	glPushMatrix();
	//glTranslatef(0., 0., -0.5*flHeight);
	drawCircle(flRadius);
	glPopMatrix();

	float alpha = 8., beta = 0.2;		// !Make these member variables!
	// Thin part of the handle.
	glPushMatrix();
	glTranslatef(0., 0., -alpha*flHeight);
	gluCylinder(quadratic, beta*flRadius, beta*flRadius, alpha*flHeight, slices, stacks);
	glPopMatrix();

	// Handle
	glPushMatrix();
	glTranslatef(0., 0., -alpha*flHeight);
	glutSolidSphere (2*beta*flRadius, slices, stacks);
	glPopMatrix();

	// Move quadrics deletion to the destructor!
	//gluDeleteQuadric(quadratic);	// Important: memory leak!


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
		if( ptInPaddle(c) )	{			// If the collision point is inside the paddle area.
			s->setVelocity(vNormal);

			if(bPlaySound)
				::playSound(system, sound, 0);
		}
	}
}

bool Paddle::ptInPaddle(const vector_3d &pt) const
{
	if( (pt[1] >= vCenter[1] - flRadius) && (pt[1] <= vCenter[1] + flRadius) && 
		(pt[2] >= vCenter[2] - flRadius) && (pt[2] <= vCenter[2] + flRadius))
		return true;
	return false;
}

void Paddle::drawSpot()
{
	glPushMatrix();
	vector_3d ambient = vector_3d(1.0, 0.0, 0.0);
	vector_3d diffuse = vector_3d(1.0, 0.0, 0.0);
	vector_3d specular = vector_3d(1.0, 0.0, 0.0);
	float alpha = 1.0;	// Opaque ball.
	float shine = 0.;
	Material m(ambient, diffuse, specular, shine, alpha);
	m.setValues();

	glTranslatef(0., 0., 1.01*flHeight);
	drawCircle(0.1*flRadius);
	glPopMatrix();
}

/*----*/
// ComputerPaddle implementation.
ComputerPaddle::ComputerPaddle(std::size_t idExt, vector_3d shiftCenter, vector_3d n,
	float r, float h, float a, float top, float front,
	vector_3d ambient, vector_3d diffuse, vector_3d specular, float shine, float alpha) : 
	Paddle(idExt, shiftCenter, n, r, h, a, top, front,
		ambient, diffuse, specular, shine, alpha)
{
	flVelocity = 0.05;
}

ComputerPaddle::~ComputerPaddle(void)
{
}

void ComputerPaddle::move(float deltaTime, vector_3d dr, bool bReset)
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
		// React only if the spot is
		//if(vSpot[0] > 0.0){
			// Move the paddle to the projections of the collision spot on the yz plane.
			vector_3d deltaSpot = vCollisionSpot  - vCenter;

			vCenter[1] += flVelocity*deltaSpot[1];
			vCenter[2] += flVelocity*deltaSpot[2];
		//}
	}
}

void ComputerPaddle::collide(Shape * s)
{
	Paddle::collide(s);
	vCollisionSpot = ((Ball*) s)->getCollisionSpot();
}