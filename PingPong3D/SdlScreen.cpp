#include "SdlScreen.h"


SdlScreen::SdlScreen(float w, float h) :
flWidth(w), flHeight(h)
{
	//surface = NULL;
}


SdlScreen::~SdlScreen(void)
{
	//if(surface != NULL){
		//SDL_FreeSurface(surface);
		//surface = NULL;
	//}
}

OptionsScreen::OptionsScreen(float w, float h, SDL_Surface* s, TEXTURE* t):
	SdlScreen(w, h), surfaceOnWhichWeRender(s), textures(t)
{
	//textures = NULL;
	// Reset previous SDL screen.
	//SDL_Quit();
	//Initialize all SDL subsystems
	//int res = SDL_Init( SDL_INIT_EVERYTHING );
 //   /*if( SDL_Init( SDL_INIT_EVERYTHING ) == -1 )
 //   {
 //       return false;
 //   }*/
	//const SDL_VideoInfo* info = SDL_GetVideoInfo( );
	//if( !info ) {}	// Add later.
	//int bpp = info->vfmt->BitsPerPixel;

	//surfaceOnWhichWeRender = NULL;

 //   //Set up the screen
 //   surfaceOnWhichWeRender = SDL_SetVideoMode(flWidth, flHeight, bpp, SDL_SWSURFACE | 
	//	SDL_RESIZABLE );

 //   //If there was an error in setting up the screen
 //   /*if( screen == NULL )
 //   {
 //       return false;
 //   }*/

 //   //Set the window caption
 //   SDL_WM_SetCaption( "PingPong3D - Options", NULL );

    //If everything initialized fine
    //return true;
}

OptionsScreen::~OptionsScreen()
{
	//SDL_FreeSurface(surfaceOnWhichWeRender);
	//SDL_Quit();
}

void OptionsScreen::show()
{	
	

	glPushMatrix();
	glFlush();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear buffer for new data.

	glMatrixMode (GL_PROJECTION);	// Switch to the projection matrix.
	glLoadIdentity ();				// Clean up the projection matrix.

	glMatrixMode (GL_MODELVIEW);	// Switch to the modelview matrix.
	glLoadIdentity ();	

    // select modulate to mix texture with color for shading
    glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );

    // when texture area is small, bilinear filter the closest mipmap
    //glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                     //GL_LINEAR_MIPMAP_NEAREST );
    // when texture area is large, bilinear filter the first mipmap
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );   

	glEnable( GL_TEXTURE_2D );
	glBindTexture( GL_TEXTURE_2D, textures->id );

	glColor4f(1.0, 0., 0., 1.0);
	//gluLookAt (0.0, 0.0, 0., 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	//glTranslatef(0, 0., 0.);

	glBegin( GL_QUADS );
	glTexCoord3d(0.0, 1.0, 0.0); glVertex3d(0.0, 0.0, 0.0);
	glTexCoord3d(1.0, 1.0, 0.0); glVertex3d(1.0, 0.0, 0.0);
	glTexCoord3d(1.0, 0.0, 0.0); glVertex3d(1.0, 1.0, 0.0);
	glTexCoord3d(0.0, 0.0, 0.0); glVertex3d(0.0, 1.0, 0.0);	
	glEnd();

	glFlush();

    glPopMatrix();
}