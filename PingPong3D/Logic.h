// Logic.h - declaration of the Logic class.
// (c) Nikolay Prodanov, Juelich, summer 2013.
#ifndef LOGIC_H
#define LOGIC_H

#include "Observer.h"

class Logic : public Subject
{
	// Public methods.
public:
	Logic(bool opts, bool run, bool pause, bool over, bool bsound, bool asound);
	~Logic(void);
	// Public members.
public:
	bool bShowOptions;		// True if show options screen, false if play game screeen.
	bool bAppRunning;		// True if the game is going on.
	bool bGamePaused;
	bool bGameOver;			// True if the player has lost.
	bool bBackgroundSound;
	bool bActionsSound;
};

#endif // LOGIC_H