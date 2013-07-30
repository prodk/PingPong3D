// Wall.cpp - implementation of the Wall and AbsorbingWall classes.
// (c) Nikolay Prodanov, summer 2013, Juelich, Germany.
#include "Wall.h"
#include "Ball.h"

Wall::Wall(std::size_t idExt, vector_3d center, float w, float h, vector_3d n):
	Shape(idExt, center), flWidth(w), flHeight(h), vNormal(n), numOfVerteces(4),
		slices(16), stacks(16)
{
	polygonMode = GL_LINE;
	// Spot-related.
	bDrawSpot = false;
	vSpot = vCenter;			// Set the spot to the center of the wall.
	setupVertices();
}

Wall::Wall(std::size_t idExt, vector_3d center, float w, float h, vector_3d n,
	vector_3d ambient, vector_3d diffuse, vector_3d specular, float shine, float alpha) : 
	Shape(idExt, center, ambient, diffuse, specular, shine, alpha), 
	flWidth(w), flHeight(h), vNormal(n), numOfVerteces(4),
	slices(16), stacks(16)
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
		v[0] = vector_3d(vCenter[i], vCenter[j] + 0.5*flHeight, vCenter[k] - 0.5*flWidth);
		v[1] = vector_3d(vCenter[i], vCenter[j] + 0.5*flHeight, vCenter[k] + 0.5*flWidth);
		v[2] = vector_3d(vCenter[i], vCenter[j] - 0.5*flHeight, vCenter[k] + 0.5*flWidth);
		v[3] = vector_3d(vCenter[i], vCenter[j] - 0.5*flHeight, vCenter[k] - 0.5*flWidth);
	}
	// Normal to y, width in x, height in z.
	else if( std::fabs(vNormal[1]) > 0.0 ) {
		v[0] = vector_3d(vCenter[i]- 0.5*flWidth, vCenter[j] , vCenter[k] + 0.5*flHeight);
		v[1] = vector_3d(vCenter[i]+ 0.5*flWidth, vCenter[j] , vCenter[k] + 0.5*flHeight);
		v[2] = vector_3d(vCenter[i]+ 0.5*flWidth, vCenter[j] , vCenter[k] - 0.5*flHeight);
		v[3] = vector_3d(vCenter[i]- 0.5*flWidth, vCenter[j] , vCenter[k] - 0.5*flHeight);
	}
	// Normal to z, width in x, height in y.
	else if( std::fabs(vNormal[2]) > 0.0 ) {
		v[0] = vector_3d(vCenter[i]- 0.5*flWidth, vCenter[j] + 0.5*flHeight, vCenter[k] );
		v[1] = vector_3d(vCenter[i]+ 0.5*flWidth, vCenter[j] + 0.5*flHeight, vCenter[k] );
		v[2] = vector_3d(vCenter[i]+ 0.5*flWidth, vCenter[j] - 0.5*flHeight, vCenter[k] );
		v[3] = vector_3d(vCenter[i]- 0.5*flWidth, vCenter[j] - 0.5*flHeight, vCenter[k] );
	}
}

float Wall::getSize() const
{
	return flWidth;
}

void Wall::draw()
{
	if(bDrawSpot){	// Draw collision spot.
		drawSpot();
	}

	material.setValues();

	// Set negative normals (pointing inside the box) such that the light is reflected.
	glPolygonMode(GL_FRONT_AND_BACK, polygonMode);
	glBegin(GL_POLYGON);

	glNormal3f(-vNormal[0], -vNormal[1], -vNormal[2]);
    glVertex3f(v[0][0],v[0][1],v[0][2]);    

	glNormal3f(-vNormal[0], -vNormal[1], -vNormal[2]);
    glVertex3f(v[1][0],v[1][1],v[1][2]);
    	
	glNormal3f(-vNormal[0], -vNormal[1], -vNormal[2]);
    glVertex3f(v[2][0],v[2][1],v[2][2]);

	glNormal3f(-vNormal[0], -vNormal[1], -vNormal[2]);
	glVertex3f(v[3][0],v[3][1],v[3][2]);
	
	glEnd();
}

bool Wall::collide(Shape * s)
{
	bool collided = false;
	// Get the center and the radius of the ball.
	vector_3d c = s->getCenter();
	float r = s->getSize();

	// The distance between the ball surface and the wall along the wall's normal.
	float a = cml::dot(vCenter, vNormal);
	float b = cml::dot(c,vNormal);
	float dr = a - b;

	// If ball's surface is beyond the wall, then handle collision.
	if(dr <= r){	// Collision occurred.
		s->setVelocity(vNormal, 0.);
		// !Very important - subtle bug that leads to instability:
		// !move the ball a bit back!!!
		c -= dr*vNormal;
		s->move(c, true);	// Reset is true to move to the c.
		collided = true;
	}

	// Define whether to draw the intersection point.
	bDrawSpot = spotOnWall(s);
	if(bDrawSpot)
		((Ball*)s)->setCollisionSpot(vSpot);	// Tell the ball where it is going to collide.

	return collided;
}

void Wall::drawSpot()
{
	glPushMatrix();
	// Green opaque spot.
	vector_3d ambient = vector_3d(0.0, 1.0, 0.0);
	vector_3d diffuse = vector_3d(0.0, 1.0, 0.0);
	vector_3d specular = vector_3d(0.0, 1.0, 0.0);
	float alpha = 1.0;	// Opaque ball.
	float shine = 0.;
	Material m(ambient, diffuse, specular, shine, alpha);
	m.setValues();

	glTranslatef(vSpot[0], vSpot[1], vSpot[2]);
	glutSolidSphere (0.005*flHeight, slices, stacks);
	glPopMatrix();
}

bool Wall::ptInWall(const vector_3d &pt) const
{
	// If normal is to x, then check y and z.
	if( std::fabs(vNormal[0]) > 0.0f ){
		if( (pt[1] >= vCenter[1] - 0.5*flHeight) &&
			(pt[1] <= vCenter[1] + 0.5*flHeight) &&
			(pt[2] >= vCenter[2] - 0.5*flWidth) &&
			(pt[2] <= vCenter[2] + 0.5*flWidth)
		  )
		  return true;
	}
	// If normal is to y, then check x and z.
	else if( std::fabs(vNormal[1]) > 0.0f ){
		if( (pt[2] >= vCenter[2] - 0.5*flHeight) &&
			(pt[2] <= vCenter[2] + 0.5*flHeight) &&
			(pt[0] >= vCenter[0] - 0.5*flWidth) &&
			(pt[0] <= vCenter[0] + 0.5*flWidth)
		  )
		  return true;
	}
	// If normal is to z, then check x and y.
	else if( std::fabs(vNormal[2]) > 0.0f ){
		if( (pt[1] >= vCenter[1] - 0.5*flHeight) &&
			(pt[1] <= vCenter[1] + 0.5*flHeight) &&
			(pt[0] >= vCenter[0] - 0.5*flWidth) &&
			(pt[0] <= vCenter[0] + 0.5*flWidth)
		  )
		  return true;
	}

	return false;
}

// Define, whether the velocity vector of the ball intersects the current wall.
// If yes, then say that the collision spot should be drawn in the intersection point on the wall.
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
	}// End if denom!=0.

	return result;
}

/*________________________________*/
// AbsorbingWall implementation.
AbsorbingWall::AbsorbingWall(std::size_t idExt, vector_3d center, float w, float h, vector_3d n):
Wall(idExt, center, w, h, n)
{
}

bool AbsorbingWall::collide(Shape *s)
{
	bool collided = Wall::collide(s);
	if(collided){
		bool bReset = true;
		s->setVelocity(vNormal, 0.0f);			
		s->move(vector_3d(0.0f, 0.0f, 0.0f), bReset);
	}

	return collided;
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
	float shine = 1.;
	Material m(ambient, diffuse, specular, shine, alpha);
	m.setValues();

	glTranslatef(vSpot[0], vSpot[1], vSpot[2]);
	glutSolidSphere (0.012*flHeight, slices, stacks);
	glPopMatrix();
}