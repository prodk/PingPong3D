// Shape.cpp - abstract Shape class implementation.
// (c) Nikolay Prodanov, Juelich, summer 2013.

#include "Shape.h"

// Global non-member function.
FMOD_RESULT playSound(FMOD::System *system, FMOD::Sound *sound, FMOD::Channel *channel)
{
	FMOD_RESULT result;	
	if( (sound != NULL) && (system != NULL) )
		result = system->playSound(sound, 0, false, &channel);
	//ERRCHECK(result);

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

vector_3d Shape::getVelocity() const
{
	return vector_3d(0.0, 0.0, 0.0);
}

FMOD_RESULT Shape::setSound(FMOD::System *sys, FMOD::Sound *snd)
{
	if(sys == NULL) return FMOD_ERR_FILE_BAD;
	if(snd == NULL) return FMOD_ERR_FILE_BAD;
	system = sys;
	sound = snd;

	// Start paused sound.
	//FMOD_RESULT result = system->playSound(sound, 0, true, &channel);

	return FMOD_OK;
}

void Shape::notify(Subject* s) 
{
	bPlaySound = ((Logic*) s)->bActionsSound;
}

/*
void Shape::playSound()
{
	if(bPlaySound)
		channel->setPaused(false);
}
*/