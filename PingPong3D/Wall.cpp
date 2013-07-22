// Wall.cpp - implementation of the Wall and AbsorbingWall classes.
// (c) Nikolay Prodanov, Juelich, summer 2013.
#include "Wall.h"
#include "Ball.h"

Wall::Wall(std::size_t idExt, vector_3d center, float w, float h, vector_3d n):
	Shape(idExt, center), width(w), height(h), vNormal(n), numOfVerteces(4)
{
	polygonMode = GL_LINE;

	// Red spot-related.
	bDrawSpot = false;
	vSpot = vCenter;		// Reconsider this!

	setupVertices();
}

Wall::Wall(std::size_t idExt, vector_3d center, float w, float h, vector_3d n,
	vector_3d ambient, vector_3d diffuse, vector_3d specular, float shine, float alpha) : 
	Shape(idExt, center, ambient, diffuse, specular, shine, alpha), 
	width(w), height(h), vNormal(n), numOfVerteces(4)
{
	polygonMode = GL_FILL;
	setupVertices();
}

Wall::~Wall(void)
{
}

void Wall::setupVertices()
{
	v.resize(numOfVerteces);	// Adjust the array to store all the vertices.

	int i = 0, j = 1, k = 2;
	// We assume that walls are perpendicular to axes.
	// Normal to x, width in z, height in y.
	if( std::fabs(vNormal[0]) > 0.0f ) {
		v[0] = vector_3d(vCenter[i], vCenter[j] + 0.5*height, vCenter[k] - 0.5*width);
		v[1] = vector_3d(vCenter[i], vCenter[j] + 0.5*height, vCenter[k] + 0.5*width);
		v[2] = vector_3d(vCenter[i], vCenter[j] - 0.5*height, vCenter[k] + 0.5*width);
		v[3] = vector_3d(vCenter[i], vCenter[j] - 0.5*height, vCenter[k] - 0.5*width);
	}
	// Normal to y, width in x, height in z.
	else if( std::fabs(vNormal[1]) > 0.0 ) {
		v[0] = vector_3d(vCenter[i]- 0.5*width, vCenter[j] , vCenter[k] + 0.5*height);
		v[1] = vector_3d(vCenter[i]+ 0.5*width, vCenter[j] , vCenter[k] + 0.5*height);
		v[2] = vector_3d(vCenter[i]+ 0.5*width, vCenter[j] , vCenter[k] - 0.5*height);
		v[3] = vector_3d(vCenter[i]- 0.5*width, vCenter[j] , vCenter[k] - 0.5*height);
	}
	// Normal to z, width in x, height in y.
	else if( std::fabs(vNormal[2]) > 0.0 ) {
		v[0] = vector_3d(vCenter[i]- 0.5*width, vCenter[j] + 0.5*height, vCenter[k] );
		v[1] = vector_3d(vCenter[i]+ 0.5*width, vCenter[j] + 0.5*height, vCenter[k] );
		v[2] = vector_3d(vCenter[i]+ 0.5*width, vCenter[j] - 0.5*height, vCenter[k] );
		v[3] = vector_3d(vCenter[i]- 0.5*width, vCenter[j] - 0.5*height, vCenter[k] );
	}
}

float Wall::getSize() const
{
	return width;
}

void Wall::draw()
{
	//glColor3f (0.0, 0.0, 1.0);

	// Draw collision spot.
	if(bDrawSpot){
		drawSpot();
	}

	material.setValues();

	// Set negatives normals (pointing inside the box) such that the light is reflected.
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glPolygonMode(GL_FRONT_AND_BACK, polygonMode);
	glBegin(GL_POLYGON);
	//glEdgeFlag(GL_TRUE);
	glNormal3f(-vNormal[0], -vNormal[1], -vNormal[2]);
    glVertex3f(v[0][0],v[0][1],v[0][2]);
    //glEdgeFlag(GL_FALSE);
	glNormal3f(-vNormal[0], -vNormal[1], -vNormal[2]);
    glVertex3f(v[1][0],v[1][1],v[1][2]);
    //glEdgeFlag(GL_TRUE);	
	glNormal3f(-vNormal[0], -vNormal[1], -vNormal[2]);
    glVertex3f(v[2][0],v[2][1],v[2][2]);

	glNormal3f(-vNormal[0], -vNormal[1], -vNormal[2]);
	glVertex3f(v[3][0],v[3][1],v[3][2]);
	glEnd();
}

void Wall::move(float deltaTime, vector_3d dr, bool bReset)
{
}

void Wall::collide(Shape * s)
{
	// Get the center and the radius of the ball.
	vector_3d c = s->getCenter();
	float r = s->getSize();

	// Find the position of the wall and of the ball's surface 
	// along the wall's normal direction.
	float a = cml::dot(vCenter, vNormal);
	float b = cml::dot(vNormal*r+c,vNormal);

	// If ball's surface is beyond the wall, then handle collision.
	if(b > a){	// Collision occurred.
		s->setVelocity(vNormal);

		if(bPlaySound)			// Maybe use channels for managing sounds!
			::playSound(system, sound, 0);
	}

	// Define whether to draw the intersection point.
	bDrawSpot = spotOnWall(s);
	if(bDrawSpot)
		((Ball*)s)->setCollisionSpot(vSpot);
}

void Wall::drawSpot()
{
	glPushMatrix();
	// Red opaque spot.
	vector_3d ambient = vector_3d(0.0, 1.0, 0.0);
	vector_3d diffuse = vector_3d(0.0, 1.0, 0.0);
	vector_3d specular = vector_3d(0.0, 1.0, 0.0);
	float alpha = 1.0;	// Opaque ball.
	float shine = 0.;
	Material m(ambient, diffuse, specular, shine, alpha);
	m.setValues();

	glTranslatef(vSpot[0], vSpot[1], vSpot[2]);
	//drawCircle(0.009*height);
	glutSolidSphere (0.005*height, 32, 32);
	glPopMatrix();
}

bool Wall::ptInWall(const vector_3d &pt) const
{
	// If normal is to x, then check y and z.
	if( std::fabs(vNormal[0]) > 0.0f ){
		if( (pt[1] >= vCenter[1] - 0.5*height) &&
			(pt[1] <= vCenter[1] + 0.5*height) &&
			(pt[2] >= vCenter[2] - 0.5*width) &&
			(pt[2] <= vCenter[2] + 0.5*width)
		  )
		  return true;
	}

	// If normal is to y, then check x and z.
	if( std::fabs(vNormal[1]) > 0.0f ){
		if( (pt[2] >= vCenter[2] - 0.5*height) &&
			(pt[2] <= vCenter[2] + 0.5*height) &&
			(pt[0] >= vCenter[0] - 0.5*width) &&
			(pt[0] <= vCenter[0] + 0.5*width)
		  )
		  return true;
	}

	// If normal is to z, then check x and y.
	if( std::fabs(vNormal[2]) > 0.0f ){
		if( (pt[1] >= vCenter[1] - 0.5*height) &&
			(pt[1] <= vCenter[1] + 0.5*height) &&
			(pt[0] >= vCenter[0] - 0.5*width) &&
			(pt[0] <= vCenter[0] + 0.5*width)
		  )
		  return true;
	}

	return false;
}

bool Wall::spotOnWall(Shape *s)
{
	bool result = false;

	vector_3d c = s->getCenter();

	vector_3d vel = s->getVelocity();
	float denom = cml::dot(vel, vNormal);
	if(denom > 0)
	{
		float nom = cml::dot(vCenter - c, vNormal);
		if(nom != 0)
		{
			vSpot = (nom/denom)*vel + c;
			if(ptInWall(vSpot))
				result = true;
		}
	}// end if denom!=0

	return result;
}


// AbsorbingWall implementation.
AbsorbingWall::AbsorbingWall(std::size_t idExt, vector_3d center, float w, float h, vector_3d n):
Wall(idExt, center, w, h, n)
{
}

void AbsorbingWall::collide(Shape *s)
{
	// Get the center and the radius of the ball.
	vector_3d c = s->getCenter();
	float r = s->getSize();

	// Find the position of the wall and of the ball's surface 
	// along the wall's normal direction.
	float a = cml::dot(vCenter, vNormal);
	float b = cml::dot(vNormal*r+c,vNormal);

	if(b > a){	// Collision occurred, reset Ball's position
		c = vector_3d(0.0f, 0.0f, 0.0f);
		float dt = 0.;
		bool bReset = true;
		s->setVelocity(vNormal);			
		s->move(dt, c, bReset);
		if(bPlaySound)			// Maybe use channels for managing sounds!
			::playSound(system, sound, 0);
		//playSound();
	}

	// Reconsider this code duplication from Wall!
	// Define whether to draw the intersection point.
	bDrawSpot = spotOnWall(s);
	if(bDrawSpot)
		((Ball*)s)->setCollisionSpot(vSpot);
}

AbsorbingWall::~AbsorbingWall(void)
{
}

void AbsorbingWall::drawSpot()
{
	glPushMatrix();
	// Red opaque spot.
	vector_3d ambient = vector_3d(1.0, 0.0, 0.0);
	vector_3d diffuse = vector_3d(1.0, 0.0, 0.0);
	vector_3d specular = vector_3d(1.0, 0.0, 0.0);
	float alpha = 1.0;	// Opaque ball.
	float shine = 0.;
	Material m(ambient, diffuse, specular, shine, alpha);
	m.setValues();

	glTranslatef(vSpot[0], vSpot[1], vSpot[2]);
	//drawCircle(0.009*height);
	glutSolidSphere (0.008*height, 32, 32);
	glPopMatrix();
}