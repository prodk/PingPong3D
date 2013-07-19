// GuiObject.cpp - implementation of the GuiObject class and all its child classes.
// (c) Nikolay Prodanov, Juelich, summer 2013.
#include "GuiObject.h"

/*________________________________*/
// GuiObject class implementation.
GuiObject::GuiObject(std::size_t idExt) :
id(idExt), bPlaySound(true)
{
}

GuiObject::~GuiObject(void)
{
}


std::size_t GuiObject::getId()
{
	return id;
}

FMOD_RESULT GuiObject::setSound(FMOD::System *sys, FMOD::Sound *snd)
{
	if(sys == NULL) return FMOD_ERR_FILE_BAD;
	if(snd == NULL) return FMOD_ERR_FILE_BAD;
	system = sys;
	sound = snd;

	// Start paused sound.
	//FMOD_RESULT result = system->playSound(sound, 0, true, &channel);

	return FMOD_OK;
}

void GuiObject::notify(Subject* s) 
{
	bPlaySound = ((Logic*) s)->bActionsSound;
}

//void GuiObject::playSound()
//{
//	if(bPlaySound)
//		channel->setPaused(false);
//}

/*________________________________*/
//Button class implementation.
Button::Button(float xExt, float yExt, float wExt, float hExt, 
	std::size_t idExt, std::string n, int tid) :
GuiObject(idExt), x(xExt), y(yExt), w(wExt), h(hExt), name(n), textureId(tid)
{
	caption = name;	
}

Button::~Button()
{
}

void Button::draw(TTF_Font *font, Logic &logic)
{		
	glPushMatrix();
	glDisable(GL_LIGHTING);

	glBindTexture( GL_TEXTURE_2D, textureId );
	
	glColor4f(1.0, 0., 0., 1.0);
	glTranslatef(x, y, 0.);

	glBegin( GL_QUADS );
	glTexCoord3d(0.0, 1.0, 0.0); glVertex3d(0.0, 0.0, 0.0);
	glTexCoord3d(1.0, 1.0, 0.0); glVertex3d(w, 0.0, 0.0);
	glTexCoord3d(1.0, 0.0, 0.0); glVertex3d(w, h, 0.0);
	glTexCoord3d(0.0, 0.0, 0.0); glVertex3d(0.0, h, 0.0);	
	glEnd();	

	glEnable(GL_LIGHTING);
	glPopMatrix();
	
	// Print button's text.
	drawText(font, logic);
}

int Button::drawText(TTF_Font *font, Logic &logic)
{
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear buffer for new data.
	int mode;

	SDL_Color textColor;
	SDL_Surface *text;
	GLuint textTexture;

	textColor.r = 1;
    textColor.g = 0;
    textColor.b = 0;
	
	text = TTF_RenderText_Blended(font, caption.c_str(), textColor);
	SDL_Delay(25);			// Small delay to prevent from full CPU load.

	if (text->format->BytesPerPixel == 3) // RGB 24bit.
    { 
        mode = GL_RGB;
    } 
    else if (text->format->BytesPerPixel == 4) // RGBA 32bit.
    { 
        mode = GL_RGBA;
    } 
    else 
    {
        printf("Could not determine pixel format of the surface");
        SDL_FreeSurface(text);
        return NULL;
    }

	// Draw the surface onto the OGL screen using gextures.
    glPushMatrix();
	glDisable(GL_LIGHTING);

    //glEnable(GL_BLEND);
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(0.0f, 1.0f, 1.0f, 1.0f);

    glGenTextures(1, &textTexture);
    glBindTexture(GL_TEXTURE_2D, textTexture);

	// Create texture bound to our text surface.
    glTexImage2D(GL_TEXTURE_2D, 0, 4, text->w, text->h, 0, mode, GL_UNSIGNED_BYTE, text->pixels );

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
    glColor3f(0.0f, 1.0f, 1.0f);

    glTranslatef(x, y, 0.0f);

	// Draw.
    glBegin( GL_QUADS );
	glTexCoord3d(0.0, 1.0, 0.0); glVertex3d(0.0, 0.0, 0.0);
	glTexCoord3d(1.0, 1.0, 0.0); glVertex3d(w, 0.0, 0.0);
	glTexCoord3d(1.0, 0.0, 0.0); glVertex3d(w, h, 0.0);
	glTexCoord3d(0.0, 0.0, 0.0); glVertex3d(0.0, h, 0.0);	
	glEnd();
	
	glEnable(GL_LIGHTING);

	glPopMatrix();

    SDL_FreeSurface(text);
    glDeleteTextures(1, &textTexture);

	return 0;
}

void Button::handleMouseButtonUp(Logic &logic, float x, float y)
{
	if( ptInRect(x, y) )
	{		
		switch(id)
		{ 
		case START_BUTTON:
			logic.bShowOptions = false;
			logic.bGamePaused = false;
			// Tell registered observers to change their settings.
			logic.notifyObservers();
			break;

		case OPTIONS_BUTTON:
			logic.notifyObservers();
			break;

		case BACKGR_SOUND_BUTTON:
			if(logic.bBackgroundSound){
				logic.bBackgroundSound = false;
				caption = name + "Off";
			}
			else{
				logic.bBackgroundSound = true;
				caption = name;
			}
			// Tell registered observers to change their settings.
			logic.notifyObservers();
			break;

		case ACT_SOUND_BUTTON:
			if(logic.bActionsSound){
				logic.bActionsSound = false;
				caption = name + "Off";
			}
			else{
				logic.bActionsSound = true;
				caption = name;
			}
			// Tell registered observers to change their settings.
			logic.notifyObservers();
			break;
		}// end switch(id)

		if(bPlaySound)
			::playSound(system, sound, 0);		// Maybe use channels for managing sounds!
		//playSound();
	}// End if ptInRect.
}

void Button::handleMouseButtonDown(Logic &logic, float x, float y)
{
	if( ptInRect(x, y) )
	{		
	}// End if ptInRect.
}

void Button::handleKeyDown(Logic &logic)
{
	
}

bool Button::ptInRect(float ptx, float pty)
{
	float xMin = 0.;
	float xMax = xMin + w;
	float yMin = 0.;
	float yMax = yMin + h;

	if( (ptx >= x) && (ptx <= x + w) && (pty >= y ) && (pty <= y+h) )
		return true;

	return false;
}