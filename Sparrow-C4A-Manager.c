 /* This file is part of Sparrow-C4A-Manager.
  * Sparrow3d is free software: you can redistribute it and/or modify
  * it under the terms of the GNU General Public License as published by
  * the Free Software Foundation, either version 2 of the License, or
  * (at your option) any later version.
  * 
  * Sparrow3d is distributed in the hope that it will be useful,
  * but WITHOUT ANY WARRANTY; without even the implied warranty of
  * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  * GNU General Public License for more details.
  * 
  * You should have received a copy of the GNU General Public License
  * along with Foobar.  If not, see <http://www.gnu.org/licenses/>
  * 
  * For feedback and questions about my Files and Projects please mail me,
  * Alexander Matthes (Ziz) , zizsdl_at_googlemail.com */

#include <sparrow3d.h>
#include "defines.h"
#include "menu.h"
#include "account.h"

SDL_Surface* screen;
spFontPointer font = NULL;
spFontPointer font_small = NULL;
spFontPointer font_very_small = NULL;
SDL_Surface* banner;

typedef enum {
	menu = 0,
	account = 1,
	highscore = 2
} stateEnum;

stateEnum state = menu;

void draw( void )
{
	spClearTarget( BACKGROUND_COLOR );
	switch (state)
	{
		case menu:
			spRotozoomSurface( screen->w/6, 2*screen->h/36+spFixedToInt(banner->h*spGetSizeFactor()/4)/2+2, 0, banner, spGetSizeFactor()/4, spGetSizeFactor()/4,0);
			draw_menu(font,font_small,font_very_small);
			break;
		case account:
			spRotozoomSurface( screen->w/6, 2*screen->h/36+spFixedToInt(banner->h*spGetSizeFactor()/4)/2+2, 0, banner, spGetSizeFactor()/4, spGetSizeFactor()/4,0);
			draw_account(font,font_small,font_very_small);
			break;
		case highscore:
			draw_highscore(font,font_small,font_very_small);
			break;
	}
	spFlip();
}

int calc(Uint32 steps)
{
	switch (state)
	{
		case menu:
			switch (calc_menu(steps))
			{
				case 1:
					state = account;
					start_account();
					break;
				case 2:
					state = highscore;
					start_highscore();
					break;
				case -1:
					return 1;
			}
			break;
		case account:
			if (calc_account(steps) != 0)
			{
				finish_account();
				state = menu;
			}
			break;
		case highscore:
			if (calc_highscore(steps) != 0)
			{
				finish_highscore();
				state = menu;
			}
			break;
	}
	return 0;
}

void resize(Uint16 w,Uint16 h)
{
	//Settings up the onboard keyboard:
	if (spGetSizeFactor() <= SP_ONE)
		spSetVirtualKeyboard(SP_VIRTUAL_KEYBOARD_IF_NEEDED,0,h-w*48/320,w,w*48/320,spLoadSurface("./images/keyboard320.png"),spLoadSurface("./images/keyboardShift320.png"));
	else
	if (spGetSizeFactor() <= 2*SP_ONE)
		spSetVirtualKeyboard(SP_VIRTUAL_KEYBOARD_IF_NEEDED,0,h-w*48/320,w,w*48/320,spLoadSurface("./images/keyboard640.png"),spLoadSurface("./images/keyboardShift640.png"));
	else
		spSetVirtualKeyboard(SP_VIRTUAL_KEYBOARD_IF_NEEDED,0,h-w*48/320,w,w*48/320,spLoadSurface("./images/keyboard1280.png"),spLoadSurface("./images/keyboardShift1280.png"));
  //Setup of the new/resized window
  spSelectRenderTarget(spGetWindowSurface());
	spFontShadeButtons(1);

	//Font Loading
	spFontSetShadeColor(BACKGROUND_COLOR);
	if (font)
		spFontDelete(font);
	font = spFontLoad(FONT_LOCATION,FONT_SIZE*spGetSizeFactor()>>SP_ACCURACY);
	spFontAdd(font,SP_FONT_GROUP_ASCII,65535);//whole ASCII
	spFontAddBorder(font,BACKGROUND_COLOR);
	spFontMulWidth(font,15<<SP_ACCURACY-4);
	spFontAddButton( font, 'B', SP_PRACTICE_OK_NOWASD_NAME, spGetRGB(230,230,230), spGetRGB(64,64,64));
	spFontAddButton( font, 'X', SP_PRACTICE_CANCEL_NOWASD_NAME, spGetRGB(230,230,230), spGetRGB(64,64,64));
	spFontAddButton( font, 'L', SP_BUTTON_L_NOWASD_NAME, spGetRGB(230,230,230), spGetRGB(64,64,64));
	spFontAddButton( font, 'R', SP_BUTTON_R_NOWASD_NAME, spGetRGB(230,230,230), spGetRGB(64,64,64));
	spFontAddButton( font, 'S', SP_BUTTON_START_NOWASD_NAME, spGetRGB(230,230,230), spGetRGB(64,64,64));
	spFontAddButton( font, 'E', SP_BUTTON_SELECT_NOWASD_NAME, spGetRGB(230,230,230), spGetRGB(64,64,64));

	spFontSetShadeColor(BACKGROUND_COLOR);
	if (font_small)
		spFontDelete(font_small);
	font_small = spFontLoad(FONT_LOCATION,FONT_SIZE_SMALL*spGetSizeFactor()>>SP_ACCURACY);
	spFontAdd(font_small,SP_FONT_GROUP_ASCII,spGetRGB(192,192,192));//whole ASCII
	spFontAddBorder(font_small,BACKGROUND_COLOR);
	spFontMulWidth(font_small,15<<SP_ACCURACY-4);
	spFontAddButton( font_small, 'B', SP_PRACTICE_OK_NOWASD_NAME, spGetRGB(230,230,230), spGetRGB(64,64,64));
	spFontAddButton( font_small, 'X', SP_PRACTICE_CANCEL_NOWASD_NAME, spGetRGB(230,230,230), spGetRGB(64,64,64));
	spFontAddButton( font_small, 'L', SP_BUTTON_L_NOWASD_NAME, spGetRGB(230,230,230), spGetRGB(64,64,64));
	spFontAddButton( font_small, 'R', SP_BUTTON_R_NOWASD_NAME, spGetRGB(230,230,230), spGetRGB(64,64,64));
	spFontAddButton( font_small, 'S', SP_BUTTON_START_NOWASD_NAME, spGetRGB(230,230,230), spGetRGB(64,64,64));
	spFontAddButton( font_small, 'E', SP_BUTTON_SELECT_NOWASD_NAME, spGetRGB(230,230,230), spGetRGB(64,64,64));

	if (font_very_small)
		spFontDelete(font_very_small);
	font_very_small = spFontLoad(FONT_LOCATION,FONT_SIZE_VERY_SMALL*spGetSizeFactor()>>SP_ACCURACY);
	spFontAdd(font_very_small,SP_FONT_GROUP_ASCII,spGetRGB(192,192,192));//whole ASCII
	spFontAddBorder(font_very_small,BACKGROUND_COLOR);
	spFontMulWidth(font_very_small,15<<SP_ACCURACY-4);
	spFontAddButton( font_very_small, 'B', SP_PRACTICE_OK_NOWASD_NAME, spGetRGB(230,230,230), spGetRGB(64,64,64));
	spFontAddButton( font_very_small, 'X', SP_PRACTICE_CANCEL_NOWASD_NAME, spGetRGB(230,230,230), spGetRGB(64,64,64));
	spFontAddButton( font_very_small, 'L', SP_BUTTON_L_NOWASD_NAME, spGetRGB(230,230,230), spGetRGB(64,64,64));
	spFontAddButton( font_very_small, 'R', SP_BUTTON_R_NOWASD_NAME, spGetRGB(230,230,230), spGetRGB(64,64,64));
	spFontAddButton( font_very_small, 'S', SP_BUTTON_START_NOWASD_NAME, spGetRGB(230,230,230), spGetRGB(64,64,64));
	spFontAddButton( font_very_small, 'E', SP_BUTTON_SELECT_NOWASD_NAME, spGetRGB(230,230,230), spGetRGB(64,64,64));
}

int main(int argc, char **argv)
{
	srand(time(NULL));
	//spSetDefaultWindowSize(800,480);
	spInitCore();
	spSetReturnBehavior(1,0);
	spSetAffineTextureHack(0); //We don't need it :)
	spInitMath();
	screen = spCreateDefaultWindow();
	resize(screen->w,screen->h);
	
	spSetZSet(0);
	spSetZTest(0);
	banner = spLoadSurface("images/banner.png");

	start_menu(font,font_small,font_very_small);
	spLoop( draw, calc, 10, resize, NULL );
	finish_menu();
	
	spDeleteSurface(banner);

	spFontDelete(font);
	spFontDelete(font_small);
	spFontDelete(font_very_small);
	spQuitCore();
	return 0;
}
