// Wall.h - declaration of the Wall and AbsorbingWall classes.
// (c) Nikolay Prodanov, Juelich, summer 2013.
#ifndef WALL_H
#define WALL_H

#include "Shape.h"

// Wall is a Shape.
class Wall : public Shape
{
public:
	Wall(std::size_t idExt, vector_3d center, float w, float h, vector_3d n);
	Wall(std::size_t idExt, vector_3d center, float w, float h, vector_3d n,
		vector_3d ambient, vector_3d diffuse, vector_3d specular, float shine, float alpha);
	~Wall(void);

	// Overriden virtual funcions.
	float getSize() const;				// Return width of the wall.
	void draw();
	void move(float deltaTime, vector_3d dr, bool bReset);
	bool collide(Shape *);

	//virtual void notify(Subject* s) {bPlaySound = ((Logic*) s)->bActionsSound;};

protected:
	float width;
	float height;
	vector_3d vNormal;					// Normal to the plane of the Wall.
	std::vector<vector_3d> v;			// Vertices.
	const int numOfVerteces;

	int polygonMode;

	bool bDrawSpot;
	vector_3d vSpot;
	bool ptInWall(const vector_3d &pt) const;
	virtual void drawSpot();
	bool spotOnWall(Shape *s);

	void setupVertices();

	// Change this function later:
// Save vertices to an array and maybe use a display list.
// Add more points to the array.
// Maybe make it nonmember because it is also used in Wall.
void drawCircle(float radius)
{
	const float DEG2RAD = 3.14159/180;

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glBegin(GL_POLYGON); 
	for (float i=0.; i < 360.; i+=0.5)
	{
		float degInRad = i*DEG2RAD;
		glNormal3f(0.0, 0.0, 1.0);
		glVertex3f(cos(degInRad)*radius, sin(degInRad)*radius, 0.0);
	}

	glEnd();
}
};

// Absorbing wall is a Wall that doesn't reflect.
class AbsorbingWall : public Wall
{
public:
	AbsorbingWall(std::size_t idExt, vector_3d center, float w, float h, vector_3d n);
	~AbsorbingWall(void);
	bool collide(Shape *s);
	void drawSpot();

	//void notify(Subject* s) {bPlaySound = ((Logic*) s)->bActionsSound;};
};

#endif // WALL_H