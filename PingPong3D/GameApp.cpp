// GameApp.cpp - implementation of the GameApp class.
// (c) Nikolay Prodanov, Juelich, summer 2013.

#include "PingPong3D.h"

GameApp::GameApp(void) :
flScreenWidth(1024), flScreenHeight(640), strGameName("Ping Pong 3D"), 
	//flZaxisDistance(1.f), flLengthUnit(0.25f), 
	bBackgroundSound(true),
	//Logic(start, options, howto, play, run, pause, over, bsound, asound);
	logic(true, false, false, false, true, false, false,  bBackgroundSound, true, 
	flScreenWidth, flScreenHeight),
	iNumOfSounds(7), iNumOfFonts(1), iNumOfTextures(7)
{
	// Exceptions and bad values are caught/checked inside the functions.
	initLibraries();
	loadData();
	setupRoundParams();	// Set the parameters of all the rounds.
	createScreens();
	registerObservers();// Register observers for the Observer pattern to manage the sound.
}

GameApp::~GameApp(void)
{
	shutDown();			// Shapes are handled via smart pointers, so no delete[] here.	
}

int GameApp::initLibraries()
{	
	if( setupSDL() < 0 ){		// Initialize SDL.
		std::cerr << "Failed to initialize SDL" << std::endl; 
		exit(1);
	}    
    
    setupRenderingContext();	// OpenGL.
    setupMatrices();			// OpenGL.    
    setupSound();				// FMOD sound.
    
    std::srand ((unsigned int)time(NULL));	// Seed random numbers.

	return 0;
}

int GameApp::setupSDL()
{
	if(SDL_Init(SDL_INIT_VIDEO) < 0)
		return -1;

	// Get some video information.
    const SDL_VideoInfo* info = SDL_GetVideoInfo();
	if(!info) {
		std::cerr << "Failed to get videInfo structue." << std::endl;
		exit(1);
	}
	int bpp = info->vfmt->BitsPerPixel;

	// Set window's icon.
	SDL_Surface* s = IMG_Load("../data/textures/pp3d.png");
	if(surface != NULL){
		SDL_WM_SetIcon(s, NULL);
		SDL_FreeSurface(s);
	}

	int flags = SDL_OPENGL | SDL_HWSURFACE | SDL_RESIZABLE | SDL_DOUBLEBUF;

    surface = SDL_SetVideoMode((int)flScreenWidth, (int)flScreenHeight, bpp, flags);

	if(surface == NULL){
		std::cerr << "SDL set video mode failed: %s\n" << SDL_GetError() << std::endl;
		exit(1);
	}
	SDL_WM_SetCaption(strGameName.c_str(), strGameName.c_str());

	return 0;
}

void GameApp::loadSounds()
{
	FMOD_RESULT result;
	sounds.resize(iNumOfSounds);
	
	result = system->createStream("../data/audio/startscreen.mp3", FMOD_LOOP_NORMAL, 0, &sounds[0]);
	checkErr(result, std::string("../data/audio/startscreen.mp3"));

	result = system->createStream("../data/audio/round1.mp3", FMOD_LOOP_NORMAL, 0, &sounds[1]);
	checkErr(result, std::string("../data/audio/round1.mp3"));

	result = system->createStream("../data/audio/round2.mp3", FMOD_LOOP_NORMAL, 0, &sounds[2]);
	checkErr(result, std::string("../data/audio/round2.mp3"));

	result = system->createStream("../data/audio/round3.mp3", FMOD_LOOP_NORMAL, 0, &sounds[3]);
	checkErr(result, std::string("../data/audio/round3.mp3"));

	result = system->createStream("../data/audio/normalwall.wav", FMOD_DEFAULT, 0, &sounds[4]);
	checkErr(result, std::string("../data/audio/normalwall.wav"));

	result = system->createStream("../data/audio/absorbwall.wav", FMOD_DEFAULT, 0, &sounds[5]);
	checkErr(result, std::string("../data/audio/absorbwall.wav"));

	result = system->createStream("../data/audio/paddle.mp3", FMOD_DEFAULT, 0, &sounds[6]);
	checkErr(result, std::string("../data/audio/paddle.mp3"));

	// Start paused sounds.
	channelRound.resize(3);
	// Options.
	if( (sounds[0] != NULL) && (system != NULL) )
		result = system->playSound(sounds[0], 0, true, &channelOptions);
	// Rounds sound.
	if( (sounds[1] != NULL) && (system != NULL) )
		result = system->playSound(sounds[1], 0, true, &channelRound[0]);
	if( (sounds[2] != NULL) && (system != NULL) )
		result = system->playSound(sounds[2], 0, true, &channelRound[1]);
	if( (sounds[3] != NULL) && (system != NULL) )
		result = system->playSound(sounds[3], 0, true, &channelRound[2]);
}

void GameApp::loadFonts()
{
	// Load fonts.
	fonts.resize(iNumOfFonts);
	if( TTF_Init() == -1 ) { 
		std::cerr << "Failed to initialize TTF library" << std::endl;
		exit(1); 
	}
	fonts[0] = TTF_OpenFont( "../data/fonts/Line-01.ttf", 28 );
	if( fonts[0] == NULL ){ 
		std::cerr << "Failed to Load font:" << "Line-01.ttf" << std::endl;
		exit(1); 
	}
}

void GameApp::loadData()
{
	// Load textures.
	textures.resize(iNumOfTextures);
	textures[0] = loadTexture("../data/textures/button.png");
	textures[START_SCREEN] = loadTexture("../data/textures/start.png");
	textures[OPTIONS_SCREEN] = loadTexture("../data/textures/options.png");
	textures[HOWTO_SCREEN] = loadTexture("../data/textures/howto.png");
	textures[PLAY_SCREEN] = loadTexture("../data/textures/round1.png");
	textures[PLAY_SCREEN+1] = loadTexture("../data/textures/round2.png");
	textures[PLAY_SCREEN+2] = loadTexture("../data/textures/round3.png");
	/*textures[PLAY_SCREEN+3] = loadTexture("../data/textures/round4.png");
	textures[PLAY_SCREEN+4] = loadTexture("../data/textures/round5.png");
	textures[PLAY_SCREEN+5] = loadTexture("../data/textures/round6.png");
	textures[PLAY_SCREEN+6] = loadTexture("../data/textures/round7.png");
	textures[PLAY_SCREEN+7] = loadTexture("../data/textures/round8.png");
	textures[PLAY_SCREEN+8] = loadTexture("../data/textures/round9.png");*/

	// Load sounds and fonts.
	loadSounds();
	loadFonts();
}

std::tr1::shared_ptr<TEXTURE> GameApp::loadTexture(std::string fileName)
{
    SDL_Surface* surface;
    GLuint textureid;
    int mode;

	surface = IMG_Load(fileName.c_str());
    
    // Could not load image.
    if (!surface) {
        std::cerr << "Could not load image: " << fileName.c_str() << std::endl;
        exit(1);
    }
    
    // Work out what format gluBuild2DMipmaps should use.
    if (surface->format->BytesPerPixel == 3){		// RGB 24bit.
        mode = GL_RGB;
    } 
    else if (surface->format->BytesPerPixel == 4) { // RGBA 32bit. 
        mode = GL_RGBA;
    } 
    else {
        std::cerr << "Could not determine pixel format of image " << fileName.c_str() << std::endl;
        SDL_FreeSurface(surface);
        exit(1);
    }

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // Create one texture name.
    glGenTextures(1, &textureid);

    // Tell OGL to use the generated texture name.
    glBindTexture(GL_TEXTURE_2D, textureid);

    // Read from the sdl surface and put it into an opengl texture.
	gluBuild2DMipmaps( GL_TEXTURE_2D, GL_RGBA, surface->w, surface->h,
                       mode, GL_UNSIGNED_BYTE, surface->pixels );
	
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, surface->w, surface->h, 0, mode, 
		//GL_UNSIGNED_BYTE, surface->pixels );

    // Allocate memory for Texture structure and fill it up.
	std::tr1::shared_ptr<TEXTURE> texture(new TEXTURE());
    if (!texture.get()) {
        std::cerr << "Could not create TEXTURE struct for image " << fileName.c_str() << std::endl;
		exit(1);
    }
    
    // Set up TEXTURE structure.
    texture->width = surface->w;
    texture->height = surface->h;
    texture->id = textureid;    
    
    SDL_FreeSurface(surface);
    
    return texture;
}

// Setup various OpenGL settings.
void GameApp::setupRenderingContext()
{
	glShadeModel( GL_SMOOTH );				// Shading model - Gouraud (smooth).

	glClearColor(0.5f, 0.5f, 0.5f, 0.0f);	// Background color.
	glShadeModel(GL_SMOOTH);				//Smooth shading.

	// Depth buffer.
	glClearDepth(1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);				// Enable z-buffer.
	glDepthFunc(GL_LEQUAL);

	// Antialiasing.
	glEnable (GL_LINE_SMOOTH);
	glHint (GL_LINE_SMOOTH_HINT, GL_DONT_CARE);
	glLineWidth (3.);

	// Blending (transparency).
	glEnable (GL_BLEND);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glAlphaFunc(GL_GREATER, (GLclampf)0.01); // Skip pixels which alpha channel is lower than 0.01.
	glEnable(GL_ALPHA_TEST);  // Enable Alpha.

	glViewport(0, 0, (GLsizei)flScreenWidth, (GLsizei)flScreenHeight); // Set viewport to window dimensions.
	
	// Enable Lighting.
	glEnable(GL_LIGHTING);
	
	// Set up light0.
	GLfloat ambientLight[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat diffuseLight[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat specularLight[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	GLfloat position[] = {  0.0, 40.0, -20.0, 1.0 };		// Adjust this position!

	glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);
	glLightfv(GL_LIGHT0, GL_POSITION, position);

	glEnable(GL_LIGHT0);		// Enable stationary light.
	glEnable(GL_LIGHT1);		// Light moving with the ball.

	// Global light settings.
	GLfloat global_ambient[] = { 0.3f, 0.3f, 0.3f, 1.0f };
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, global_ambient);

	glPointSize(20);			// Size of points.
}

void GameApp::setupMatrices()
{ 
	//initResize();
    // Reset texture view matrix stack.
    glMatrixMode(GL_TEXTURE);
    glLoadIdentity();
}

void GameApp::createScreens()
{
	try {
		startScreen = std::tr1::shared_ptr<StartScreen>(
			new StartScreen(START_SCREEN, 
			flScreenWidth, flScreenHeight, surface, textures, &fonts[0],
			system, sounds) );

		optionsScreen = std::tr1::shared_ptr<OptionsScreen>(
			new OptionsScreen(OPTIONS_SCREEN,
			flScreenWidth, flScreenHeight, surface, textures, &fonts[0],
			system, sounds, logic) );

		howtoScreen = std::tr1::shared_ptr<HowtoScreen>(
			new HowtoScreen(HOWTO_SCREEN,
			flScreenWidth, flScreenHeight, surface, textures, &fonts[0],
			system, sounds) );

		playScreen = std::tr1::shared_ptr<PlayScreen>(
			new PlayScreen(PLAY_SCREEN,
			flScreenWidth, flScreenHeight, surface, textures, &fonts[0],
			system, sounds, roundParams) );
	}// End try.
	catch(std::bad_alloc& ba) {
		std::cerr << "Failed to create one of the screens: memory error, " + std::string(ba.what());
		shutDown();
		exit(1);
	}
}

void GameApp::manageGame()
{
	playBackgroundSound();

	while(logic.bAppRunning) 
	{		
		SDL_PollEvent(&sdlEvent);					// Important: use only one PollEvent!!!

		if(logic.bShowStartScreen){
			startScreen->setScreenSize(logic.flScreenWidth, logic.flScreenHeight);
			startScreen->doDrawing(logic);	
			startScreen->doInput(logic, sdlEvent);	// Put These in a separate method.			
		}
		else if(logic.bShowOptionsScreen){
			if(logic.bNewOptionsScreen){			// Initialize buttons if required.
				optionsScreen->setScreenSize(logic.flScreenWidth, logic.flScreenHeight);
				logic.bNewOptionsScreen = false;
				optionsScreen->setupNewScreen(logic);
			}
			optionsScreen->doDrawing(logic);
			optionsScreen->doInput(logic, sdlEvent);
		}
		else if(logic.bShowHowtoScreen){
			howtoScreen->doDrawing(logic);
			howtoScreen->doInput(logic, sdlEvent);
		}
		else if(logic.bShowPlayScreen){
			playScreen->play(logic, sdlEvent);		// Play the game.	
		}
		
		swapBuffers();
	} // End while(logic.bAppRunning).
}



void GameApp::swapBuffers()
{	
	glFlush();
    SDL_GL_SwapBuffers();			// Swap buffers in the double buffer.
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear buffer for new data.
}

void GameApp::shutDown()
{
	FMOD_RESULT result;	

	// Cleanup textures.
	std::size_t i;
	for(i = 0; i < textures.size(); i++)
		glDeleteTextures(1, &(textures[i].get()->id));

	// Cleanup sounds.
	for(i = 0; i < sounds.size(); i++){
		result = sounds[i]->release();
		if (result != FMOD_OK) {
			std::cerr << "FMOD cleanup error! " << result << FMOD_ErrorString(result) << std::endl;
			exit(-1);
		}
	}// End for sounds.
	system->release();

	// Cleanup fonts.
	for(i = 0; i < fonts.size(); i++)
		TTF_CloseFont(fonts[i]);

	if(surface != NULL){
		SDL_FreeSurface(surface);
		surface = NULL;
	}
    SDL_Quit();
	// Most object pointers (e.g. screens) are cleaned up through the smart pointers.
	// That's why no delete operators here.
}

//
//void GameApp::draw2DTextureEx(float _x, float _y, float _z, float _alpha,  TEXTURE* _tex) 
//{
//    /* only use when currently rendering in ortho projection matrix */
//    //if(rendering_ortho)
//    //{
//        /* Temporarily disable lighting */
//        glDisable(GL_LIGHTING);
//        
//        //cleanObject();
//        glTranslatef(_x, _y, _z);
//        glScalef(_tex->width, _tex->height, 1.0f); /* Blow up quad to texture size */
//        glBindTexture(GL_TEXTURE_2D, _tex->id);
//        /* Do proper blending with alpha channel */
//        glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 
//        
//        glColor4f ( 1.0f, 1.0f, 1.0f, _alpha );
//        
//        glEnable(GL_TEXTURE_2D);
//        glBegin(GL_QUADS);
//            /* top left */
//            glTexCoord2f(0, 0);
//            glVertex2f(0, 0);
//            
//            /* bottom left */
//            glTexCoord2f(0, 1);
//            glVertex2f(0, 1);
//    
//            /* bottom right */
//            glTexCoord2f(1, 1);
//            glVertex2f(1, 1);
//            
//            /* top right */
//            glTexCoord2f(1, 0);
//            glVertex2f(1, 0);
//        glEnd();  
//        glDisable(GL_TEXTURE_2D);
//        
//        glEnable(GL_LIGHTING);
//    //}
//}

int GameApp::setupSound()
{
	FMOD_RESULT result;
	
	result = FMOD::System_Create(&system);		// Create the main system object.
	checkErr(result, std::string("FMOD error! ") + std::string(FMOD_ErrorString(result)));
	
	result = system->setSoftwareChannels(100);	// Allow 100 software mixed voices to be audible at once.
	checkErr(result, std::string("FMOD error! ") + std::string(FMOD_ErrorString(result)));

	result = system->init(100, FMOD_INIT_NORMAL, 0);	// Initialize FMOD.
	checkErr(result, std::string("FMOD error! ") + std::string(FMOD_ErrorString(result)));

	return result;
}

void GameApp::notify(Subject* s)	// Observer pattern callback hook method.
{
	bBackgroundSound = ((Logic*) s)->bBackgroundSound;
	playBackgroundSound();			// Play according to the new options.
}

void GameApp::registerObservers()
{
	logic.registerObserver( dynamic_cast<Observer*>(this) );	// Add the GameApp instance.
	// Register screens as observers.
	logic.registerObserver( dynamic_cast<Observer*>(startScreen.get()) );
	logic.registerObserver( dynamic_cast<Observer*>(optionsScreen.get()) );
	logic.registerObserver( dynamic_cast<Observer*>(howtoScreen.get()) );
	logic.registerObserver( dynamic_cast<Observer*>(playScreen.get()) );
}

void GameApp::playBackgroundSound()
{
	if(bBackgroundSound)
	{		
		// Play Options background sound.
		if(logic.bShowStartScreen || logic.bShowOptionsScreen || logic.bShowHowtoScreen){
			//channelPlay->setPaused(true);
			for(std::size_t i = 0; i < channelRound.size(); i++)
				channelRound[i]->setPaused(true);
			channelOptions->setPaused(false);
		}
		else if(logic.bShowPlayScreen){		// Play the game sound using the corresp. theme.	
			channelOptions->setPaused(true);
			if(!logic.bGamePaused)
				//channelPlay->setPaused(false);
				for(std::size_t i = 0; i < channelRound.size(); i++){
					if(i != (logic.iRound - 1)%3 )
						channelRound[i]->setPaused(true);
					else
						channelRound[i]->setPaused(false);
				}
			else
				for(std::size_t i = 0; i < channelRound.size(); i++)
					channelRound[i]->setPaused(true);
		}
	}
	else	// Pause all the sounds.
	{
		channelOptions->setPaused(true);
		//channelPlay->setPaused(true);
		for(std::size_t i = 0; i < channelRound.size(); i++)
			channelRound[i]->setPaused(true);
	}
}

void GameApp::setupRoundParams()
{
	roundParams.resize(logic.iRoundMax);

	float flBoxWidth = 1.2f;
	float flBoxHeight = 1.0f;
	float flBoxThickness = 0.6f;
	float flBallVel = 4e-03;
	float flCompPaddleVel = 0.03;

	std::size_t nRounds = roundParams.size();
	// Specify different values of the parameters for each round.
	for(std::size_t i = 0; i < nRounds; i++){
		RoundParameters *pParams =
			new RoundParameters(flBoxWidth*(1. - 0.5/(double)(nRounds - i + 1.)), 
			flBoxHeight, 
			flBallVel,//*(1. - 1./(double)(nRounds - i + 1.)), 
			0.02*(1. + i),
			flCompPaddleVel*(1. + 0.5/(double)(nRounds - i + 1.)), 
			0.07*flBoxWidth*(1. - 0.5/(double)(nRounds - i + 1.)) );
		roundParams[i] = std::tr1::shared_ptr<RoundParameters>(pParams);
	}
}

void GameApp::checkErr(FMOD_RESULT r, std::string &msg)
{
	if( r != FMOD_OK){
		std::cerr << msg << std::endl;
	}
}