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

#include <string.h>
#include "menu.h"
#include "defines.h"

#ifdef GCW
	#define C4A_POS "/usr/local/home/.config/compo4all/"
#elif defined PANDORA
	#define C4A_POS "your compo4all appdata folder"
#else
	#define C4A_POS "~/.config/compo4all/"
#endif

char explanation[2048] = "Welcome to the Sparrow-C4A-Manager. C4A stands for \
\"Compo4All\". The Compo4All Highscore system made by skeezix is an easy way \
to submit highscores to an online highscore list to compare your achievements \
with players all over the world. With this application you can create your \
unique account for every supported game and view online highscores - even of \
uninstalled games. An internet connection is needed! You can use the same \
profile on different devices by copying the c4a-prof to any device. On the \
"SP_DEVICE_STRING" the file lies in "C4A_POS". ";

spTextBlockPointer help;
int menu_point = 0;

void draw_middle_with_border(int x,int y,int z,char* text_,spFontPointer font,int p)
{
	int text_len = strlen(text_);
	char text[text_len+9]; //->____<- + 0
	int factor = 1;
	if (p == menu_point)
	{
		sprintf(text,"->  %s  <-",text_);
		factor = 2;
	}
	else
		memcpy(text,text_,text_len+1);
	int l = spFontWidth(text,font);
	int l_ = spFontWidth(text_,font);
	spFontDraw(x-l/2,y,z,text,font);
	spRectangleBorder(x,y+font->maxheight/2,z,l_+2*BORDER_DISTANCE,font->maxheight+2*BORDER_DISTANCE,BORDER_SIZE*factor,BORDER_SIZE*factor,BORDER_COLOR);
}

int cachenr = 0;
int menu_after_task = 0;

int error_mode = 0;

void draw_menu(spFontPointer font,spFontPointer font_small,spFontPointer font_very_small)
{
	SDL_Surface* screen = spGetWindowSurface();
	spFontDrawRight( screen->w-2, 2, 0, "[E] Exit", font_very_small );
	spFontDrawMiddle( screen->w*2/3, 2, 0, "[B] Enter menu point", font_very_small );
	spFontDraw( 2, 2, 0, SP_PAD_NAME": Select menu point", font_very_small );
	
	draw_middle_with_border( screen->w*2/3, screen->h*1/9, 0, "Account Management", font, 0);
	draw_middle_with_border( screen->w*2/3, screen->h*2/9, 0, "Online Highscores", font, 1);
	char buffer[64];
	sprintf(buffer,"Commit cached scores (%i)",cachenr);
	draw_middle_with_border( screen->w*2/3, screen->h*3/9, 0, buffer, font, 2);
	draw_middle_with_border( screen->w*2/3, screen->h*4/9, 0, "Exit", font, 3);
	
	spFontDrawTextBlock(middle,5,screen->h*5/9,0,help,spGetWindowSurface()->h,0,font_very_small);

	switch (error_mode)
	{
		case 1:
			spInterpolateTargetToColor(0,3*SP_ONE/4);
			spFontDrawMiddle( screen->w/2, screen->h/2-font->maxheight/2, 0, "Couldn't submit. Check your internet connection!", font);
			spFontDrawMiddle( screen->w/2, screen->h/2+font->maxheight/2, 0, "[B] Okay", font);
			break;
	}	
	if (spNetC4AGetStatus() > 0)
	{
		spInterpolateTargetToColor(0,3*SP_ONE/4);
		spFontDrawMiddle( screen->w/2, screen->h/2-font->maxheight/2, 0, "Submitting cached scores...", font);
		sprintf(buffer,"Timeout in %i.%i",spNetC4AGetTimeOut()/1000,(spNetC4AGetTimeOut()/100)%10);
		spFontDrawMiddle( screen->w/2, screen->h/2+font->maxheight/2, 0, buffer, font);
	}
}

void set_error_mode(int e)
{
	error_mode = e;
}

int calc_menu(Uint32 steps)
{
	if (spNetC4AGetStatus() > 0)
	{
		menu_after_task = 1;
		return 0;
	}
	if (menu_after_task)
	{
		if (spNetC4AGetTaskResult())
			error_mode = 1;
		else
			cachenr = spNetC4AHowManyCached();
	}
	menu_after_task = 0;
	
	switch ( error_mode )
	{
		case 1:
			if ( spGetInput()->button[SP_PRACTICE_OK_NOWASD] )
			{
				spGetInput()->button[SP_PRACTICE_OK_NOWASD] = 0;
				error_mode = 0;
			}
			break;
	}
	if (error_mode)
		return 0;
	if ( spGetInput()->button[SP_BUTTON_SELECT_NOWASD] )
		return -1;
	if ( spGetInput()->button[SP_PRACTICE_OK_NOWASD] )
	{
		spGetInput()->button[SP_PRACTICE_OK_NOWASD] = 0;
		switch (menu_point)
		{
			case 0: //account
				return 1;
			case 1: //highscore
				return 2;
			case 2: //commit
				return 3;
			case 3: //exit
				return -1;
		}
	}
	if ( spGetInput()->axis[1] > 0)
	{
		menu_point = (menu_point + 1) % 4;
		spGetInput()->axis[1] = 0;
	}
	if ( spGetInput()->axis[1] < 0)
	{
		menu_point = (menu_point + 3) % 4;
		spGetInput()->axis[1] = 0;
	}	
	return 0;
}

void start_menu(spFontPointer font,spFontPointer font_small,spFontPointer font_very_small)
{
	spNetC4AFreeProfile(spNetC4AGetProfile());
	cachenr = spNetC4AHowManyCached();
	help = spCreateTextBlock( explanation, spGetWindowSurface()->w-10,font_very_small);
}

void finish_menu()
{
	spDeleteTextBlock(help);
}
