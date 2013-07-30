// GuiObject.cpp - implementation of the GuiObject class and all its child classes.
// (c) Nikolay Prodanov, summer 2013, Juelich, Germany.
#include "GuiObject.h"

/*________________________________*/
// GuiObject class implementation.
GuiObject::GuiObject(float xExt, float yExt, float wExt, float hExt,
	std::size_t idExt, std::string n) :
	x(xExt), y(yExt), w(wExt), h(hExt), id(idExt), name(n)
{
	bIsPressed = false;
	bHasFocus = false;
	wPressed = 0.98*w;
	hPressed = 0.98*h;
	xPressed = x + 0.5*(w - wPressed);
	yPressed = y - 0.5*(h - hPressed);
	txtW = 0.06*w*n.length();
	txtX = x + 0.5*(w - txtW);
}

GuiObject::~GuiObject(void)
{
}

int GuiObject::getId() const
{
	return id;
}

int GuiObject::drawText(const std::string &txt, GLfloat x, GLfloat y, GLfloat w, GLfloat h,
	TTF_Font *font)
{
	int mode;

	SDL_Color textColor;
	SDL_Surface *text;
	GLuint textTexture;

	textColor.r = 1;
    textColor.g = 0;
    textColor.b = 0;
	
	text = TTF_RenderText_Blended(font, txt.c_str(), textColor);
	SDL_Delay(4);			// Small delay to prevent from full CPU load.

	if (text->format->BytesPerPixel == 3) {// RGB 24bit.
		mode = GL_RGB;
    } 
    else if (text->format->BytesPerPixel == 4) { // RGBA 32bit.
		mode = GL_RGBA;
    } 
    else {
		std::cerr << "Could not determine pixel format of the surface in GuiObject!" << std::endl;
        SDL_FreeSurface(text);
        exit(1);
    }

	// Draw the surface onto the OGL screen using textures.
    glPushMatrix();
	glDisable(GL_LIGHTING);

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

bool GuiObject::isPressed(float ptx, float pty)
{
	if( ptInRect(ptx, pty) )
		return true;
	
	return false;
}

bool GuiObject::isPressed()
{
	if(bIsPressed)
		return true;
	
	return false;
}

void GuiObject::setPressed(bool pressed)
{
	bIsPressed = pressed;
}

void GuiObject::setFocus(bool focus)
{
	bHasFocus = focus;
}

bool GuiObject::hasFocus()
{
	return bHasFocus;
}

bool GuiObject::ptInRect(float ptx, float pty)
{
	float xMin = 0.;
	float xMax = xMin + w;
	float yMin = 0.;
	float yMax = yMin + h;

	if( (ptx >= x) && (ptx <= x + w) && (pty >= y ) && (pty <= y+h) )
		return true;

	return false;
}

void GuiObject::draw(TTF_Font *font)
{
	if(bIsPressed)
		drawPressed(font);
	else
		drawUnpressed(font);
}

/*________________________________*/
//Button class implementation.
Button::Button(float xExt, float yExt, float wExt, float hExt, 
	std::size_t idExt, std::string n, int tid) :
GuiObject(xExt, yExt, wExt, hExt, idExt, n), textureId(tid)
{
}

Button::~Button()
{
}

void Button::drawPressed(TTF_Font *font)
{		
	glPushMatrix();
	glDisable(GL_LIGHTING);
	// Select texture
	glBindTexture( GL_TEXTURE_2D, textureId );	
	
	if(bHasFocus)
		glColor4f(0.0, 1.0, 0.0, 1.0);		// Green color for the button under cursor.
	else
		glColor4f(1.0, 1.0, 1.0, 1.0);		// White color for buttons not under cursor.
	glTranslatef(xPressed, yPressed, 0.);

	glBegin( GL_QUADS );
	glTexCoord3d(0.0, 1.0, 0.0); glVertex3d(0.0, 0.0, 0.0);
	glTexCoord3d(1.0, 1.0, 0.0); glVertex3d(wPressed, 0.0, 0.0);
	glTexCoord3d(1.0, 0.0, 0.0); glVertex3d(wPressed, hPressed, 0.0);
	glTexCoord3d(0.0, 0.0, 0.0); glVertex3d(0.0, hPressed, 0.0);	
	glEnd();	

	glEnable(GL_LIGHTING);
	glPopMatrix();
	
	// Print button's text.
	SDL_Delay(4);				// Additional delay because we draw only one text.
	drawText(name, xPressed, yPressed, wPressed, hPressed, font);
}

void Button::drawUnpressed(TTF_Font *font)
{		
	glPushMatrix();
	glDisable(GL_LIGHTING);
	// Select texture.
	glBindTexture( GL_TEXTURE_2D, textureId );

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	
	if(bHasFocus)
		glColor4f(0.0, 1.0, 0.0, 1.0);
	else
		glColor4f(1.0, 1.0, 1.0, 1.0);
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
	SDL_Delay(4);				// Additional delay because we draw only one text.

	drawText(name, txtX, y, txtW, h, font);
}

/*________________________________*/
// OptionsButton class implementation.
OptionsButton::OptionsButton(float xExt, float yExt, float wExt, float hExt, 
	std::size_t idExt, std::string n, int tid, std::string cap) :
Button(xExt, yExt, wExt, hExt, idExt, n, tid), caption(cap)
{
	xSmall = x + 1.7*w;
	wSmall = 0.25*w;
	xSmallPressed = x - xPressed + xSmall;
}

OptionsButton::~OptionsButton()
{
}

void OptionsButton::drawPressed(TTF_Font *font)
{	
	glPushMatrix();
	glDisable(GL_LIGHTING);

	glBindTexture( GL_TEXTURE_2D, textureId );
	
	if(bHasFocus)
		glColor4f(0.0, 1.0, 0.0, 1.0);
	else
		glColor4f(1.0, 1.0, 1.0, 1.0);
	glTranslatef(xPressed, yPressed, 0.);

	// Draw larger button.
	glBegin( GL_QUADS );
	glTexCoord3d(0.0, 1.0, 0.0); glVertex3d(0.0, 0.0, 0.0);
	glTexCoord3d(1.0, 1.0, 0.0); glVertex3d(wPressed, 0.0, 0.0);
	glTexCoord3d(1.0, 0.0, 0.0); glVertex3d(wPressed, hPressed, 0.0);
	glTexCoord3d(0.0, 0.0, 0.0); glVertex3d(0.0, hPressed, 0.0);	
	glEnd();	

	glTranslatef(xSmallPressed, y - yPressed, 0.);
	// Draw smaller button.
	glBegin( GL_QUADS );
	glTexCoord3d(0.0, 1.0, 0.0); glVertex3d(0.0, 0.0, 0.0);
	glTexCoord3d(1.0, 1.0, 0.0); glVertex3d(wSmall, 0.0, 0.0);
	glTexCoord3d(1.0, 0.0, 0.0); glVertex3d(wSmall, h, 0.0);
	glTexCoord3d(0.0, 0.0, 0.0); glVertex3d(0.0, h, 0.0);	
	glEnd();

	glEnable(GL_LIGHTING);
	glPopMatrix();

	drawText(name, xPressed, yPressed, wPressed, hPressed, font);	// Button's name.
	drawText(caption, x+xSmall, y, wSmall, h, font);				// Parameter's value.
}

void OptionsButton::drawUnpressed(TTF_Font *font)
{		
	glPushMatrix();
	glDisable(GL_LIGHTING);

	glBindTexture( GL_TEXTURE_2D, textureId );
	
	if(bHasFocus)
		glColor4f(0.0, 1.0, 0.0, 1.0);
	else
		glColor4f(1.0, 1.0, 1.0, 1.0);
	glTranslatef(x, y, 0.);

	// Draw larger button.
	glBegin( GL_QUADS );
	glTexCoord3d(0.0, 1.0, 0.0); glVertex3d(0.0, 0.0, 0.0);
	glTexCoord3d(1.0, 1.0, 0.0); glVertex3d(w, 0.0, 0.0);
	glTexCoord3d(1.0, 0.0, 0.0); glVertex3d(w, h, 0.0);
	glTexCoord3d(0.0, 0.0, 0.0); glVertex3d(0.0, h, 0.0);	
	glEnd();	

	glTranslatef(xSmall, 0., 0.);
	// Draw smaller button.
	glBegin( GL_QUADS );
	glTexCoord3d(0.0, 1.0, 0.0); glVertex3d(0.0, 0.0, 0.0);
	glTexCoord3d(1.0, 1.0, 0.0); glVertex3d(wSmall, 0.0, 0.0);
	glTexCoord3d(1.0, 0.0, 0.0); glVertex3d(wSmall, h, 0.0);
	glTexCoord3d(0.0, 0.0, 0.0); glVertex3d(0.0, h, 0.0);	
	glEnd();

	glEnable(GL_LIGHTING);
	glPopMatrix();

	//drawText(name, x, y, w, h, font);					// Button's name.
	drawText(name, txtX, y, txtW, h, font);
	drawText(caption, x+xSmall, y, wSmall, h, font);	// Parameter's value.
}

void OptionsButton::setCaption(const std::string & c)
{
	caption = c;
}