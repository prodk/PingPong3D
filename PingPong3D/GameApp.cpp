// GameApp.cpp - implementation of the GameApp class.
// (c) Nikolay Prodanov, Juelich, summer 2013.

#include "PingPong3D.h"

GameApp::GameApp(void):
	flScreenWidth(1024), flScreenHeight(640), strGameName("Ping Pong"), 
		flZaxisDistance(2.2f), flLengthUnit(0.4f),
		logic(true, true, false, false) // Show options, running/paused/over.
{
	initLibraries();	// !Check return value later.
	loadData();

	// Init options screen.
	optionsScreen = std::tr1::shared_ptr<OptionsScreen>(
		new OptionsScreen(flScreenWidth, flScreenHeight, surface, textures) );

	// Init play screen.
	playScreen = std::tr1::shared_ptr<PlayScreen>(
		new PlayScreen(flScreenWidth, flScreenHeight, surface, textures) );

	// Init frustum.
	//playScreen->initView();
}

GameApp::~GameApp(void)
{
	shutDown();		// Shapes are handled via smart pointers, so no delete here.	
}

int GameApp::initLibraries()
{
	// Initialize SDL.
	if( setupSDL() < 0 ){ return -1; }	// Change this !
    
    // OpenGL.
    setupRenderingContext();
    setupMatrices();
    
    // Setup fmod - add later.
    //setupSound();				// Check return value.
    
    // Set the game timer up.
    setupTimers();

    // Seed random numbers.
    std::srand ((unsigned int)time(NULL));
    
    // Initialize new game.
    setupNewGame();

	return 0;
}

int GameApp::setupSDL()
{
	if( SDL_Init(SDL_INIT_VIDEO) < 0 )
		return -1;

	/* Let's get some video information. */
    const SDL_VideoInfo* info = SDL_GetVideoInfo( );
	if( !info ) {}	// Add later.
	int bpp = info->vfmt->BitsPerPixel;

	/*
     * Now, we want to setup our requested window attributes for our OpenGL window.
     * We want *at least* 5 bits of red, green and blue. We also want at least a 16-bit
     * depth buffer.
     * The last thing we do is request a double buffered window. '1' turns on double
     * buffering, '0' turns it off.     *
     * Note that we do not use SDL_DOUBLEBUF in the flags to SDL_SetVideoMode. That does
     * not affect the GL attribute state, only the standard 2D blitting setup.
     */
    //SDL_GL_SetAttribute( SDL_GL_RED_SIZE, 5 );
   // SDL_GL_SetAttribute( SDL_GL_GREEN_SIZE, 5 );
    //SDL_GL_SetAttribute( SDL_GL_BLUE_SIZE, 5 );
    //SDL_GL_SetAttribute( SDL_GL_DEPTH_SIZE, 16 );
    //SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );

	//int flags = SDL_OPENGL | SDL_FULLSCREEN;

	//if( SDL_SetVideoMode( flWidth, flHeight, bpp, flags ) == 0 ) {
        /* 
         * This could happen for a variety of reasons,
         * including DISPLAY not being set, the specified
         * resolution not being available, etc.
         */
       /* fprintf( stderr, "Video mode set failed: %s\n",
             SDL_GetError( ) );
        quit_tutorial( 1 );*/
    //}

    surface = SDL_SetVideoMode((int)flScreenWidth, (int)flScreenHeight, 0, 
					SDL_OPENGL | SDL_HWSURFACE | SDL_RESIZABLE | SDL_DOUBLEBUF);
	SDL_WM_SetCaption(strGameName.c_str(), strGameName.c_str());

	// Add icon here!

	return 0;
}

void GameApp::loadData()
{
	// Load sound and textures here.
	textures.resize(1);
	textures[0] = loadTexture("Button.png");
}

std::tr1::shared_ptr<TEXTURE> GameApp::loadTexture(std::string fileName)
{
    SDL_Surface* surface;
    GLuint textureid;
    int mode;

	surface = IMG_Load(fileName.c_str());
    
    // Could not load image.
    if (!surface) 
	{
        printf("Could not load image: %s", fileName.c_str());
        return NULL;
    }
    
    // Work out what format to tell glTexImage2D to use...
    if (surface->format->BytesPerPixel == 3) // RGB 24bit.
    { 
        mode = GL_RGB;
    } 
    else if (surface->format->BytesPerPixel == 4) // RGBA 32bit.
    { 
        mode = GL_RGBA;
    } 
    else 
    {
        printf("Could not determine pixel format of image: %s", fileName.c_str());
        SDL_FreeSurface(surface);
        return NULL;
    }

    // Create one texture name.
    glGenTextures(1, &textureid);

    // Tell opengl to use the generated texture name.
    glBindTexture(GL_TEXTURE_2D, textureid);

    // Read from the sdl surface and put it into an opengl texture.
	gluBuild2DMipmaps( GL_TEXTURE_2D, 3, surface->w, surface->h,
                       mode, GL_UNSIGNED_BYTE, surface->pixels );
    //glTexImage2D(GL_TEXTURE_2D, 0, mode, surface->w, surface->h, 0, mode, GL_UNSIGNED_BYTE, surface->pixels);

    // Set up texture filters.
    //glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    //glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

    // Allocate memory for Texture structure and fill it up.
	std::tr1::shared_ptr<TEXTURE> texture(new TEXTURE());
    
    // Could not allocate memory.
    if (!texture.get())
    {
        printf("Could not create TEXTURE struct for image: %s", fileName.c_str());
		return NULL;
    }
    
    // Set up TEXTURE structure.
    texture->width = surface->w;
    texture->height = surface->h;
    texture->id = textureid;    
    
    SDL_FreeSurface(surface);  // Clean up.
    
    return texture;
}

void GameApp::setupRenderingContext()
{
// Here working and understood code starts.
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
	glLineWidth (1.5);

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
   
   // Create the newly set up lightsource.
   glEnable(GL_LIGHT0);

   glEnable(GL_LIGHT1);		// Light moving with the ball.

   // Global light settings.
   GLfloat global_ambient[] = { 0.3f, 0.3f, 0.3f, 1.0f };
   glLightModelfv(GL_LIGHT_MODEL_AMBIENT, global_ambient);

   glPointSize(20);			// Size of points.
}

void GameApp::setupMatrices()
{ 
// Here working and understood code starts.	
	initResize();	

    // Reset texture view matrix stack.
    glMatrixMode(GL_TEXTURE);
    glLoadIdentity();
}

void GameApp::setupTimers()
{
	deltaTime = 0.0f;
    lastMillisec = SDL_GetTicks();
}

void GameApp::updateTimers()
{
    // Find delta time for this frame.
    deltaTime = 0;
    if(SDL_GetTicks() > lastMillisec)
    {
        deltaTime = SDL_GetTicks() - lastMillisec;
        lastMillisec = SDL_GetTicks();
    }
}

void GameApp::setupNewGame()
{
	/*
	// Clear board
    int i, j;
    for(i = GAMEBOARD_H-1; i >= 0; i--) 
        for(j = GAMEBOARD_W-1; j >= 0; j--)
            gameboard[i][j] = 0;
    
    // Randomize next brick
    brick_type_next = (rand() % MAX_BRICK_TYPES);
    brick_type_current = 0;         
    new_brick = true;  
    
    score = 0;
    level = 1;
    game_over = false;
	*/
}

void GameApp::manageGame()
{
	while(logic.bAppRunning) 
	{
		SDL_PollEvent(&sdlEvent);	// Important: use only one PollEvent!!!

		if(logic.bShowOptions){
			optionsScreen->doInput(logic, sdlEvent);	// Put These in a separate method.
			optionsScreen->doDrawing();
			swapBuffers();			
		}
		else {
			playScreen->play(logic, sdlEvent);
			/*playScreen->doInput(logic, sdlEvent);
			if(!logic.bGamePaused)
				playScreen->doLogic(logic);
			playScreen->doDrawing();*/
			swapBuffers();			
		}// End else.		
	}
}

void GameApp::initResize()
{
	glMatrixMode (GL_PROJECTION);	// Switch to the projection matrix.
	glLoadIdentity ();				// Clean up the projection matrix.
	gluPerspective(calculateAngle(flLengthUnit, flZaxisDistance), 
		flScreenWidth/flScreenHeight, 1.0, 1024.0);	// Set up the projection matrix with the same units in x and y.
	glMatrixMode (GL_MODELVIEW);	// Switch to the modelview matrix.
	glLoadIdentity ();
}

void GameApp::swapBuffers()
{	
	glFlush();
    SDL_GL_SwapBuffers(); // Put it on the screen.
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear buffer for new data.
}

void GameApp::shutDown()
{
	// free texture structs 
	/*
    int i;
    for(i = 0; i < TEXTURE_COUNT; i++)
        free(textures[i]);
        
    // glDeleteTextures( 1, &texture ); // Not really needed in this case. OGL deletes its own stuff

    // free sample data
    for(i = 0; i < SAMPLE_COUNT; i++)
        FSOUND_Sample_Free(samples[i]);

    // shutdown apis
    FSOUND_Close();
	*/

	// Most object pointers (e.g. screens) are cleaned up through the smart pointers.
	if(surface != NULL){
		SDL_FreeSurface(surface);
		surface = NULL;
	}
    SDL_Quit();
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