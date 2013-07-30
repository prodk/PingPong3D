// Shape.h - abstract Shape class declaration.
// (c) Nikolay Prodanov, summer 2013, Juelich, Germany.
#ifndef SHAPE_H
#define SHAPE_H

#include <fmod.hpp>
#include <fmod_common.h>
#include <fmod_errors.h>
#include "Observer.h"
#include "Material.h"
#include "Logic.h"

// Use a global, non-member functions to play sound and generate random numbers.
FMOD_RESULT playSound(FMOD::System *system, FMOD::Sound *sound, FMOD::Channel *channel);
float generateRand(float start, float end);
float generateRandomSignInterval(float start, float end);

/*________________________*/
class Shape
{
public:
	Shape(std::size_t idExt, vector_3d c);
	Shape(std::size_t idExt, vector_3d c, 
		vector_3d ambient, vector_3d diffuse, vector_3d specular, float shine, float alpha);
	virtual ~Shape(void);

	// Methods that are not to be overridden.
	std::size_t getId() const;
	vector_3d & getCenter();				// Return the coordinates of the Shape's center.
	FMOD_RESULT setSound(FMOD::System *sys, FMOD::Sound *snd);
	void playSound();

	// Virtual methods.
	virtual void move(vector_3d dr, bool bReset);	//Default: do nothing.
	virtual bool collide(Shape * s);				// Detect collision with another Shape.
	virtual void setVelocity(vector_3d n, float factor);// Change the direction of the velocity.
	virtual vector_3d getVelocity() const;

	// Pure virtual methods, all children have to implement them.
	virtual void draw() = 0;						// Draw itself.
	virtual float getSize() const = 0;				// Returns a characteristic size.

protected:
	std::size_t id;
	vector_3d vCenter;
	Material material;

	bool bPlaySound;
	FMOD::Sound* sound;
	FMOD::System *system;
	FMOD::Channel *channel;
};

#endif // SHAPE_H