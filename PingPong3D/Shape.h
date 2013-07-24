// Shape.h - abstract Shape class declaration.
// (c) Nikolay Prodanov, Juelich, summer 2013.
#ifndef SHAPE_H
#define SHAPE_H

#include <fmod.hpp>
#include <fmod_common.h>
#include <fmod_errors.h>
#include "Observer.h"
#include "Material.h"
#include "Logic.h"

// Use a global non-member function to play sound.
FMOD_RESULT playSound(FMOD::System *system, FMOD::Sound *sound, FMOD::Channel *channel);

// Inherit from the Observer abstract class to use Shape in the Observer pattern.
class Shape
{
public:
	Shape(std::size_t idExt, vector_3d c);
	Shape(std::size_t idExt, vector_3d c, 
		vector_3d ambient, vector_3d diffuse, vector_3d specular, float shine, float alpha);
	virtual ~Shape(void);

	// Methods that are not overridden.
	std::size_t getId() const;
	vector_3d & getCenter();			// Return the coordinates of the Shape's center.
	FMOD_RESULT setSound(FMOD::System *sys, FMOD::Sound *snd);
	void playSound();

	// Virtual methods.
	virtual void move(float deltaTime, vector_3d dr, bool bReset);	//Default: do nothing.
	virtual bool collide(Shape * s);		// Detect collision with another Shape.
	virtual void setVelocity(vector_3d n);	// Change the direction of the velocity.
	virtual vector_3d getVelocity() const;

	// Pure virtual methods, all children have to implement them.
	virtual void draw() = 0;				// Draw itself.
	virtual float getSize() const = 0;		// Returns characteristic size.

protected:
	std::size_t id;
	vector_3d vCenter;
	Material material;

	bool bPlaySound;
	FMOD::Sound* sound;
	FMOD::System *system;
	FMOD::Channel *channel;	// Channel is used for manipulation of the sound.
};

#endif // SHAPE_H