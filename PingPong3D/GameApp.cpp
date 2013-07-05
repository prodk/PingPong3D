// GameApp.cpp - implementation of the GameApp class.
// (c) Nikolay Prodanov, Juelich, summer 2013.

#include "PingPong3D.h"

GameApp::GameApp(void):
	flScreenWidth(1024),flScreenHeight(640),strGameName("Ping Pong"),flZaxisDistance(2.0f),
		flLengthUnit(0.35f),bRunning(true),bGameOver(false)
{
	flBoxWidth = 1.6f;		// Add these to the constructor and use when init walls.
	flBoxHeight = 1.0f;
	flBoxThickness = 0.5f;
	initLibraries();	// !Check return value later.
	loadData();
	addShapes();
}

GameApp::~GameApp(void)
{
	shutDown();		// Shapes are handled via smart pointers, so no delete here.	
}

int GameApp::initLibraries()
{
	// Initialize SDL.
	if( setupSDL() < 0 ){
		SDL_Quit();
		return -1;
	}
    
    // OpenGL.
    setupRenderingContext();
    setupMatrices();
    
    // Setup fmod - add later.
    //setupSound();
    
    // Set the game timer up.
    setupTimers();

    // Seed random numbers.
    std::srand ((unsigned int)time(NULL));
    
    // Initialize new game.
    setupNewGame();
}

int GameApp::setupSDL()
{
	if( SDL_Init(SDL_INIT_VIDEO) < 0 ){
		SDL_Quit();
		return -1;
	}	

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

    SDL_SetVideoMode((int)flScreenWidth, (int)flScreenHeight, 0, SDL_OPENGL | SDL_HWSURFACE | SDL_RESIZABLE | SDL_DOUBLEBUF);
	SDL_WM_SetCaption(strGameName.c_str(), strGameName.c_str());
}

void GameApp::loadData()
{
	// Load sound and textures here.
}

void GameApp::addShapes()
{
	// Add shapes to the game.
	shapes.resize(8);	// ball + 6 walls + 2 paddles
	// Add ball
	vector_3d center(0.0f, 0.0f, 0.0f);			// Ball starts at the center of the scene.
	vector_3d velocity(5e-03f, 6e-03f, 5e-03f);		// Random initial velocity.
	Ball* pBall = new Ball(BALL, 0.05, center, velocity);
	Shape* pShape = dynamic_cast<Shape*>(pBall);	
	shapes[0] = std::tr1::shared_ptr<Shape>(pShape);

	// Add walls.
	// Left wall.
	center = vector_3d(-0.5*flBoxWidth, 0., 0.);
	vector_3d n(-1., 0., 0.);
	Wall* pWall = new AbsorbingWall( WALL, flBoxThickness, flBoxHeight, center, n);
	pShape = dynamic_cast<Shape*>(pWall);
	shapes[1] = std::tr1::shared_ptr<Shape>(pShape);

	// Right wall.
	center = vector_3d(0.5*flBoxWidth, 0., 0.);
	n = vector_3d(1., 0., 0.);
	pWall = new Wall( WALL, flBoxThickness, flBoxHeight, center, n);
	pShape = dynamic_cast<Shape*>(pWall);
	shapes[2] = std::tr1::shared_ptr<Shape>(pShape);

	// Top wall.
	center = vector_3d(0., 0.5*flBoxHeight, 0.);
	n = vector_3d(0., 1., 0.);
	pWall = new Wall( WALL, flBoxWidth, flBoxThickness, center, n);
	pShape = dynamic_cast<Shape*>(pWall);
	shapes[3] = std::tr1::shared_ptr<Shape>(pShape);

	// Bottom wall.
	center = vector_3d(0., -0.5*flBoxHeight, 0.);
	n = vector_3d(0., -1., 0.);
	pWall = new Wall( WALL, flBoxWidth, flBoxThickness, center, n);
	pShape = dynamic_cast<Shape*>(pWall);
	shapes[4] = std::tr1::shared_ptr<Shape>(pShape);

	// Front wall.
	center = vector_3d(0., 0., 0.5*flBoxThickness);
	n = vector_3d(0., 0., 1.);
	pWall = new Wall( WALL, flBoxWidth, flBoxHeight, center, n);
	pShape = dynamic_cast<Shape*>(pWall);
	shapes[5] = std::tr1::shared_ptr<Shape>(pShape);

	// Back wall.
	center = vector_3d(0., 0., -0.5*flBoxThickness);
	n = vector_3d(0., 0., -1.);
	pWall = new Wall( WALL, flBoxWidth, flBoxHeight, center, n);
	pShape = dynamic_cast<Shape*>(pWall);
	shapes[6] = std::tr1::shared_ptr<Shape>(pShape);

	// User (left) paddle.
	center = vector_3d(-0.5*flBoxWidth, 0., 0.);
	n = vector_3d(-1., 0., 0.);			// Norm is -x.
	float angle = 90.0;
	Paddle *pPaddle = new Paddle( PADDLE, center, n, 0.15*flBoxWidth, 0.01*flBoxWidth, 
		90., 0.5*flBoxHeight, 0.5*flBoxThickness);
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

	glClearColor(0.4f, 0.4f, 0.4f, 0.0f);	// Background color.
	glShadeModel(GL_SMOOTH);				//Smooth shading.

	// Depth buffer.
	glClearDepth(1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);				// Enable z-buffer.
	glDepthFunc(GL_LEQUAL);

	glViewport(0, 0, (GLsizei)flScreenWidth, (GLsizei)flScreenHeight); // Set viewport to window dimensions.

// Here working and understood code ends.

 //   glFrontFace(GL_CCW); // counter clockwise polys face out.
 //
	//glAlphaFunc(GL_GREATER, (GLclampf)0.01); // Skip pixels which alpha channel is lower than 0.01.
 //   glEnable(GL_TEXTURE_2D);  // Enable 2D texturing.
 //   glEnable(GL_BLEND);  // Enable Blending.
 //   glEnable(GL_ALPHA_TEST);  // Enable Alpha.
 //   glEnable(GL_CULL_FACE);  // Make sure face culling is enabled (speed demon ;).
	//glBlendFunc(GL_SRC_ALPHA , GL_ONE_MINUS_SRC_ALPHA);  // Turn On The Blender.
	//
	//glPointSize(20); // size of points.
 //   
    //glPolygonMode(GL_BACK, GL_FILL);
 //   glPolygonMode(GL_BACK, GL_NONE);
 //   

 //   // Enable Lighting.
 //   glEnable(GL_LIGHTING);

 //   // Set up light0.
 //   GLfloat ambientLight[] = { 1.0f, 1.0f, 1.0f, 1.0f };
 //   GLfloat diffuseLight[] = { 1.0f, 1.0f, 1.0f, 1.0f };
 //   GLfloat specularLight[] = { 0.0f, 0.0f, 0.0f, 1.0f };
 //   GLfloat position[] = {  0.0, 40.0, -20.0, 1.0 };
    
 //   glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
 //   glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
 //   glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);
 //   glLightfv(GL_LIGHT0, GL_POSITION, position);
 //   
 //   // Create the newly set up lightsource.
 //   glEnable(GL_LIGHT0);

 //   // GLOBAL light settings.
 //   GLfloat global_ambient[] = { 0.6f, 0.6f, 0.6f, 1.0f };
 //   glLightModelfv(GL_LIGHT_MODEL_AMBIENT, global_ambient);
 //   
 //   
 //   // Material color tracking.
 //   glEnable(GL_COLOR_MATERIAL);
 //      //Make it so ambient and diffuse material properties will use glcolor
 //      //Soo...  glColor3f(0.0f, 0.0f, 1.0f); would make blue reflective properties
 //      //Exactly like calling:
 //      //float mcolor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
 //      //glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, mcolor);
 //      //before the geometry
 //   glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
}

void GameApp::setupMatrices()
{ 
// Here working and understood code starts.	
    glMatrixMode (GL_PROJECTION);	// Switch to the projection matrix.
	glLoadIdentity ();				// Clean up the projection matrix.
	double angle = calculateAngle(flLengthUnit, flZaxisDistance);	// View frustum angle.
	float ratio = (float) flScreenWidth / (float) flScreenHeight;
	gluPerspective(angle, ratio, 1.0, 1024.0);	// Set up the projection matrix with the same units in x and y.
	glMatrixMode (GL_MODELVIEW);	// Switch to the modelview matrix.

// Here working and understood code ends.
    
	
    // Reset texture view matrix stack.
    //glMatrixMode(GL_TEXTURE);
    //glLoadIdentity();
    
    // Reset model view matrix stack
    //glMatrixMode(GL_MODELVIEW); 
    //glLoadIdentity();
    // ... and keep it on for rendering.
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

int GameApp::play()
{
	while(bRunning)
    {  
        doLogic();
        doDrawing();
        
        swapBuffers();
    }

	return 0;
}

void GameApp::doLogic()
{
	// Make sure input isn't affected by the timer.
    doInput();    
    updateTimers();
    
    // If the player hasnt lost, do stuff.
    if(!bGameOver)
    {
		//if(bLeftMouseDown == false){
		// Move the shapes.
		bool bReset = false;
		for(std::size_t i = 0; i < shapes.size(); i++)
			shapes[i]->move(deltaTime, vector_3d(0., 0., 0.), bReset);

		// Detect collisions for all shapes with the ball.
		for(std::size_t i = 0; i < shapes.size(); i++)
			shapes[i]->collide(shapes[0].get());
		//shapes[1]->collide(shapes[0].get());
		//shapes[0]->collide(NULL);
		//}
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
	glClear (GL_COLOR_BUFFER_BIT);
	glColor3f (1.0, 0.0, 1.0);
	glLoadIdentity ();             // Clear the current matrix.
	// Move camera to the point (0,0,5) in eye coords, look at point (0,0,0), 
	// camera orientation - normal is along (0,1,0)
	gluLookAt (0.0, 0.0, flZaxisDistance, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	// Rotate scene a bit around y and then around new x.
	glRotatef(-15.0f, 0.0f, 1.0f, 0.0f);
	glRotatef(3.0f, 1.0f, 0.0f, 0.0f);

	// Bigger sphere.
	//glutSolidSphere (1., 32, 32);	

	drawAxes();

	//glColor3f (1.0, 1.0, 1.0);
	//glutWireCube (2.0);
	
	// Draw all the shapes.
	for(std::size_t i = 0; i < shapes.size(); i++)
		shapes[i]->draw();

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
        case SDL_KEYDOWN:
			handleKeyDown(sdlEvent.key.keysym);
			break;
        
        case SDL_KEYUP:
			handleKeyUp(sdlEvent.key.keysym);
			break;
			
		case SDL_VIDEORESIZE:
			handleResize(sdlEvent);
			break;			
            
        case SDL_QUIT:
            bRunning = false;
        break;
    }
}

void GameApp::handleKeyDown(const SDL_keysym& keysym)
{
	switch(keysym.sym)
	{
	case SDL_MOUSEMOTION:
		//leftB=(event->button.down==SDL_BUTTON_LEFT);
		//rightB(event->button.down==SDL_BUTTON_RIGHT);
		//if (leftB && rightB)//{// here both buttons are pressed//}
		break;
	case SDL_MOUSEBUTTONDOWN:
		//bLeftMouseDown = true;
		//printf("Mouse button %d pressed at (%d,%d)\n",
		//event.button.button, event.button.x, event.button.y);
		break;

	case SDLK_ESCAPE:
		bRunning = false;
		break;

		// Arrow Key Style
	case SDLK_LEFT:{ 
		vector_3d dr(0., 0.0, 0.01);
		shapes[leftPaddleIdx]->move(0., dr, false);
				   }break; 
	case SDLK_RIGHT:{ 
		vector_3d dr(0., 0.0, -0.01);
		shapes[leftPaddleIdx]->move(0., dr, false);
					}break;

	case SDLK_UP:{  
		vector_3d dr(0., 0.01, 0.);
		shapes[leftPaddleIdx]->move(0., dr, false);
				 }break;

	case SDLK_DOWN:{
		vector_3d dr(0., -0.01, 0.);
		shapes[leftPaddleIdx]->move(0., dr, false);
				   }break;

		// WASD Style
	case SDLK_a:{               
				}break; 

	case SDLK_d:{               
				}break;

	case SDLK_w:{              
				}break;

	case SDLK_s:{                 
				}break;
	}
}

void GameApp::handleKeyUp(const SDL_keysym& keysym)
{
	switch(keysym.sym)
	{
	case SDL_MOUSEBUTTONUP:
		//printf("Mouse button %d pressed at (%d,%d)\n",
		//event.button.button, event.button.x, event.button.y);
		break;
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
    SDL_GL_SwapBuffers(); // Put it on the screen.
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear buffer for new data.
}

void GameApp::handleResize(const SDL_Event& sdle)
{
	SDL_SetVideoMode( sdlEvent.resize.w, sdlEvent.resize.h, 32, SDL_OPENGL | SDL_HWSURFACE | SDL_RESIZABLE );
	glViewport (0, 0, (GLsizei) sdle.resize.w, (GLsizei) sdle.resize.h);
	flScreenWidth = sdle.resize.w;
	flScreenHeight = sdle.resize.h;
	glMatrixMode (GL_PROJECTION);	// Switch to the projection matrix.
	glLoadIdentity ();				// Clean up the projection matrix.
	gluPerspective(calculateAngle(flLengthUnit, flZaxisDistance), 
		flScreenWidth/flScreenHeight, 1.0, 1024.0);	// Set up the projection matrix with the same units in x and y.
	glMatrixMode (GL_MODELVIEW);	// Switch to the modelview matrix.
	glLoadIdentity ();
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
    SDL_Quit();
}