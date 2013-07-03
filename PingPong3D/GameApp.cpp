// GameApp.cpp - implementation of the GameApp class.
// (c) Nikolay Prodanov, Juelich, summer 2013.

#include "PingPong3D.h"

GameApp::GameApp(void):
	flWidth(640),flHeight(480),strGameName("Ping Pong"),bRunning(true),bGameOver(false)
{
	initLibraries();
	loadData();
	addShapes();
}

GameApp::~GameApp(void)
{
	shutDown();
	//for(int i=0; i < shapes.size(); i++){
		//delete shapes[i];
		//shapes[i] = NULL;
	//}

}

void GameApp::initLibraries()
{
	// Initialize SDL.
    SDL_Init(SDL_INIT_VIDEO);
    SDL_SetVideoMode((int)flWidth, (int)flHeight, 0, SDL_OPENGL | SDL_HWSURFACE | SDL_RESIZABLE /*| SDL_DOUBLEBUF*/);
	SDL_WM_SetCaption(strGameName.c_str(), strGameName.c_str());

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

void GameApp::loadData()
{
	// Load sound and textures here.
}

void GameApp::addShapes()
{
	// Add shapes to the game.
	vector_3d center(0.0f, 0.0f, 0.0f);			// Ball starts at the center of the scene.
	vector_3d velocity(1e-04f, 0.0f, 0.0f);		// Random initial velocity.
	Ball* pBall = new Ball(BALL, 0.1, center, velocity);
	Shape* pShape = dynamic_cast<Shape*>(pBall);
	//ball = std::tr1::shared_ptr<Ball>(pBall);
	shapes.resize(1);
	shapes[0] = std::tr1::shared_ptr<Shape>(pShape);
}

void GameApp::setupRenderingContext()
{
// Here working and understood code starts.
	glClearColor(0.4f, 0.4f, 0.4f, 1.0f);	// Background color.
	glShadeModel(GL_SMOOTH);				//Smooth shading.

// Here working and understood code ends.


    //glClearDepth(1.0);						// Clear depth buffer.
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);    
 //   glEnable(GL_DEPTH_TEST);
 //   glDepthFunc(GL_LEQUAL);
 //   glFrontFace(GL_CCW); // counter clockwise polys face out.
 //
	//glAlphaFunc(GL_GREATER, (GLclampf)0.01); // Skip pixels which alpha channel is lower than 0.01.
 //   glEnable(GL_TEXTURE_2D);  // Enable 2D texturing.
 //   glEnable(GL_BLEND);  // Enable Blending.
 //   glEnable(GL_ALPHA_TEST);  // Enable Alpha.
 //   glEnable(GL_CULL_FACE);  // Make sure face culling is enabled (speed demon ;).
	//glBlendFunc(GL_SRC_ALPHA , GL_ONE_MINUS_SRC_ALPHA);  // Turn On The Blender.
	//
	//glPointSize(2); // size of points.
 //   
 //   glPolygonMode(GL_FRONT, GL_FILL);
 //   glPolygonMode(GL_BACK, GL_NONE);
 //   

 //   // Enable Lighting.
 //   glEnable(GL_LIGHTING);

 //   // Set up light0.
 //   GLfloat ambientLight[] = { 1.0f, 1.0f, 1.0f, 1.0f };
 //   GLfloat diffuseLight[] = { 1.0f, 1.0f, 1.0f, 1.0f };
 //   GLfloat specularLight[] = { 0.0f, 0.0f, 0.0f, 1.0f };
 //   GLfloat position[] = {  0.0, 40.0, -20.0, 1.0 };
 //   
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
	// glViewport can be skipped.
    glViewport(0, 0, (GLsizei)flWidth, (GLsizei)flHeight); // Set viewport to window dimensions.
    glMatrixMode (GL_PROJECTION);	// Switch to the projection matrix.
	glLoadIdentity ();				// Clean up the projection matrix.
	gluPerspective(30.0, flWidth/flHeight, 1.5, 20.0);	// Set up the projection matrix with the same units in x and y.
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
		// Move the shapes.
		for(std::size_t i = 0; i < shapes.size(); i++)
			shapes[i]->move(deltaTime);
		//Ball *ball = dynamic_cast<Shape*>(shapes[0]);
		//ball->move(deltaTime);
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
	gluLookAt (0.0, 0.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);	
	//glTranslatef(0.f, 0.f, -5.f);
	glutSolidSphere (1., 32, 32);	

	drawAxes();

	glColor3f (1.0, 1.0, 1.0);	

	glutWireCube (1.0);
	
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
	glVertex3f(0.f, 0.f, -2.f);
	glVertex3f(0.f, 0.f, 1.f);
	glEnd();
}

void GameApp::doInput()
{
	SDL_PollEvent(&sdlEvent); // check input
	        
    switch(sdlEvent.type)
    {
        case SDL_KEYDOWN:{
        switch(sdlEvent.key.keysym.sym)
        {
			 case SDL_MOUSEMOTION:
				 //leftB=(event->button.down==SDL_BUTTON_LEFT);
				 //rightB(event->button.down==SDL_BUTTON_RIGHT);
				 //if (leftB && rightB)
				 //{
					 // here both buttons are pressed
				 //}
                break;
            case SDL_MOUSEBUTTONDOWN:
                //printf("Mouse button %d pressed at (%d,%d)\n",
                       //event.button.button, event.button.x, event.button.y);
                break;

            case SDLK_ESCAPE:
                bRunning = false;
            break;
            
            // Arrow Key Style
            case SDLK_LEFT:{                
            }break; 
            
            case SDLK_RIGHT:{               
            }break;
            
            case SDLK_UP:{                
            }break;
            
            case SDLK_DOWN:{                   
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
        }break;
        
        case SDL_KEYUP:{
        switch(sdlEvent.key.keysym.sym)
        {
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
        }break;

		case SDL_VIDEORESIZE:
			SDL_SetVideoMode( sdlEvent.resize.w, sdlEvent.resize.h, 32, SDL_OPENGL | SDL_HWSURFACE | SDL_RESIZABLE );
			glViewport (0, 0, (GLsizei) sdlEvent.resize.w, (GLsizei) sdlEvent.resize.h);
			flWidth = sdlEvent.resize.w;
			flHeight = sdlEvent.resize.h;
			glMatrixMode (GL_PROJECTION);
			glLoadIdentity ();
			gluPerspective(30.0, flWidth/flHeight, 1.5, 20.0);	// Set the same units in x and y.
			glMatrixMode (GL_MODELVIEW);			
			break;
            
        case SDL_QUIT:
            bRunning = false;
        break;
    }
}

void GameApp::swapBuffers()
{	
	glFlush(); // Make sure everything is through the pipes.
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
    SDL_Quit();
}