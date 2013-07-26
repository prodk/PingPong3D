// Paddle.cpp - implementation of the Paddle class.
// (c) Nikolay Prodanov, Juelich, summer 2013.

#include "Paddle.h"
#include "Ball.h"

Paddle::Paddle(std::size_t idExt, vector_3d shiftCenter, vector_3d n,
	float r, float h, float a, float top, float front,
	vector_3d ambient, vector_3d diffuse, vector_3d specular, float shine, float alpha, float dv) : 
	Shape(idExt, shiftCenter, ambient, diffuse, specular, shine, alpha), 
	vNormal(n), flRadius(r), flHeight(h), angle(a), maxCoordTop(top), maxCoordFront(front),
	slices(32), stacks(32), numOfCircleVertices(360), flBallDeltaVel(dv)
{
	vVelocity = vector_3d(0., 0., 0.);
	vSpot = vCenter;
	quadratic = gluNewQuadric();
	generateCircle();				// Generate circle coordinates only once.
}

Paddle::~Paddle(void)
{
	gluDeleteQuadric(quadratic);	// Important: memory leak!
}

void Paddle::generateCircle()
{
	const float DEG2RAD = 3.14159/180;
	float degInRad;
	circleVerteces.resize(numOfCircleVertices);

	for (int i = 0; i < numOfCircleVertices; i++) {
		degInRad = i*DEG2RAD;
		circleVerteces[i] = vector_3d(cos(degInRad)*flRadius, sin(degInRad)*flRadius, 0.0);
	}
}

void Paddle::drawCircle()
{	
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glBegin(GL_POLYGON); 

	for (std::size_t i = 0; i < circleVerteces.size(); i++) {
		glNormal3f(0.0, 0.0, 1.0);
		glVertex3f(circleVerteces[i][0], circleVerteces[i][1], circleVerteces[i][2]);
	}

	glEnd();
}

float Paddle::getSize() const
{
	return flRadius;
}

void Paddle::draw()
{
	glPushMatrix();	
	
	gluQuadricDrawStyle (quadratic, GLU_FILL); 	
	glTranslatef(vCenter[0]+vNormal[0]*0.5*flHeight, vCenter[1], vCenter[2]);	// Translate to the wall.
	glRotatef(angle, 0.0f, 1.0f, 0.0f);

	material.setValues();	
	gluCylinder(quadratic, flRadius, flRadius, flHeight, slices, stacks);
	
	// Inner face of the paddle cylinder.
	glPushMatrix();
	// Move by the cylinder thikness, from point -0.5*h to the point 0.5*h in z(x) direction.
	glTranslatef(0., 0., flHeight);
	drawCircle();
	glPopMatrix();

	// Outer face of the paddle cylinder.
	glPushMatrix();
	drawCircle();
	glPopMatrix();

	float alpha = 8., beta = 0.2;		// !Make these member variables!
	// Thin part of the handle.
	glPushMatrix();
	glTranslatef(0., 0., -alpha*flHeight);
	gluCylinder(quadratic, beta*flRadius, beta*flRadius, alpha*flHeight, slices, stacks);
	glPopMatrix();

	// Handle.
	glPushMatrix();
	glTranslatef(0., 0., -alpha*flHeight);
	glutSolidSphere (2*beta*flRadius, slices, stacks);
	glPopMatrix();

	glPopMatrix();
}

void Paddle::move(vector_3d dr, bool bReset)
{	
	if( vCenter[1] + flRadius >= maxCoordTop ){			// Top constraint.
		vVelocity = vector_3d(0., 0., 0.);
		vCenter[1] = maxCoordTop - 1.01*flRadius;
	}
	else if( vCenter[1] - flRadius <= -maxCoordTop ){	// Bottom constraint.
		vVelocity = vector_3d(0., 0., 0.);
		vCenter[1] = -maxCoordTop + 1.01*flRadius;
	}
	else if( vCenter[2] + flRadius >= maxCoordFront ){	// Front constraint.
		vVelocity = vector_3d(0., 0., 0.);
		vCenter[2] = maxCoordFront - 1.01*flRadius;
	}
	else if( vCenter[2] - flRadius <= -maxCoordFront ){	// Back constraint.
		vVelocity = vector_3d(0., 0., 0.);
		vCenter[2] = -maxCoordFront + 1.01*flRadius;
	}
	else {
		vCenter += dr;
	}
}

bool Paddle::collide(Shape *s)
{
	bool collided = false;
	vector_3d c = s->getCenter();
	float r = s->getSize();

	vector_3d surfacePoint = vCenter;
	// Take into account, that paddle's surface doesn't coincide with the wall.
	surfacePoint[0] = surfacePoint[0] - vNormal[0]*flHeight;

	// Find the smallest distance between the wall and the ball.
	float a = cml::dot(surfacePoint, vNormal);
	float b = cml::dot(vNormal*r+c,vNormal);

	if(b >= a){	// Collision is probable to occur.
		if( ptInPaddle(c) )	{			// If the collision point is inside the paddle area.
			s->setVelocity(vNormal, flBallDeltaVel);// !Change the factor later.
			collided = true;
		}
	}

	return collided;
}

bool Paddle::ptInPaddle(const vector_3d &pt) const
{
	if( (pt[1] >= vCenter[1] - flRadius) && (pt[1] <= vCenter[1] + flRadius) && 
		(pt[2] >= vCenter[2] - flRadius) && (pt[2] <= vCenter[2] + flRadius))
		return true;
	return false;
}

/*______________________________*/
// ComputerPaddle implementation.
ComputerPaddle::ComputerPaddle(std::size_t idExt, vector_3d shiftCenter, vector_3d n,
	float r, float h, float a, float top, float front,
	vector_3d ambient, vector_3d diffuse, vector_3d specular, float shine, float alpha, float dv) : 
	Paddle(idExt, shiftCenter, n, r, h, a, top, front,
		ambient, diffuse, specular, shine, alpha, dv)
{
	flVelocity = 0.05;
}

ComputerPaddle::~ComputerPaddle(void)
{
}

void ComputerPaddle::move(vector_3d dr, bool bReset)
{
	if( vCenter[1] + flRadius >= maxCoordTop ){			// Top constraint.
		vVelocity = vector_3d(0., 0., 0.);
		vCenter[1] = maxCoordTop - 1.01*flRadius;
	}
	else if( vCenter[1] - flRadius <= -maxCoordTop ){	// Bottom constraint.
		vVelocity = vector_3d(0., 0., 0.);
		vCenter[1] = -maxCoordTop + 1.01*flRadius;
	}
	else if( vCenter[2] + flRadius >= maxCoordFront ){	// Front constraint.
		vVelocity = vector_3d(0., 0., 0.);
		vCenter[2] = maxCoordFront - 1.01*flRadius;
	}
	else if( vCenter[2] - flRadius <= -maxCoordFront ){	// Back constraint.
		vVelocity = vector_3d(0., 0., 0.);
		vCenter[2] = -maxCoordFront + 1.01*flRadius;
	}
	else {
		// Move the paddle to the projections of the collision spot on the yz plane.
		vector_3d deltaSpot = vCollisionSpot  - vCenter;
		vCenter[1] += flVelocity*deltaSpot[1];
		vCenter[2] += flVelocity*deltaSpot[2];
	}
}

bool ComputerPaddle::collide(Shape * s)
{
	bool collided = Paddle::collide(s);
	vCollisionSpot = ((Ball*) s)->getCollisionSpot();

	return collided;
}