// Paddle.h - declaration of the Paddle class.
// (c) Nikolay Prodanov, summer 2013, Juelich, Germany.
#ifndef PADDLE_H
#define PADDLE_H

#include "Shape.h"

class Paddle : public Shape
{
public:
	Paddle(std::size_t idExt, vector_3d shiftCenter, vector_3d n, 
				float r, float h, float a, float top, float front,
				vector_3d ambient, vector_3d diffuse, vector_3d specular, 
				float shine, float alpha, float dv);
	virtual ~Paddle(void);

	// Overridden virtual funcions.
	float getSize() const;					// Return radius of the cylinder.
	void draw();
	virtual void move(vector_3d dr, bool bReset);
	virtual bool collide(Shape *);			// Collision detection.

// Private methods.
protected:
	bool ptInPaddle(const vector_3d &pt) const;
	void drawCircle();
	void generateCircle();

protected:
	vector_3d vNormal;
	float flRadius;
	float flHeight;
	const float angle;
	float maxCoordTop;
	float maxCoordFront;
	vector_3d vVelocity;
	float flBallDeltaVel;

	// Rendering-related.
	vector_3d vSpot;
	GLUquadricObj *quadratic;
	float flAlpha;				// Handle related.
	float flBeta;				// Handle related.
	const int slices;
	const int stacks;
	std::vector<vector_3d> circleVerteces;
	int	numOfCircleVertices;	
};	// End Paddle.

class ComputerPaddle : public Paddle
{
public:
	ComputerPaddle(std::size_t idExt, vector_3d shiftCenter, vector_3d n, 
				float r, float h, float a, float top, float front,
				vector_3d ambient, vector_3d diffuse, vector_3d specular, 
				float shine, float alpha, float dv, float v);
	~ComputerPaddle(void);

	void move(vector_3d dr, bool bReset);
	bool collide(Shape * s);	

private:
	vector_3d vCollisionSpot;
	float flVelocity;
};

#endif // PADDLE_H