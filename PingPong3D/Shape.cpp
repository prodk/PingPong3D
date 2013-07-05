// Shape.cpp - abstract Shape class implementation.
// (c) Nikolay Prodanov, Juelich, summer 2013.

#include "Shape.h"

Shape::Shape(void)
{
}

Shape::~Shape(void)
{
}

void Shape::move(float deltaTime, vector_3d dr, bool bReset)
{
}

void Shape::collide(Shape * s)
{
}

void Shape::setVelocity(vector_3d n)
{
}