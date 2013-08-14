/*
 The contents of this file are subject to the Mozilla Public License				
 Version 1.1 (the "License"); you may not use this file except in					 
 compliance with the License. You may obtain a copy of the License at			 
 http://www.mozilla.org/MPL/																								
																																						
 Software distributed under the License is distributed on an "AS IS"				
 basis, WITHOUT WARRANTY OF ANY KIND, either express or implied. See the		
 License for the specific language governing rights and limitations				 
 under the License.																												 
																																						
 Alternatively, the contents of this file may be used under the terms			 
 of the GNU Lesser General Public license (the	"LGPL License"), in which case the	
 provisions of LGPL License are applicable instead of those									
 above.																																		 
																																						
 For feedback and questions about my Files and Projects please mail me,		 
 Alexander Matthes (Ziz) , zizsdl_at_googlemail.com												 
*/

#include <sparrow3d.h>

#define FONT_LOCATION "./font/Play-Bold.ttf"
#define FONT_SIZE 15

SDL_Surface* screen;
spFontPointer font = NULL;
SDL_Surface* banner;


void draw( void )
{
	spClearTarget( 0 );
	spRotozoomSurface( screen->w/2, spFixedToInt(banner->h*spGetSizeFactor()/3)/2+2, 0, banner, spGetSizeFactor()/3, spGetSizeFactor()/3,0);
	spFontDrawRight( screen->w-2, screen->h-font->maxheight-2, 0, "[S] Exit", font );
	spFlip();
}

int calc(Uint32 steps)
{
	if ( spGetInput()->button[SP_BUTTON_START] )
		return 1;
	return 0;
}

void resize(Uint16 w,Uint16 h)
{
  //Setup of the new/resized window
  spSelectRenderTarget(spGetWindowSurface());
  spSetPerspective(50.0,(float)spGetWindowSurface()->w/(float)spGetWindowSurface()->h,1.0,100);
	spFontShadeButtons(1);

	//Font Loading
	spFontSetShadeColor(65535);
	if (font)
		spFontDelete(font);
	font = spFontLoad(FONT_LOCATION,FONT_SIZE*spGetSizeFactor()>>SP_ACCURACY);
	spFontAdd(font,SP_FONT_GROUP_ASCII,0);//whole ASCII
	spFontAddBorder(font,65535);
	spFontMulWidth(font,15<<SP_ACCURACY-4);
	spFontAddButton( font, 'A', SP_BUTTON_LEFT_NAME, spGetRGB(230,230,230), spGetRGB(64,64,64));
	spFontAddButton( font, 'B', SP_BUTTON_RIGHT_NAME, spGetRGB(230,230,230), spGetRGB(64,64,64));
	spFontAddButton( font, 'X', SP_BUTTON_DOWN_NAME, spGetRGB(230,230,230), spGetRGB(64,64,64));
	spFontAddButton( font, 'Y', SP_BUTTON_UP_NAME, spGetRGB(230,230,230), spGetRGB(64,64,64));
	spFontAddButton( font, 'L', SP_BUTTON_L_NAME, spGetRGB(230,230,230), spGetRGB(64,64,64));
	spFontAddButton( font, 'R', SP_BUTTON_R_NAME, spGetRGB(230,230,230), spGetRGB(64,64,64));
	spFontAddButton( font, 'S', SP_BUTTON_START_NAME, spGetRGB(230,230,230), spGetRGB(64,64,64));
	spFontAddButton( font, 'E', SP_BUTTON_SELECT_NAME, spGetRGB(230,230,230), spGetRGB(64,64,64));
}

int main(int argc, char **argv)
{
	spInitCore();
	spSetAffineTextureHack(0); //We don't need it :)
	spInitMath();
	screen = spCreateDefaultWindow();
	resize(screen->w,screen->h);
	
	spSetZSet(0);
	spSetZTest(0);
	banner = spLoadSurface("images/banner.png");

	spLoop( draw, calc, 10, resize, NULL );
	
	spDeleteSurface(banner);

	spFontDelete(font);
	spQuitCore();
	return 0;
}
