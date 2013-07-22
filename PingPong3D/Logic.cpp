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
	iRound = 1;
	iRoundMax = 9;
	iUserScore = 0;
	iCompScore = 0;
	iMaxScore = 5;
	bNewRound = true;
}


Logic::~Logic(void)
{
}

/*______________*/
// RoundParameters implementation.
RoundParameters::RoundParameters(float w, float h, float bv, float bdv, float pv, float pr):
	flBoxWidth(w), flBoxHeight(h), flBallVelocity(bv), flBallDeltaVel(bdv),
	flComputerPaddleVel(pv), flPaddleRadius(pr)
{
}

RoundParameters::~RoundParameters()
{
}