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

#include "menu.h"
#include "defines.h"

char explanation[2048] = "Welcome to the Sparrow-C4A-Manager. C4A stands for \
\"Compo4All\". The Compo4All Highscore system made by skeezix is an easy way \
to submit highscores to an online highscore list to compare your achievements \
with players all over the world. With this application you can create your \
unique account for every supported game and view online highscores - even of \
uninstalled games. An internet connection is needed!";

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

void draw_menu(spFontPointer font,spFontPointer font_small)
{
	SDL_Surface* screen = spGetWindowSurface();
	spFontDrawRight( screen->w-2, 2, 0, "[X] Exit", font_small );
	spFontDrawMiddle( screen->w*2/3, 2, 0, "[B] Enter menu point", font_small );
	spFontDraw( 2, 2, 0, SP_PAD_NAME": Select menu point", font_small );
	
	draw_middle_with_border( screen->w*2/3, screen->h*1/8, 0, "Account Management", font, 0);
	draw_middle_with_border( screen->w*2/3, screen->h*2/8, 0, "Online Highscores", font, 1);
	draw_middle_with_border( screen->w*2/3, screen->h*3/8, 0, "Exit", font, 2);
	
	spFontDrawTextBlock(middle,5,screen->h*4/8,0,help,spGetWindowSurface()->h,0,font_small);
}

int calc_menu(Uint32 steps)
{
	if ( spGetInput()->button[SP_PRACTICE_CANCEL_NOWASD] )
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
			case 2: //exit
				return -1;
		}
	}
	if ( spGetInput()->axis[1] > 0)
	{
		menu_point = (menu_point + 1) % 3;
		spGetInput()->axis[1] = 0;
	}
	if ( spGetInput()->axis[1] < 0)
	{
		menu_point = (menu_point + 2) % 3;
		spGetInput()->axis[1] = 0;
	}	
	return 0;
}

void start_menu(spFontPointer font,spFontPointer font_small)
{
	help = spCreateTextBlock( explanation, spGetWindowSurface()->w-10,font_small);
}

void finish_menu()
{
	spDeleteTextBlock(help);
}
