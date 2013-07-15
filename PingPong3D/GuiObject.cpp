#include "GuiObject.h"

/*________________________________*/
// GuiObject class implementation.
GuiObject::GuiObject(std::size_t idExt) :
id(idExt)
{
}

GuiObject::~GuiObject(void)
{
}


std::size_t GuiObject::getId()
{
	return id;
}

/*________________________________*/
//Button class implementation.
Button::Button(float xExt, float yExt, float wExt, float hExt, std::size_t idExt) :
GuiObject(idExt), x(xExt), y(yExt), w(wExt), h(hExt)
{
}

void Button::draw()
{
	/*
	 glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );

    // when texture area is small, bilinear filter the closest mipmap
    //glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                     //GL_LINEAR_MIPMAP_NEAREST );
    // when texture area is large, bilinear filter the first mipmap
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );   

	glEnable( GL_TEXTURE_2D );
	*/

	glPushMatrix();
	glBindTexture( GL_TEXTURE_2D, id );
	
	glColor4f(1.0, 0., 0., 1.0);
	//gluLookAt (0.0, 0.0, 0., 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	glTranslatef(x, y, 0.);

	glBegin( GL_QUADS );
	glTexCoord3d(0.0, 1.0, 0.0); glVertex3d(0.0, 0.0, 0.0);
	glTexCoord3d(1.0, 1.0, 0.0); glVertex3d(1.0, 0.0, 0.0);
	glTexCoord3d(1.0, 0.0, 0.0); glVertex3d(1.0, 0.5, 0.0);
	glTexCoord3d(0.0, 0.0, 0.0); glVertex3d(0.0, 0.5, 0.0);	
	glEnd();
	glPopMatrix();
}