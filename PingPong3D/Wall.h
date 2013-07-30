// Wall.h - declaration of the Wall and AbsorbingWall classes.
// (c) Nikolay Prodanov, summer 2013, Juelich, Germany.
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
	bool collide(Shape *);

protected:
	void setupVertices();

protected:
	float flWidth;
	float flHeight;
	vector_3d vNormal;					// Normal to the plane of the Wall, points outside the box.
	std::vector<vector_3d> v;			// Vertices.
	const int numOfVerteces;

	int polygonMode;					// An OGL mode for drawing polygons.
	bool bDrawSpot;						// Whether to display green/red collision spot.
	vector_3d vSpot;					// Collision spot location on the wall.
	bool ptInWall(const vector_3d &pt) const;
	virtual void drawSpot();
	bool spotOnWall(Shape *s);

	const int slices;					// For drawing spherical spots using glut.
	const int stacks;
};

// Absorbing wall is a Wall that doesn't reflect ball and shows a red spot (not a green one).
class AbsorbingWall : public Wall
{
public:
	AbsorbingWall(std::size_t idExt, vector_3d center, float w, float h, vector_3d n);
	~AbsorbingWall(void);
	bool collide(Shape *s);
	void drawSpot();
};

#endif // WALL_H