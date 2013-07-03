
#pragma once

// Libraries/APIs.
#ifdef WIN32			// Additional include on Windows.
#include <Windows.h>
#endif

// Libraries.
#include <SDL.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#include <glut.h>

class Shape
{
public:
	Shape(void);
	virtual ~Shape(void);
	virtual void draw() = 0;	// Pure virtual function, all childs have to implement it.
	virtual void move(float deltaTime);		// Virtual function, default: do nothing.
};

