// Shape.cpp - abstract Shape class implementation.
// (c) Nikolay Prodanov, Juelich, summer 2013.

#include "Shape.h"

Shape::Shape(std::size_t idExt, vector_3d c):id(idExt), vCenter(c)
{
}

Shape::~Shape(void)
{
}

// Non-virtual functions.
std::size_t Shape::getId() const
{
	return id;
}

vector_3d Shape::getCenter() const
{
	return vCenter;
}

// Virtual functions - default implementation.
void Shape::move(float deltaTime, vector_3d dr, bool bReset)
{
}

void Shape::collide(Shape * s)
{
}

void Shape::setVelocity(vector_3d n)
{
}