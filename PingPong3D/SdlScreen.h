#pragma once

#include "GuiObject.h"

// Abstract base class for screens.
class SdlScreen
{
public:
	SdlScreen(float w, float h);
	virtual ~SdlScreen(void) = 0;

protected:
	//SDL_Surface* surface;
	float flWidth;			// Screen width.
	float flHeight;			// Screen height.
};

class OptionsScreen : public SdlScreen
{
public:
	OptionsScreen(float w, float h, SDL_Surface* s, TEXTURE* t);
	~OptionsScreen(void);
	void show();
private:
	SDL_Surface* surfaceOnWhichWeRender;
	TEXTURE * textures;
	//int numOfTextures;
};