// Shape.h - abstract Shape class declaration.
// (c) Nikolay Prodanov, Juelich, summer 2013.
#pragma once

#include <fmod.hpp>
#include <fmod_common.h>
#include <fmod_errors.h>
#include "Material.h"

//std::vector<FMOD::Sound*> sounds;
//FMOD::System *system;
// Use a global non-member function to play sound.
FMOD_RESULT playSound(FMOD::System *system, FMOD::Sound *sound);

// Maybe move id, vCenter, vNormal to this class, as protected members.
class Shape
{
public:
	Shape(std::size_t idExt, vector_3d c);
	Shape(std::size_t idExt, vector_3d c, 
		vector_3d ambient, vector_3d diffuse, vector_3d specular, float shine, float alpha);
	virtual ~Shape(void);

	// Methods that are not supposed to be overridden.
	std::size_t getId() const;
	vector_3d getCenter() const;// Return the coordinates of the Shape's center.

	void setSound(FMOD::System *sys, FMOD::Sound *snd);

	// Virtual methods.
	virtual void move(float deltaTime, vector_3d dr, bool bReset);		// Virtual function, default: do nothing.
	virtual void collide(Shape * s);		// Detect collision with another Shape.
	virtual void setVelocity(vector_3d n);	// Change the direction of the velocity.
	virtual vector_3d getVelocity() const;

	// Pure virtual methods, all children have to implement them.
	virtual void draw() = 0;				// Draw itself.
	virtual float getSize() const = 0;		// Returns characteristic size.

protected:
	std::size_t id;
	vector_3d vCenter;
	Material material;

	// Reconsider sound:
	FMOD::Sound* sound;
	FMOD::System *system;
};

