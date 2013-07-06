// Wall.cpp - implementation of the Wall class.
// (c) Nikolay Prodanov, Juelich, summer 2013.
#include "Wall.h"

Wall::Wall(std::size_t idExt, vector_3d center, float w, float h, vector_3d n):
	Shape(idExt, center), width(w), height(h), vNormal(n), numOfVerteces(4)
{
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
	if( std::fabs(vNormal[0]) > 0.0f ) {
		v[0] = vector_3d(vCenter[i], vCenter[j] + 0.5*height, vCenter[k] - 0.5*width);
		v[1] = vector_3d(vCenter[i], vCenter[j] + 0.5*height, vCenter[k] + 0.5*width);
		v[2] = vector_3d(vCenter[i], vCenter[j] - 0.5*height, vCenter[k] + 0.5*width);
		v[3] = vector_3d(vCenter[i], vCenter[j] - 0.5*height, vCenter[k] - 0.5*width);
	}
	else if( std::fabs(vNormal[1]) > 0.0 ) {
		v[0] = vector_3d(vCenter[i]- 0.5*width, vCenter[j] , vCenter[k] + 0.5*height);
		v[1] = vector_3d(vCenter[i]+ 0.5*width, vCenter[j] , vCenter[k] + 0.5*height);
		v[2] = vector_3d(vCenter[i]+ 0.5*width, vCenter[j] , vCenter[k] - 0.5*height);
		v[3] = vector_3d(vCenter[i]- 0.5*width, vCenter[j] , vCenter[k] - 0.5*height);
	}
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

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glBegin(GL_POLYGON);
	//glEdgeFlag(GL_TRUE);
    glVertex3f(v[0][0],v[0][1],v[0][2]);
    //glEdgeFlag(GL_FALSE);
    glVertex3f(v[1][0],v[1][1],v[1][2]);
    //glEdgeFlag(GL_TRUE);	
    glVertex3f(v[2][0],v[2][1],v[2][2]);
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
	if(b >= a){	// Collision occurred.
		s->setVelocity(vNormal);
	}
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
	}
}

AbsorbingWall::~AbsorbingWall(void)
{
}