#pragma once

#include "Shape.h"

class Wall : public Shape
{
public:
	Wall(std::size_t idExt, float w, float h, vector_3d center, vector_3d n);
	~Wall(void);

	// Overriden virtual funcions.
	std::size_t getId() const;
	vector_3d getCenter() const;
	float getSize() const;			// Return width of the wall.
	void draw();
	void move(float deltaTime);
	void collide(Shape *);

private:
	std::size_t id;
	float width;
	float height;
	vector_3d vCenter;
	vector_3d vNormal;
	std::vector<vector_3d> v;			// Vertices
	const int numOfVerteces;

	void setupVertices();
};

