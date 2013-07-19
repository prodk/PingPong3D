// Logic.cpp - implementation of the Logic class.
// (c) Nikolay Prodanov, Juelich, summer 2013.
#include "Logic.h"


Logic::Logic(bool opts, bool run, bool pause, bool over, bool bsound, bool asound) :
	bShowOptions(opts), bAppRunning(run), bGamePaused(pause), bGameOver(over),
		bBackgroundSound(bsound), bActionsSound(asound)
{
}


Logic::~Logic(void)
{
}
