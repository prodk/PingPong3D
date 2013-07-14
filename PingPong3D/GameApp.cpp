// GameApp.cpp - implementation of the GameApp class.
// (c) Nikolay Prodanov, Juelich, summer 2013.

#include "PingPong3D.h"

GameApp::GameApp(void):
	flScreenWidth(1024), flScreenHeight(640), strGameName("Ping Pong"), flZaxisDistance(2.2f),
		flLengthUnit(0.4f), MAXHITS(64), selectRegion(5),
		logic(true, true, false, false) // Show options, running/paused/over.
{
	// !Add function to init these vars!
	flBoxWidth = 1.3f;		// Add these to the constructor and use when init walls.
	flBoxHeight = 1.0f;
	flBoxThickness = 0.5f;
	xViewOld = xView = 0.;
	angleViewY = 115.;		// Initial angle (around y) of the view.
	angleViewZ = 0.;
	flScaleAll = 1.;
	bPaddlePicked = false;

	xPaddleOld = yPaddleOld = 0.;

	initLibraries();	// !Check return value later.
	loadData();
	addShapes();

	// Init options screen.
	optionsScreen = 
		std::tr1::shared_ptr<OptionsScreen>(
		new OptionsScreen(flScreenWidth, flScreenHeight, surface, textures[0].get()) );
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
					SDL_OPENGL | SDL_SWSURFACE | SDL_RESIZABLE | SDL_DOUBLEBUF);
	SDL_WM_SetCaption(strGameName.c_str(), strGameName.c_str());

	return 0;
}

void GameApp::loadData()
{
	// Load sound and textures here.
	textures.resize(1);
	std::tr1::shared_ptr<TEXTURE> tmp;
	tmp = loadTexture("car.png");
	textures[0] = tmp;
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
        //printf("Could not load image: %s", fileName.c_str());
        return NULL;
    }
    
    // Work out what format to tell glTexImage2D to use...
    if (surface->format->BytesPerPixel == 3) // RGB 24bit
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
        //printf("Could not create TEXTURE struct for image: %s", fileName.c_str());
		return NULL;
    }
    
    // Set up TEXTURE structure.
    texture->width = surface->w;
    texture->height = surface->h;
    texture->id = textureid;    
    
    // Clean up.
    SDL_FreeSurface(surface);
    
    // Return pointer to image structure.
    return texture;
}

void GameApp::addShapes()
{
	// Add shapes to the game.
	shapes.resize(8);	// ball + 6 walls + 2 paddles

	// Add ball
	vector_3d ambient = vector_3d(0.0, 0.5, 0.0);
	vector_3d diffuse = vector_3d(0.0, 1.0, 0.0);
	vector_3d specular = vector_3d(0.0, 0.0, 0.0);
	float alpha = 1.0;	// Opaque ball.
	float shine = 0.;
	vector_3d center(0.0f, 0.0f, 0.0f);			// Ball starts at the center of the scene.
	vector_3d velocity(-4e-03f, -2e-03f, 2e-03f);		// Random initial velocity.
	Ball* pBall = new Ball(BALL, center, 0.05*flBoxHeight, velocity,
		ambient, diffuse, specular, shine, alpha);
	Shape* pShape = dynamic_cast<Shape*>(pBall);	
	shapes[0] = std::tr1::shared_ptr<Shape>(pShape);

	// Add walls.
	// Left wall.
	center = vector_3d(-0.5*flBoxWidth, 0., 0.);
	vector_3d n(-1., 0., 0.);
	Wall* pWall = new AbsorbingWall( WALL, center, flBoxThickness, flBoxHeight, n);
	pShape = dynamic_cast<Shape*>(pWall);
	shapes[1] = std::tr1::shared_ptr<Shape>(pShape);

	// Right wall.
	center = vector_3d(0.5*flBoxWidth, 0., 0.);
	n = vector_3d(1., 0., 0.);
	pWall = new Wall( WALL, center, flBoxThickness, flBoxHeight, n);
	pShape = dynamic_cast<Shape*>(pWall);
	shapes[2] = std::tr1::shared_ptr<Shape>(pShape);

	// Top wall.
	ambient = vector_3d(0.0, 0.0, 0.5);
	diffuse = vector_3d(0.0, 0.0, 1.0);
	specular = vector_3d(0.0, 0.0, 0.2);
	alpha = 0.1;	// A bit transparent wall.
	shine = 20.;
	center = vector_3d(0., 0.5*flBoxHeight, 0.);
	n = vector_3d(0., 1., 0.);
	pWall = new Wall( WALL, center, flBoxWidth, flBoxThickness, n,
		ambient, diffuse, specular, shine, alpha);
	pShape = dynamic_cast<Shape*>(pWall);
	shapes[3] = std::tr1::shared_ptr<Shape>(pShape);

	// Bottom wall.
	ambient = vector_3d(0.0, 0.0, 0.5);
	diffuse = vector_3d(0.0, 0.0, 1.0);
	specular = vector_3d(0.0, 0.0, 0.2);
	alpha = 0.1;	// Abit transparent wall.
	shine = 20.;
	center = vector_3d(0., -0.5*flBoxHeight, 0.);
	n = vector_3d(0., -1., 0.);
	pWall = new Wall( WALL, center, flBoxWidth, flBoxThickness, n,
		ambient, diffuse, specular, shine, alpha);
	pShape = dynamic_cast<Shape*>(pWall);
	shapes[4] = std::tr1::shared_ptr<Shape>(pShape);

	// Front wall.
	center = vector_3d(0., 0., 0.5*flBoxThickness);
	n = vector_3d(0., 0., 1.);
	pWall = new Wall( WALL, center, flBoxWidth, flBoxHeight, n);
	pShape = dynamic_cast<Shape*>(pWall);
	shapes[5] = std::tr1::shared_ptr<Shape>(pShape);

	// Back wall.
	center = vector_3d(0., 0., -0.5*flBoxThickness);
	n = vector_3d(0., 0., -1.);
	pWall = new Wall( WALL, center, flBoxWidth, flBoxHeight, n);
	pShape = dynamic_cast<Shape*>(pWall);
	shapes[6] = std::tr1::shared_ptr<Shape>(pShape);

	// User (left) paddle.
	center = vector_3d(-0.5*flBoxWidth, 0., 0.);
	n = vector_3d(-1., 0., 0.);			// Norm is -x.
	float angle = 90.0;
	// Setup colors.
	ambient = vector_3d(0.5, 0.5, 0.0);
	diffuse = vector_3d(1.0, 1.0, 0.0);
	specular = vector_3d(0.5, 0.5, 0.0);
	alpha = 0.9;	// A bit transparent paddle.
	shine = 10.;
	Paddle *pPaddle = new Paddle( LEFT_PADDLE, center, n, 0.05*flBoxWidth, 0.01*flBoxWidth, 
		90., 0.5*flBoxHeight, 0.5*flBoxThickness,
		ambient, diffuse, specular, shine, alpha);
	pShape = dynamic_cast<Shape*>(pPaddle);
	shapes[7] = std::tr1::shared_ptr<Shape>(pShape);
	leftPaddleIdx = 7;
}

void GameApp::setupRenderingContext()
{
// Here working and understood code starts.
	
	// Shading model - Gouraud (smooth).
	glShadeModel( GL_SMOOTH );

	// Culling.
	//glCullFace( GL_BACK );
	//glFrontFace( GL_CCW );
	//glEnable( GL_CULL_FACE );

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

   // GLOBAL light settings.
   GLfloat global_ambient[] = { 0.3f, 0.3f, 0.3f, 1.0f };
   glLightModelfv(GL_LIGHT_MODEL_AMBIENT, global_ambient);

   //glAlphaFunc(GL_GREATER, (GLclampf)0.01); // Skip pixels which alpha channel is lower than 0.01.
   //glEnable(GL_TEXTURE_2D);  // Enable 2D texturing.

   glEnable(GL_ALPHA_TEST);  // Enable Alpha.

  
// Here working and understood code ends.

 //   glFrontFace(GL_CCW); // counter clockwise polys face out.
  
 
	glPointSize(20); // size of points.

}

void GameApp::setupMatrices()
{ 
// Here working and understood code starts.	
	initResize();

// Here working and understood code ends.	
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
	//sleep(0); // Give back spare cpu cycles.
    
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

void GameApp::play()
{
	//while(logic.bGameRunning)
    //{  
		if(!logic.bGamePaused)
			doLogic();

        doDrawing();
        
        //swapBuffers();
    //}
}

void GameApp::manageGame()
{
	while(logic.bAppRunning) 
	{
		if(logic.bShowOptions){
			doInput(); 
			optionsScreen->show();
			//swapBuffers();
		}
		else
			play();	

		swapBuffers();
	}
}

void GameApp::doLogic()
{
	// Make sure input isn't affected by the timer.
    doInput();    
    updateTimers();
    
    // If the player hasn't lost, do stuff.
    if(!logic.bGameOver)
    {		
		// Move the shapes.
		bool bReset = false;
		for(std::size_t i = 0; i < shapes.size(); i++)
			shapes[i]->move(deltaTime, vector_3d(0., 0., 0.), bReset);

		// Detect collisions for all shapes with the ball.
		for(std::size_t i = 0; i < shapes.size(); i++)
			shapes[i]->collide(shapes[0].get());
		
		//playSound(samples[SAM_BRICKMOVE]);        
    } // if(!game_over)
    
    // Update other general logic
    // theta += time_delta * 0.01;
    // secondary timer for move_down input so you cant drop a brick too fast!
    //if(secondary_timer > 0)
        //secondary_timer -= time_delta * 0.1;
        
    // update scoring glow
    //if(score_glow > GLOW_MIN)
        //score_glow -= time_delta * 0.001;
    //if(score_glow < GLOW_MIN)
        //score_glow = GLOW_MIN;
        
    // update other stuff
    //updateStarRank();
    //updateSkillLevel();
    
    // if the player lost, do game over effects
    //if(game_over){
        //updateGameOver();
    //}   
}

void GameApp::doDrawing()
{	
// Here working and understood code starts.
	initResize();
	initView();

	// Code up to here put into the InitView method. Use it also in the pickObjects.
	
	// Draw all the shapes.
	for(std::size_t i = 0; i < shapes.size(); i++){
		//glColor3f ((float)shapes[i]->getId(), 0.0, 0.0);
		shapes[i]->draw();
	}

	glFlush();

// End of working and understood code.
                
    /************************************************************
    /* Draw HUD (score, star rank etc)
    /************************************************************/
    /*enableOrtho2D();

        draw2DTexture(0, 0, textures[TEX_HEADER]);
        draw2DNumberFromRight(788, 27, textures[TEX_NUMBERS], 14, 16, score);
        drawStarRank();
        
        if(game_over)
            drawGameOver();
            
    disableOrtho2D();*/
}

void GameApp::drawAxes()
{
	glBegin(GL_LINES);
	// x, red
	glColor3f(2.f, 0.f, 0.f);
	glVertex3f(-2.f, 0.f, 0.f);
	glVertex3f(1.f, 0.f, 0.f);
	// y, green
	glColor3f(0.f, 2.f, 0.f);
	glVertex3f(0.f, -2.f, 0.f);
	glVertex3f(0.f, 1.f, 0.f);
	// z, blue
	glColor3f(0.f, 0.f, 2.f);
	glVertex3f(0.f, 0.f, -20.f);
	glVertex3f(0.f, 0.f, 1.f);
	glEnd();
}

void GameApp::doInput()
{
	SDL_PollEvent(&sdlEvent); // check input
	        
    switch(sdlEvent.type)
    {
	case SDL_MOUSEBUTTONDOWN:
		handleMouseButtonDown(sdlEvent);
		break;

	case SDL_MOUSEBUTTONUP:
		handleMouseButtonUp(sdlEvent);
		break;

	case SDL_MOUSEMOTION:
		handleMouseMotion(sdlEvent);
		break;
	
	case SDL_KEYDOWN:
		handleKeyDown(sdlEvent);
		break;

	case SDL_KEYUP:
		handleKeyUp(sdlEvent);
		break;

	case SDL_VIDEORESIZE:
		handleResize(sdlEvent);
		break;			

	case SDL_QUIT:
		logic.bAppRunning = false;
		break;
	}
}

void GameApp::handleMouseButtonDown(const SDL_Event& sdle)
{
	int objName = 0;
	switch(sdle.button.button)
	{
	case SDL_BUTTON_LEFT:
		//bPaddlePicked = pickPaddle(sdle.button.x, sdle.button.y);
		objName = pickObject(sdle.button.x, sdle.button.y);
		if(objName == LEFT_PADDLE){
			bPaddlePicked = true;
			xPaddleOld = sdle.button.x;
			yPaddleOld = sdle.button.y;
		}
		xViewOld = sdle.button.x;
//!Debug.
		//logic.bShowOptions = !logic.bShowOptions;
//!End debug.
		break;
	case SDL_BUTTON_WHEELUP:
		angleViewZ -= 0.3;			// rotate around the horizontal axis.
		break;
	case SDL_BUTTON_WHEELDOWN:
		angleViewZ += 0.3;
		break;
	}// end switch
}

void GameApp::handleMouseButtonUp(const SDL_Event& sdle)
{
	switch(sdle.button.button)
	{
	case SDL_BUTTON_LEFT:
		bPaddlePicked = false;
		break;
	}
}

void GameApp::handleMouseMotion(const SDL_Event& sdle)
{
	switch(sdle.motion.state)
	{
	case SDL_BUTTON_LEFT:
// !Add later: Rotate view only if no objects are selected and bPaddlePicked == false.
		if(!bPaddlePicked){
			angleViewY += sdle.motion.x - xViewOld;
			xViewOld = sdle.motion.x;
		}
		else{
			vector_3d dr(0., -0.0025*(sdle.motion.y - yPaddleOld), 
				0.0025*(sdle.motion.x - xPaddleOld) );
			shapes[leftPaddleIdx]->move(0., dr, false);
			xPaddleOld = sdle.motion.x;
			yPaddleOld = sdle.motion.y;
		}
		break;
	}
}

void GameApp::handleKeyDown(const SDL_Event& sdle)
{	
	// Check keyboard.
	switch(sdle.key.keysym.sym)
	{
	case SDLK_EQUALS:		// Reset view.
		angleViewY = 115;
		angleViewZ = 0.;
		flScaleAll = 1.;
		break;

	case SDLK_p:	 //Scale.
		flScaleAll += 0.01;
		break;

	case SDLK_m:
		flScaleAll -= 0.01;
		break;

	case SDLK_ESCAPE:
		logic.bAppRunning = false;
		break;

		// Arrow Key Style
	case SDLK_a:			// Fall through.
	case SDLK_LEFT:{ 
		vector_3d dr(0., 0.0, 0.01);
		shapes[leftPaddleIdx]->move(0., dr, false);
				   }break;
	case SDLK_d:			// Fall through.
	case SDLK_RIGHT:{ 
		vector_3d dr(0., 0.0, -0.01);
		shapes[leftPaddleIdx]->move(0., dr, false);
					}break;

	case SDLK_w:			// Fall through.
	case SDLK_UP:{  
		vector_3d dr(0., 0.01, 0.);
		shapes[leftPaddleIdx]->move(0., dr, false);
				 }break;

	case SDLK_s:			// Fall through.
	case SDLK_DOWN:{
		vector_3d dr(0., -0.01, 0.);
		shapes[leftPaddleIdx]->move(0., dr, false);
				   }break;
	}
}

void GameApp::handleKeyUp(const SDL_Event& sdle)
{
	switch(sdle.key.keysym.sym)
	{
	case SDL_MOUSEBUTTONUP:
		//printf("Mouse button %d pressed at (%d,%d)\n",
		//event.button.button, event.button.x, event.button.y);
		break;

	case SDLK_b:
		if(logic.bShowOptions)
			logic.bShowOptions = false;
		else
			logic.bShowOptions=true;
		break;
		swapBuffers();
		// Arrow Key Style
	case SDLK_LEFT:                
		break;
	case SDLK_RIGHT:               
		break;
	case SDLK_UP:              
		break;
	case SDLK_DOWN:               
		break;
		// WASD Style
	case SDLK_a:               
		break;             
	case SDLK_d:
		break;            
	case SDLK_w:
		break;
	case SDLK_s:                
		break;
	}
}

void GameApp::swapBuffers()
{	
	glFlush();
    SDL_GL_SwapBuffers(); // Put it on the screen.
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear buffer for new data.
}

void GameApp::handleResize(const SDL_Event& sdle)
{
	SDL_SetVideoMode( sdlEvent.resize.w, sdlEvent.resize.h, 32, SDL_OPENGL | SDL_HWSURFACE | SDL_RESIZABLE );
	glViewport (0, 0, (GLsizei) sdle.resize.w, (GLsizei) sdle.resize.h);
	flScreenWidth = sdle.resize.w;
	flScreenHeight = sdle.resize.h;

	initResize();		// Avoid code duplication with setup matrices.
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
	if(surface != NULL){
		SDL_FreeSurface(surface);
		surface = NULL;
	}
    SDL_Quit();
}

// Object selection using back buffer.
int GameApp::pickObject(int x, int y)
{
	// Render the scene into the back buffer with colors corresponding to
	// names of the objects.
	initView();		// Avoid code duplication with doDrawing().

	glDisable(GL_LIGHTING);
	// Draw all the shapes.
	for(std::size_t i = 0; i < shapes.size(); i++)
	{
		int name = shapes[i]->getId();
		//name = 0.2*name;
			
		glColor3f (0.3*name, 0., 0.);		// !Reconsider this later! May be too simple.
		shapes[i]->draw();
	}

	glFlush();

	// Now read the pixel under cursor and analyze the color.
	GLint viewport[4];	
	glGetIntegerv (GL_VIEWPORT, viewport);

	GLint mouse_x = x;
	GLint mouse_y = viewport[3] - y;

	float pixel[3];
    glReadPixels(mouse_x, mouse_y, 1, 1, GL_RGB, GL_FLOAT, pixel);
    float r = pixel[0];
    float g = pixel[1];
    float b = pixel[2];

	int iFigureType = (int)(r/0.3);

	glEnable(GL_LIGHTING);
   
	return iFigureType;
}

void GameApp::initView()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear buffer for new data.
	//glClear (GL_COLOR_BUFFER_BIT);
	glLoadIdentity ();             // Clear the current matrix.
	// Move camera to the point (0,0,5) in eye coords, look at point (0,0,0), 
	// camera orientation - normal is along (0,1,0)
	gluLookAt (0.0, 0.0, flZaxisDistance, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	
	glRotatef(angleViewY, 0.0f, 1.0f, 0.0f);
	glRotatef(angleViewZ, 0.0f, 0.0f, 1.0f);
	glScalef(flScaleAll, flScaleAll,flScaleAll);

	drawAxes();	
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