// Logic.cpp - implementation of the Logic class.
// (c) Nikolay Prodanov, summer 2013, Juelich, Germany.

#include "Logic.h"

Logic::Logic(bool startscreen , bool options, bool howto, bool play, 
	bool run, bool pause, bool over, bool bsound, bool asound, float scrw, float scrh) :
	bShowStartScreen(startscreen), bShowOptionsScreen(options), 
	bShowHowtoScreen(howto), bShowPlayScreen(play),
	bAppRunning(run), bGamePaused(pause), bGameOver(over),
	bBackgroundSound(bsound), bActionsSound(asound),
	flScreenWidth(scrw), flScreenHeight(scrh), 
	iRoundMax(9), iMaxScore(5)		// Constants.
{
	bTrain = false;
	iRound = 1;
	iUserScore = 0;
	iCompScore = 0;
	bNewRound = true;
	bNewOptionsScreen = false;
	bRotated = false;
	bRoundFinished = false;			// To show the message when someone looses.
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