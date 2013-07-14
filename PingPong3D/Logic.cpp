#include "Logic.h"


Logic::Logic(bool opts, bool run, bool pause, bool over) :
	bShowOptions(opts), bAppRunning(run), bGamePaused(pause), bGameOver(over)
{
}


Logic::~Logic(void)
{
}
