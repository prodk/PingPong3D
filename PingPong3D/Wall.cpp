#include "Wall.h"


Wall::Wall(std::size_t idExt, float w, float h, vector_3d center, vector_3d n):
	id(idExt), width(w), height(h), vCenter(center), vNormal(n), numOfVerteces(4)
{
	setupVertices();
}


Wall::~Wall(void)
{
}

void Wall::setupVertices()
{
	v.resize(numOfVerteces);

	int i = 0, j = 1, k = 2;
	// We assume that walls are perpendicular to axes.
	if( std::fabs(vNormal[0]) > 0.0f ) {
		//i = 0; j = 1; k = 2;
		v[0] = vector_3d(vCenter[i], vCenter[j] + 0.5*height, vCenter[k] - 0.5*width);
		v[1] = vector_3d(vCenter[i], vCenter[j] + 0.5*height, vCenter[k] + 0.5*width);
		v[2] = vector_3d(vCenter[i], vCenter[j] - 0.5*height, vCenter[k] + 0.5*width);
		v[3] = vector_3d(vCenter[i], vCenter[j] - 0.5*height, vCenter[k] - 0.5*width);
	}
	else if( std::fabs(vNormal[1]) > 0.0 ) {
		//i = 1; j = 0; k = 2;
		v[0] = vector_3d(vCenter[i]- 0.5*width, vCenter[j] , vCenter[k] + 0.5*height);
		v[1] = vector_3d(vCenter[i]+ 0.5*width, vCenter[j] , vCenter[k] + 0.5*height);
		v[2] = vector_3d(vCenter[i]+ 0.5*width, vCenter[j] , vCenter[k] - 0.5*height);
		v[3] = vector_3d(vCenter[i]- 0.5*width, vCenter[j] , vCenter[k] - 0.5*height);
	}
	else if( std::fabs(vNormal[2]) > 0.0 ) {
		//i = 2; j = 1; k = 0;
		v[0] = vector_3d(vCenter[i]- 0.5*width, vCenter[j] + 0.5*height, vCenter[k] );
		v[1] = vector_3d(vCenter[i]+ 0.5*width, vCenter[j] + 0.5*height, vCenter[k] );
		v[2] = vector_3d(vCenter[i]+ 0.5*width, vCenter[j] - 0.5*height, vCenter[k] );
		v[3] = vector_3d(vCenter[i]- 0.5*width, vCenter[j] - 0.5*height, vCenter[k] );
	}

	//v[0] = vector_3d(vCenter[i], vCenter[j] + 0.5*height, vCenter[k] - 0.5*width);
	//v[1] = vector_3d(vCenter[i], vCenter[j] + 0.5*height, vCenter[k] + 0.5*width);
	//v[2] = vector_3d(vCenter[i], vCenter[j] - 0.5*height, vCenter[k] + 0.5*width);
	//v[3] = vector_3d(vCenter[i], vCenter[j] - 0.5*height, vCenter[k] - 0.5*width);
}

std::size_t Wall::getId() const
{
	return id;
}

vector_3d Wall::getCenter() const
{
	return vCenter;
}

float Wall::getSize() const
{
	return width;
}

void Wall::draw()
{
	//glPushMatrix();		// Save current matrix on the stack.

	glColor3f (0.0, 0.0, 1.0);	
	//glTranslatef(vCenter[0], vCenter[1], vCenter[2]);
	//glutSolidSphere (radius, slices, stacks);

	//glPopMatrix();		// Restore the matrix stack.
	//glEnableClientState (GL_COLOR_ARRAY);
	//glEnableClientState (GL_VERTEX_ARRAY);
	//glColorPointer (3, GL_FLOAT, 0, colors);
	
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

void Wall::move(float deltaTime)
{
}

void Wall::collide(Shape * s)
{
	vector_3d c = s->getCenter();
	float r = s->getSize();

	// Find the shortest distance between the wall and the ball.
	float a = cml::dot(vCenter, vNormal);
	float b = cml::dot(vNormal*r+c,vNormal);
	//float dr = std::fabs( a ) - 
		       //std::fabs( b );

	if(b > a){	// Collision occurred.
		s->setVelocity(vNormal);
	}
}