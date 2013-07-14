#pragma once

#include "Shape.h"

class GuiObject
{
public:
	GuiObject(int x, int y, int w, int h);
	virtual ~GuiObject(void);

private:
	SDL_Rect rcBox;
};

