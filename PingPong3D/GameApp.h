// GameApp.h - GameApp class declaration.
// (c) Nikolay Prodanov, Juelich, summer 2013.
#ifndef GAMEAPP_H
#define GAMEAPP_H

#include <string>
#include "SdlScreen.h"

// The main class which contains all the game ingredients.
class GameApp : public Observer
{
public:
	GameApp(void);
	~GameApp(void);
	void manageGame();				// Check what screen should be used, and use it.
	void notify(Subject* s);		// Callback, invoked by the Subject in the Observer pattern.

	// Private methods.
private:
	int initLibraries();			// Initialize: SDL, OGL, sound, game params, rand numbers.
	void loadData();					// Load sounds, textures.
	void setupRoundParams();		// Initialize the parameters of all the rounds.
	void createScreens();
	void registerObservers();		// What objects should be tracked by the Subject.
	int setupSDL();
	int setupSound();				// Init fmod library.

	// Data load.
	std::tr1::shared_ptr<TEXTURE> loadTexture(std::string fileName);// Factory function
	void loadSounds();
	void loadFonts();
	
	void shutDown();				// Clean up resources.
	
	// Sounds.
	void playBackgroundSound();
	void checkErr(FMOD_RESULT r, std::string &msg);

	// OpenGl-specific.
	void setupRenderingContext();	// OGL initial state: depth, lights, materials, etc.
	void setupMatrices();			// Init transforms: modelview, projection and other matrices.
	//void initResize();				// Avoid code dupl. in setupMatrices() and handleResize().
	void swapBuffers();				// Swap double buffers.	

	// Inline: calculate the angle used in the perspective, we use size as a unit of length.
	float calculateAngle(float size, double zAxisDistance)
	{
		double radtheta, degtheta;
		radtheta = 2.0 * std::atan2 (size/2.0, zAxisDistance);
		degtheta = (180.0 * radtheta) / atan(1.);
		return (degtheta);
	}

	//void draw2DTextureEx(float _x, float _y, float _z, float _alpha,  TEXTURE* _tex);

	// Private members.
private:
	enum {START_SCREEN = 1, OPTIONS_SCREEN, HOWTO_SCREEN, PLAY_SCREEN};
	float flScreenWidth;			// Screen width.
	float flScreenHeight;			// Screen height.
	//float flLengthUnit;				// Scaling factor in the perspective.
	//float flZaxisDistance;			// Distance from the camera to the scene.

	// Game logic related.
	Logic logic;					// Contains all the flats controlling the flow of the app.
	bool bBackgroundSound;			// Whether to play background sound.

	// Screens.
	std::tr1::shared_ptr<StartScreen> startScreen;
	std::tr1::shared_ptr<OptionsScreen> optionsScreen;
	std::tr1::shared_ptr<HowtoScreen> howtoScreen;
	std::tr1::shared_ptr<PlayScreen> playScreen;
	RoundParamsVector roundParams;
	SDL_Event sdlEvent;

	// FMOD sound.
	std::vector<FMOD::Sound*> sounds;
	FMOD::System *system;
	FMOD::Channel *channelOptions;
	std::vector<FMOD::Channel *> channelRound;
	

	// Data.	
	std::string strGameName;
	SDL_Surface* surface;			// SDL surface on which everything is rendered.
	std::vector<TTF_Font* > fonts;	
	std::vector<std::tr1::shared_ptr<TEXTURE> > textures;

	const std::size_t iNumOfSounds;
	const std::size_t iNumOfFonts;
	const std::size_t iNumOfTextures;
};
#endif // GAMEAPP_H