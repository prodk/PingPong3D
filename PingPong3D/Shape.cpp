// Shape.cpp - abstract Shape class implementation.
// (c) Nikolay Prodanov, summer 2013, Juelich, Germany.

#include "Shape.h"

// Global non-member function.
FMOD_RESULT playSound(FMOD::System *system, FMOD::Sound *sound, FMOD::Channel *channel)
{
	FMOD_RESULT result;
	// Check that sound has been loaded.
	if( (sound != NULL) && (system != NULL) )
		result = system->playSound(sound, 0, false, &channel);

	return result;
}

float generateRand(float start, float end)
{
	float range = (end - start);
	float rnd = start + float((range * rand()) / (RAND_MAX + 1.0));
	return rnd;
}

// Returns a rand number from the specified interval of absolute values with a random sign.
float generateRandomSignInterval(float start, float end)
{
	float sign = 1.;
	if( generateRand(-1., 1.) < 0)
		sign = -1.;

	return generateRand(sign*start, sign*end);
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
void Shape::move(vector_3d dr, bool bReset)
{
}

bool Shape::collide(Shape * s)
{
	return false;
}

void Shape::setVelocity(vector_3d n, float factor)
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

	return FMOD_OK;
}

void Shape::playSound()
{
	::playSound(system, sound, 0);
}