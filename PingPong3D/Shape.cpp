// Shape.cpp - abstract Shape class implementation.
// (c) Nikolay Prodanov, Juelich, summer 2013.

#include "Shape.h"

// Global non-member function.
FMOD_RESULT playSound(FMOD::System *system, FMOD::Sound *sound, FMOD::Channel *channel)
{
	FMOD_RESULT result;	
	//if( (sound != NULL) && (system != NULL) )
	result = system->playSound(sound, 0, false, &channel);

	return result;
}

// Use default constructor for the material.
Shape::Shape(std::size_t idExt, vector_3d c) : id(idExt), vCenter(c), material()
{
	bPlaySound = true;
}

Shape::Shape(std::size_t idExt, vector_3d c, 
		vector_3d ambient, vector_3d diffuse, vector_3d specular, float shine, float alpha) :
id(idExt), vCenter(c), material(ambient, diffuse, specular, shine, alpha)
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

vector_3d & Shape::getCenter()
{
	return vCenter;
}

// Virtual functions - default implementation.
void Shape::move(float deltaTime, vector_3d dr, bool bReset)
{
}

bool Shape::collide(Shape * s)
{
	return false;
}

void Shape::setVelocity(vector_3d n)
{
}

vector_3d Shape::getVelocity() const
{
	// It's not correct to return a reference to the automatic local variable.
	// But I suppose that the default implementation of this function
	// is never used. I don't want to make this method pure virtual, because
	// not all the shapes have velocity.
	return vector_3d(0.0, 0.0, 0.0);
}

FMOD_RESULT Shape::setSound(FMOD::System *sys, FMOD::Sound *snd)
{
	if(sys == NULL) return FMOD_ERR_FILE_BAD;
	if(snd == NULL) return FMOD_ERR_FILE_BAD;
	system = sys;
	sound = snd;

	return FMOD_OK;
}

void Shape::playSound()
{
	::playSound(system, sound, 0);
}