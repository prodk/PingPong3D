// Logic.h - declaration of the Logic and RoundParameters classes.
// (c) Nikolay Prodanov, summer 2013, Juelich, Germany.
#ifndef LOGIC_H
#define LOGIC_H

#include "Observer.h"
#include "Material.h"

class Logic : public Subject	// Logic is a subject in the Observer pattern.
{
public:
	Logic(bool startscreen, bool options, bool howto, bool play, 
		bool run, bool pause, 
		bool over, bool bsound, bool asound, float scrw, float scrh);
	~Logic(void);

	// Public members.
public:
	bool bShowStartScreen;		// True if show options screen; false if play game screeen.
	bool bShowOptionsScreen;
	bool bShowHowtoScreen;
	bool bShowPlayScreen;
	bool bAppRunning;			// True if the game is going on.
	bool bGamePaused;
	bool bGameOver;				// True if the user has lost.
	bool bBackgroundSound;
	bool bActionsSound;			// True if play actions sounds.
	bool bTrain;				// True if we want to see how comp is playing.
	bool bNewRound;				// True if a new round has to be started.
	bool bNewOptionsScreen;		// When we switch to the options screen, create new buttons.
	bool bRoundFinished;		// To show the message when someone looses.

	std::size_t iRound;			// Current round.
	int iUserScore;
	int iCompScore;

	float flScreenWidth;
	float flScreenHeight;
	bool bRotated;				// True if the view has been rotated at the beginning of a round.

	const std::size_t iRoundMax;// Maximum number of rounds, it is == 9.
	const int iMaxScore;	
};

class RoundParameters
{
public:
	RoundParameters(float w, float h, float bv, float bdv, float pv, float pr);
	~RoundParameters();

	float flBoxWidth;
	float flBoxHeight;
	float flBallVelocity;
	float flBallDeltaVel;			// Scale the velocity by this value after collisions with a paddle.
	float flComputerPaddleVel;
	float flPaddleRadius;
};

typedef std::vector<std::tr1::shared_ptr<RoundParameters> > RoundParamsVector;

#endif // LOGIC_H