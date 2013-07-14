#pragma once	// Change this to #define later!

class Logic
{
	// Public methods.
public:
	Logic(bool opts, bool run, bool pause, bool over);
	~Logic(void);
	// Public members.
public:
	bool bShowOptions;		// True if show options screen, false if play game screeen.
	bool bAppRunning;		// True if the game is going on.
	bool bGamePaused;
	bool bGameOver;			// True if the player has lost.
};
