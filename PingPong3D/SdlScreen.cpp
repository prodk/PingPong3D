// SdlScreen.cpp - implementation of the SdlScreen class and all its child classes.
// (c) Nikolay Prodanov, Juelich, summer 2013.
#include "SdlScreen.h"

/*________________________________*/
// SdlScreen implementation.
SdlScreen::SdlScreen(float w, float h, SDL_Surface* s) :
flWidth(w), flHeight(h), surfaceOnWhichWeRender(s)
{
}

SdlScreen::~SdlScreen(void)
{
}

void SdlScreen::doInput(Logic &l, SDL_Event sdlEvent)
{
}

void SdlScreen::doDrawing(Logic &logic)
{
}

/*________________________________*/
// OptionsScreen implementation.
OptionsScreen::OptionsScreen(float w, float h, SDL_Surface* s, 
	TEXTURE_PTR_ARRAY t, TTF_Font** fnt,
	FMOD::System *sys, std::vector<FMOD::Sound*> snd):
	SdlScreen(w, h, s), textures(t), fonts(fnt), system(sys), sounds(snd)
{
	addButtons();
	bLeftMouseButton = true;
}

OptionsScreen::~OptionsScreen()
{
}

void OptionsScreen::addButtons()
{
	guiObjects.resize(4);
	float x = -1;
	float y = 0.5;
	float w = 1.;
	float h = 0.25;
	std::string name = "Start";

	// Later put the code for adding a button into a method to avoid code duplication!
	
	// Button 0.
	Button* pButton = new Button(x, y, w, h, START_BUTTON, name, textures[0]->id);
	GuiObject* pGuiObj = dynamic_cast<GuiObject*>(pButton);	
	pButton->setSound(system, sounds[1]);
	guiObjects[START_BUTTON] = std::tr1::shared_ptr<GuiObject>(pGuiObj);

	// Button 1.
	y = 0.125;
	name = "Options";
	pButton = new Button(x, y, w, h, OPTIONS_BUTTON, name, textures[0]->id);	// The same texture id.
	pGuiObj = dynamic_cast<GuiObject*>(pButton);
	pButton->setSound(system, sounds[1]);
	guiObjects[OPTIONS_BUTTON] = std::tr1::shared_ptr<GuiObject>(pGuiObj);

	// Button 2.
	y = -0.25;
	name = "Background Sound";
	pButton = new Button(x, y, w, h, BACKGR_SOUND_BUTTON, name, textures[0]->id);	// The same texture id.
	pGuiObj = dynamic_cast<GuiObject*>(pButton);
	pButton->setSound(system, sounds[1]);
	guiObjects[BACKGR_SOUND_BUTTON] = std::tr1::shared_ptr<GuiObject>(pGuiObj);

	// Button 3.
	y = -0.625;
	name = "Actions Sound";
	pButton = new Button(x, y, w, h, ACT_SOUND_BUTTON, name, textures[0]->id);	// The same texture id.
	pGuiObj = dynamic_cast<GuiObject*>(pButton);
	pButton->setSound(system, sounds[1]);
	guiObjects[ACT_SOUND_BUTTON] = std::tr1::shared_ptr<GuiObject>(pGuiObj);
}

void OptionsScreen::doDrawing(Logic &logic)
{	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear buffer for new data.

	glPushMatrix();

	glMatrixMode (GL_PROJECTION);	// Switch to the projection matrix.
	glLoadIdentity ();				// Clean up the projection matrix.

	glMatrixMode (GL_MODELVIEW);	// Switch to the modelview matrix.
	glLoadIdentity ();	

	glEnable( GL_TEXTURE_2D );
	
	for(std::size_t i = 0; i < guiObjects.size(); i++)
		guiObjects[i]->draw(fonts[0], logic);

	glFlush();

	glDisable( GL_TEXTURE_2D );

    glPopMatrix();
}

void OptionsScreen::doInput(Logic &logic, SDL_Event sdlEvent)
{
	//int typePrevious = -1;

    switch(sdlEvent.type)
    {
	case SDL_MOUSEBUTTONDOWN:
		bLeftMouseButton = true;
		handleMouseButtonDown(sdlEvent, logic);
		break;

	case SDL_MOUSEBUTTONUP:
		if(bLeftMouseButton){			
			handleMouseButtonUp(sdlEvent, logic);
			bLeftMouseButton = false;
		}
		break;

	case SDL_KEYDOWN:
		handleKeyDown(sdlEvent, logic);
		break;

	case SDL_QUIT:
		logic.bAppRunning = false;
		break;
	}
}

void OptionsScreen::handleKeyDown(const SDL_Event& sdle, Logic &logic)
{	
	// Check keyboard.
 	switch(sdle.key.keysym.sym)
	{
	case SDLK_h:
		if(logic.bShowOptions)
		{
			logic.bShowOptions = false;
			logic.bGamePaused = false;
		}
		break;
	case SDLK_k:
		//for(std::size_t i = 0; i < guiObjects.size(); i++)
			//guiObjects[i]->handleKeyDown(logic);
		break;
	}
}

void OptionsScreen::handleMouseButtonDown(const SDL_Event& sdle, Logic &logic)
{
	//int objName = 0;
	switch(sdle.button.button)
	{
	case SDL_BUTTON_LEFT:
		//if(logic.bShowOptions){
			//logic.bShowOptions = false;
			//logic.bGamePaused = false;
		//}
		//{
			// Normalize screen coordinates.
			// OpenGL coords (used for rendering) go from -1 to 1 with the origin in the middle.
			// While clicking coords have (0, 0) in the upper left corner.
			//float xnorm = (sdle.button.x - 0.5*flWidth) / (0.5*flWidth);
			//float ynorm = (0.5*flHeight - sdle.button.y) / (0.5*flHeight);
			
			//for(std::size_t i = 0; i < guiObjects.size(); i++)
				//guiObjects[i]->handleMouseButtonDown(logic, xnorm, ynorm);
		//}
		//break;

	case SDL_BUTTON_WHEELUP:
		break;

	case SDL_BUTTON_WHEELDOWN:
		break;
	}// end switch
}

void OptionsScreen::handleMouseButtonUp(const SDL_Event& sdle, Logic &logic)
{
	switch(sdle.button.button)
	{
	case SDL_BUTTON_LEFT:
		{
			// Normalize screen coordinates.
			// OpenGL coords (used for rendering) go from -1 to 1 with the origin in the middle.
			// While clicking coords have (0, 0) in the upper left corner.
			float xnorm = (sdle.button.x - 0.5*flWidth) / (0.5*flWidth);
			float ynorm = (0.5*flHeight - sdle.button.y) / (0.5*flHeight);
			
			for(std::size_t i = 0; i < guiObjects.size(); i++){
				guiObjects[i]->handleMouseButtonUp(logic, xnorm, ynorm);
			}
		}
		break;
	}// end switch
}

std::vector<std::tr1::shared_ptr<GuiObject> > & OptionsScreen::getGuiObjects()
{
	return guiObjects;
}

/*________________________________*/
// PlayScreen implementation.
PlayScreen::PlayScreen(float w, float h, SDL_Surface* s, TEXTURE_PTR_ARRAY t,
	 FMOD::System *sys,std::vector<FMOD::Sound*> snd):
	SdlScreen(w, h, s), textures(t), system(sys), sounds(snd)
{
	initMembers();
	addShapes();
}

PlayScreen::~PlayScreen()
{
}

void PlayScreen::initMembers()
{
	flBoxWidth = 1.3f;		// Add these to the constructor and use when init walls.
	flBoxHeight = 1.0f;
	flBoxThickness = 0.5f;
	xViewOld = xView = 0.;
	angleViewY = 115.;		// Initial angle (around y) of the view.
	angleViewZ = 0.;
	flScaleAll = 1.;
	bPaddlePicked = false;
	xPaddleOld = yPaddleOld = 0.;

	flZaxisDistance = 2.2f;
	flLengthUnit = 0.4f;

	flScreenWidth = flWidth;
	flScreenHeight = flHeight;
}

void PlayScreen::addShapes()
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

	// !Reconsider this code: add some method to avoid code duplication.

	// Add walls.
	// Left wall.
	center = vector_3d(-0.5*flBoxWidth, 0., 0.);
	vector_3d n(-1., 0., 0.);
	Wall* pWall = new AbsorbingWall( WALL, center, flBoxThickness, flBoxHeight, n);
	pShape = dynamic_cast<Shape*>(pWall);
	pShape->setSound(system, sounds[2]);
	shapes[1] = std::tr1::shared_ptr<Shape>(pShape);

	// Right wall.
	center = vector_3d(0.5*flBoxWidth, 0., 0.);
	n = vector_3d(1., 0., 0.);
	pWall = new Wall( WALL, center, flBoxThickness, flBoxHeight, n);
	pShape = dynamic_cast<Shape*>(pWall);
	pShape->setSound(system, sounds[1]);
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
	pShape->setSound(system, sounds[1]);
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
	pShape->setSound(system, sounds[1]);
	shapes[4] = std::tr1::shared_ptr<Shape>(pShape);

	// Front wall.
	center = vector_3d(0., 0., 0.5*flBoxThickness);
	n = vector_3d(0., 0., 1.);
	pWall = new Wall( WALL, center, flBoxWidth, flBoxHeight, n);
	pShape = dynamic_cast<Shape*>(pWall);
	pShape->setSound(system, sounds[1]);
	shapes[5] = std::tr1::shared_ptr<Shape>(pShape);

	// Back wall.
	center = vector_3d(0., 0., -0.5*flBoxThickness);
	n = vector_3d(0., 0., -1.);
	pWall = new Wall( WALL, center, flBoxWidth, flBoxHeight, n);
	pShape = dynamic_cast<Shape*>(pWall);
	pShape->setSound(system, sounds[1]);
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
	pShape->setSound(system, sounds[3]);
	shapes[7] = std::tr1::shared_ptr<Shape>(pShape);
	leftPaddleIdx = 7;
}

void PlayScreen::doInput(Logic &logic, SDL_Event sdlEvent)
{
	//SDL_PollEvent(&sdlEvent); // check input
	        
    switch(sdlEvent.type)
    {
	case SDL_MOUSEBUTTONDOWN:
		if(!logic.bGamePaused)
			handleMouseButtonDown(sdlEvent, logic);
		break;

	case SDL_MOUSEBUTTONUP:
		if(!logic.bGamePaused)
			handleMouseButtonUp(sdlEvent, logic);
		break;

	case SDL_MOUSEMOTION:
		if(!logic.bGamePaused)
			handleMouseMotion(sdlEvent, logic);
		break;
	
	case SDL_KEYDOWN:
		handleKeyDown(sdlEvent, logic);
		break;

	case SDL_KEYUP:
		handleKeyUp(sdlEvent, logic);
		break;

	case SDL_VIDEORESIZE:
		handleResize(sdlEvent, logic);
		break;			

	case SDL_QUIT:
		logic.bAppRunning = false;
		break;
	}
}

void PlayScreen::doDrawing(Logic &logic)
{
	// Here working and understood code starts.
	initResize();
	initView();
	
	// Draw all the shapes.
	for(std::size_t i = 0; i < shapes.size(); i++){
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

void PlayScreen::handleMouseButtonDown(const SDL_Event& sdle, Logic &logic)
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
		break;

	case SDL_BUTTON_WHEELUP:
		angleViewZ -= 0.3;			// rotate around the horizontal axis.
		break;

	case SDL_BUTTON_WHEELDOWN:
		angleViewZ += 0.3;
		break;
	}// end switch
}

void PlayScreen::handleMouseButtonUp(const SDL_Event& sdle, Logic &logic)
{
	switch(sdle.button.button)
	{
	case SDL_BUTTON_LEFT:
		bPaddlePicked = false;
		break;
	}
}

void PlayScreen::handleMouseMotion(const SDL_Event& sdle, Logic &logic)
{
	switch(sdle.motion.state)
	{
	case SDL_BUTTON_LEFT:
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

void PlayScreen::handleKeyDown(const SDL_Event& sdle, Logic &logic)
{	
	// Check keyboard.
 	switch(sdle.key.keysym.sym)
	{
	case SDLK_EQUALS:		// Reset view.
		angleViewY = 115;
		angleViewZ = 0.;
		flScaleAll = 1.;
		break;

	case SDLK_b:
		if(!logic.bShowOptions)
		{
			logic.bShowOptions = true;
			logic.bGamePaused = true;
		}
		logic.notifyObservers();	// Tell observers to change their sound behavior.
		break;

	case SDLK_p:	 //Scale.
		flScaleAll += 0.01;
		break;

	case SDLK_m:
		flScaleAll -= 0.01;
		//logic.bGamePaused = !logic.bGamePaused;
		break;

	case SDLK_v:
		logic.bGamePaused = true;
		break;

	case SDLK_SPACE:
		logic.bGamePaused = false;
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

void PlayScreen::handleKeyUp(const SDL_Event& sdle, Logic &logic)
{
	switch(sdle.key.keysym.sym)
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

void PlayScreen::handleResize(const SDL_Event& sdle, Logic &logic)
{
	SDL_SetVideoMode( sdlEvent.resize.w, sdlEvent.resize.h, 32, SDL_OPENGL | SDL_HWSURFACE | SDL_RESIZABLE );
	glViewport (0, 0, (GLsizei) sdle.resize.w, (GLsizei) sdle.resize.h);
	flScreenWidth = sdle.resize.w;
	flScreenHeight = sdle.resize.h;

	initResize();		// Avoid code duplication with setup matrices.
}

// Object selection using back buffer.
int PlayScreen::pickObject(int x, int y)
{
	// Render the scene into the back buffer with colors corresponding to
	// names of the objects.
	initView();		// Avoid code duplication with doDrawing().

	glDisable(GL_LIGHTING);
	// Draw all the shapes.
	for(std::size_t i = 0; i < shapes.size(); i++)
	{
		int name = shapes[i]->getId();
			
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

void PlayScreen::initResize()
{
	glMatrixMode (GL_PROJECTION);	// Switch to the projection matrix.
	glLoadIdentity ();				// Clean up the projection matrix.
	gluPerspective(calculateAngle(flLengthUnit, flZaxisDistance), 
		flScreenWidth/flScreenHeight, 1.0, 1024.0);	// Set up the projection matrix with the same units in x and y.
	glMatrixMode (GL_MODELVIEW);	// Switch to the modelview matrix.
	glLoadIdentity ();
}

void PlayScreen::initView()
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

void PlayScreen::drawAxes()
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

void PlayScreen::doLogic(const Logic &logic)
{
	// Make sure input isn't affected by the timer.
    //doInput();    
    //updateTimers();

	double deltaTime = 0.;	// Is not used.
    
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

void PlayScreen::play(Logic &logic, SDL_Event sdlEvent)
{
	doInput(logic, sdlEvent);
	
	if(!logic.bGamePaused)
		doLogic(logic);
	
	doDrawing(logic);	
}

std::vector<std::tr1::shared_ptr<Shape> > & PlayScreen::getShapes()
{
	return shapes;
}