// GameApp.h - GameApp class declaration.
// (c) Nikolay Prodanov, Juelich, summer 2013.

#pragma once			// !Later replace by macro for portability between OS.
#include <string>
#include "SdlScreen.h"

class GameApp
{
public:
	GameApp(void);
	~GameApp(void);
	void manageGame();	

	// Private methods.
private:
	int initLibraries();	// Initialize: SDL, OGL, sound, timers, game params, rand numbers.
	int loadData();		// Load sounds, textures.
	
	void setupTimers();
	void updateTimers();
	void setupNewGame();	// Init game-specific variables/objects.
	void shutDown();		// Clean up resources.

	std::vector<FMOD::Sound*> sounds;
	FMOD::System *system;
	int setupSound();		// Init fmod library.



	// SDL-specific.
	int setupSDL();

	// OpenGl-specific.
	void setupRenderingContext();	// OGL initial state: depth, lights, materials, etc.
	void setupMatrices();	// Init transforms: modelview, projection and other matrices.
	void initResize();		// Avoid code dupl. in setupMatrices() and handleResize().
	void swapBuffers();		// Swap double buffers.

	// Calculate the angle used in the perspective, we use size as a unit of length.
	float calculateAngle(float size, double zAxisDistance)
	{
		double radtheta, degtheta;
		radtheta = 2.0 * std::atan2 (size/2.0, zAxisDistance);
		degtheta = (180.0 * radtheta) / atan(1.);
		return (degtheta);
	}

	// Data load.
	std::tr1::shared_ptr<TEXTURE> loadTexture(std::string fileName);// Factory function

	//void draw2DTextureEx(float _x, float _y, float _z, float _alpha,  TEXTURE* _tex);

	// Private members.
private:
	float flScreenWidth;			// Screen width.
	float flScreenHeight;			// Screen height.
	float flLengthUnit;
	float flZaxisDistance;

	// Game logic related.
	Logic logic;
	std::tr1::shared_ptr<OptionsScreen> optionsScreen;
	std::tr1::shared_ptr<PlayScreen> playScreen;

	// Timers.
	float deltaTime;
	Uint32 lastMillisec;

	SDL_Event sdlEvent;

	// Data.
	std::string strGameName;
	SDL_Surface* surface;
	std::vector<TTF_Font* > fonts;
	// Textures.
	std::vector<std::tr1::shared_ptr<TEXTURE> > textures;
};