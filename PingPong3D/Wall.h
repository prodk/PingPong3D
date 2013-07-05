// Wall.h - declaration of the Wall and AbsorbingWall classes.
// (c) Nikolay Prodanov, Juelich, summer 2013.
#pragma once

#include "Shape.h"

// Wall is a Shape.
class Wall : public Shape
{
public:
	Wall(std::size_t idExt, float w, float h, vector_3d center, vector_3d n);
	~Wall(void);

	// Overriden virtual funcions.
	std::size_t getId() const;
	vector_3d getCenter() const;
	float getSize() const;				// Return width of the wall.
	void draw();
	void move(float deltaTime, vector_3d dr, bool bReset);
	void collide(Shape *);

protected:
	std::size_t id;
	float width;
	float height;
	vector_3d vCenter;
	vector_3d vNormal;					// Normal to the plane of the Wall.
	std::vector<vector_3d> v;			// Vertices.
	const int numOfVerteces;

	void setupVertices();
};

// Absorbing wall is a Wall that doesn't reflect.
class AbsorbingWall : public Wall
{
public:
	AbsorbingWall(std::size_t idExt, float w, float h, vector_3d center, vector_3d n);
	~AbsorbingWall(void);
	void collide(Shape *);
};