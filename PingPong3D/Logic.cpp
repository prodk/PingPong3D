// Logic.cpp - implementation of the Logic class.
// (c) Nikolay Prodanov, Juelich, summer 2013.
#include "Logic.h"


Logic::Logic(bool startscreen , bool options, bool howto, bool play, 
	bool run, bool pause, bool over, bool bsound, bool asound) :
	bShowStartScreen(startscreen), bShowOptionsScreen(options), 
	bShowHowtoScreen(howto), bShowPlayScreen(play),
	bAppRunning(run), bGamePaused(pause), bGameOver(over),
	bBackgroundSound(bsound), bActionsSound(asound)
{
	bTrain = false;
	flBackroundVolume = 1.;
	flActionVolume = 1.;
}


Logic::~Logic(void)
{
}