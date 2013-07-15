#pragma once

#include "Shape.h"

/*________________________________*/
// GuiObject class declaration.
class GuiObject
{
public:
	GuiObject(std::size_t idExt);
	virtual ~GuiObject(void);
	virtual void draw() = 0;

	std::size_t getId();

protected:
	std::size_t id;
};

/*________________________________*/
//Button class declaration.
class Button : public GuiObject
{
public:
	Button(float x, float y, float w, float h, std::size_t idExt);
	
	// Overridden virtual methods.
	void draw();
private:
	void addButtons();
private:
	float x;
	float y;
	float w;
	float h;
};