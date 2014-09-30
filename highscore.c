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

#include "highscore.h"
#include "defines.h"

int highscore_blink = 0;
char filter[256];
int serverTask = 0; //getting game List
spNetC4AGamePointer gameList = NULL;
spNetC4AScorePointer scoreList = NULL;
spNetC4AGamePointer beforeGame[2] = {NULL,NULL};
spNetC4AGamePointer selectedGame = NULL;
int scoreCount = 0;
int highMode = 0;
int showScore = 0;
int gameCount = 0;
int pos = 0;
int r_pressed = 0;
int l_pressed = 0;
int r_time = 0;
int l_time = 0;
int year,momYear;
int month,momMonth;

void draw_high_with_border(int x,int y,int z,const char* text_,spFontPointer font,int p)
{
	int text_len = strlen(text_);
	char text[text_len+9]; //->____<- + 0
	if (p)
		sprintf(text,"->  %s  <-",text_);
	else
		memcpy(text,text_,text_len+1);
	int l = spFontWidth(text,font);
	int l_ = spFontWidth(text_,font);
	spFontDrawMiddle(x,y,z,text,font);
	if (p)
		spRectangleBorder(x,y+font->maxheight/2,z,l_+2*BORDER_DISTANCE,font->maxheight+2*BORDER_DISTANCE,BORDER_SIZE,BORDER_SIZE,BORDER_COLOR);
}

int filter_matches(const char* test)
{
	if (filter[0] == 0)
		return 1;
	int l = strlen(test)+1;
	char TEST[l];
	int i;
	for (i=0; i < l; i++)
	{
		if ('a' <= test[i] && test[i] <= 'z')
			TEST[i] = test[i] + 'A'-'a';
		else
			TEST[i] = test[i];
	}
	if (strstr(TEST,filter))
		return 1;
	return 0;
}

void updateSelectedGame()
{
	int i = 0;
	selectedGame = gameList;
	beforeGame[0] = NULL;
	beforeGame[1] = NULL;
	while (selectedGame && (i < pos || !filter_matches(selectedGame->longname)))
	{
		if (filter_matches(selectedGame->longname))
		{
			beforeGame[1] = beforeGame[0];
			beforeGame[0] = selectedGame;
		}
		selectedGame = selectedGame->next;
		i++;
	}
	pos = i;
}

Sint32 scorePosition = 0;

int right_after_task = 0;

void draw_highscore(spFontPointer font,spFontPointer font_small,spFontPointer font_very_small)
{
	SDL_Surface* screen = spGetWindowSurface();
	char buffer[256];
	if (showScore == 0)
	{
		if (spGetVirtualKeyboardState() == SP_VIRTUAL_KEYBOARD_ALWAYS)	
		{
			spFontDrawMiddle( screen->w*2/3, 2, 0, "[B] Enter letter", font_very_small );
			spFontDrawMiddle( screen->w/5, screen->h/9, 0, "[S] View Highscore", font_small);
			spFontDraw( 2, 2, 0, "[L] & [R]: Select Row", font_very_small );
		}
		else
		{
			spFontDrawMiddle( screen->w/5, screen->h/9, 0, "[B] View Highscore", font_small);
			spFontDraw( 2, 2, 0, SP_PAD_NAME": Select Row", font_very_small );
		}
		
		spFontDrawRight( screen->w/2, 1*screen->h/9, 0, "Filter:", font);
		spFontDrawMiddle( 3*screen->w/4, 1*screen->h/9, 0, filter, font);
		spLine( screen->w/2+10, 3*screen->h/18, 0, screen->w-10, 3*screen->h/18,0,65535);
		spFontDrawMiddle(  3*screen->w/4, 3*screen->h/18, 0, "(e.g. Puzzletube)", font_very_small);
		spLine( 3*screen->w/4 + spFontWidth(filter,font)/2+1, 2*screen->h/18, 0,
				3*screen->w/4 + spFontWidth(filter,font)/2+1, 3*screen->h/18-1, 0, ((highscore_blink/512)&1)?0:65535);

		if (serverTask > 0)
		{
			if (selectedGame)
			{
				if (beforeGame[1])
					draw_high_with_border(screen->w/2,screen->h/2 + -2*screen->h/12,0,beforeGame[1]->longname,font,0);
				if (beforeGame[0])
					draw_high_with_border(screen->w/2,screen->h/2 + -1*screen->h/12,0,beforeGame[0]->longname,font,0);
				draw_high_with_border(screen->w/2,screen->h/2 +  0*screen->h/12,0,selectedGame->longname,font,1);
				int i;
				spNetC4AGamePointer game = selectedGame->next;
				for (i = 1; i < 6; i++)
				{
					while (game)
					{
						if (filter_matches(game->longname))
							break;
						game = game->next;
					}
					if (!game)
						break;
					draw_high_with_border(screen->w/2,screen->h/2 + i*screen->h/12,0,game->longname,font,0);
					game = game->next;
				}
			}
		}
	}
	else
	{
		spNetC4AScorePointer score = scoreList;
		int y = 2*screen->h/9-spFixedToInt(scorePosition*screen->h/15);
		while (score)
		{
			if (y > 1*screen->h/9 && y < 10*screen->h/9)
			{
				spFontDrawRight( screen->w/2-20, y, 0, score->longname, font );
				spLine(screen->w/2-10,y+font->maxheight/2,0,screen->w/2+10,y+font->maxheight/2,0,65535);
				sprintf(buffer,"%i",score->score);
				spFontDraw( screen->w/2+20, y, 0, buffer, font );
			}
			y+=screen->h/15;
			score = score->next;
		}
		spRectangle(screen->w/2,screen->h/9,0,screen->w,2*screen->h/9,BACKGROUND_COLOR);
		if (year == momYear && month == momMonth + 1)
			sprintf(buffer,"All Time Highscore of \"%s\"",selectedGame->longname);
		else
		switch (month)
		{
			case  1: sprintf(buffer,"January %i - Highscore of \"%s\"",year,selectedGame->longname); break;
			case  2: sprintf(buffer,"February %i - Highscore of \"%s\"",year,selectedGame->longname); break;
			case  3: sprintf(buffer,"March %i - Highscore of \"%s\"",year,selectedGame->longname); break;
			case  4: sprintf(buffer,"April %i - Highscore of \"%s\"",year,selectedGame->longname); break;
			case  5: sprintf(buffer,"May %i - Highscore of \"%s\"",year,selectedGame->longname); break;
			case  6: sprintf(buffer,"June %i - Highscore of \"%s\"",year,selectedGame->longname); break;
			case  7: sprintf(buffer,"July %i - Highscore of \"%s\"",year,selectedGame->longname); break;
			case  8: sprintf(buffer,"August %i - Highscore of \"%s\"",year,selectedGame->longname); break;
			case  9: sprintf(buffer,"September %i - Highscore of \"%s\"",year,selectedGame->longname); break;
			case 10: sprintf(buffer,"October %i - Highscore of \"%s\"",year,selectedGame->longname); break;
			case 11: sprintf(buffer,"November %i - Highscore of \"%s\"",year,selectedGame->longname); break;
			case 12: sprintf(buffer,"December %i - Highscore of \"%s\"",year,selectedGame->longname); break;
		}
		spFontDrawMiddle( screen->w/2, screen->h/9, 0, buffer, font );
		spFontDraw( 2, 2, 0, SP_PAD_NAME": Scroll", font_very_small );
		spFontDrawMiddle( screen->w/2, 2, 0, "[L] & [R]: Select month", font_very_small );
	}
	spFontDrawRight( screen->w-2, 2, 0, "[X] Back", font_very_small );

	switch (highMode)
	{
		case 1:
			spInterpolateTargetToColor(0,3*SP_ONE/4);
			spFontDrawMiddle( screen->w/2, screen->h/2-font->maxheight/2, 0, "Couldn't load game list. Check your internet connection!", font);
			spFontDrawMiddle( screen->w/2, screen->h/2+font->maxheight/2, 0, "[B] Okay", font);
			break;
		case 2:
			spInterpolateTargetToColor(0,3*SP_ONE/4);
			spFontDrawMiddle( screen->w/2, screen->h/2-font->maxheight/2, 0, "Couldn't load highscore. Check your internet connection!", font);
			spFontDrawMiddle( screen->w/2, screen->h/2+font->maxheight/2, 0, "[B] Okay", font);
			break;
	}	

	if (spNetC4AGetStatus() > 0)
	{
		spInterpolateTargetToColor(0,3*SP_ONE/4);
		switch (serverTask)
		{
			case 0: spFontDrawMiddle( screen->w/2, screen->h/2-font->maxheight/2, 0, "Getting game list...", font); break;
			case 1:
				sprintf(buffer,"Getting highscore of %s...",selectedGame->longname);
				spFontDrawMiddle( screen->w/2, screen->h/2-font->maxheight/2, 0, buffer, font);
				break;
		}
		sprintf(buffer,"Timeout in %i.%i",spNetC4AGetTimeOut()/1000,(spNetC4AGetTimeOut()/100)%10);
		spFontDrawMiddle( screen->w/2, screen->h/2+font->maxheight/2, 0, buffer, font);
	}
	if (spIsKeyboardPolled() && spGetVirtualKeyboardState() == SP_VIRTUAL_KEYBOARD_ALWAYS)
		spBlitSurface(screen->w/2,screen->h-spGetVirtualKeyboard()->h/2,0,spGetVirtualKeyboard());	
}

void updateScore()
{
	spNetC4ADeleteScores(&scoreList);
	if (year == momYear && month == momMonth + 1)
	{
		if (spNetC4AGetScore(&scoreList,NULL,selectedGame->shortname,TIME_OUT) == 0)
			right_after_task = 1;
	}
	else
	{
		if (spNetC4AGetScoreOfMonth(&scoreList,NULL,selectedGame->shortname,year,month,TIME_OUT) == 0)
			right_after_task = 1;
	}
}

int calc_highscore(Uint32 steps)
{
	highscore_blink+=steps;
	if (spNetC4AGetStatus() > 0)
	{
		right_after_task = 1;
		return 0;
	}
	if (right_after_task)
	{
		if (spNetC4AGetTaskResult() == 0)
		{
			if (serverTask == 0)
			{
				spNetC4AGamePointer game = gameList;
				selectedGame = gameList;
				gameCount = 0;
				while (game)
				{
					printf("%s - %s - %s - %i - %i\n",game->longname,game->shortname,game->genre,game->status,game->field);
					gameCount++;
					game = game->next;
				}
				spPollKeyboardInput(filter,256,SP_PRACTICE_OK_NOWASD_MASK);
				serverTask = 1;
			}
			else
			{
				showScore = 1;
				scorePosition = 0;
				spNetC4AScorePointer score = scoreList;
				scoreCount = 0;
				while (score)
				{
					scoreCount++;
					score = score->next;
				}
			}
		}
		else
		{
			if (serverTask == 0)
				highMode = 1; //error
			else
				highMode = 2;
		}
	}
	right_after_task = 0;
	if ( spGetInput()->button[SP_PRACTICE_CANCEL_NOWASD] )
	{
		spGetInput()->button[SP_PRACTICE_CANCEL_NOWASD] = 0;
		if (showScore == 0)
		{
			spStopKeyboardInput();
			return 1;
		}
		else
		{
			spNetC4ADeleteScores(&scoreList);
			showScore = 0;
			spPollKeyboardInput(filter,256,SP_PRACTICE_OK_NOWASD_MASK);
		}
	}

	switch ( highMode )
	{
		case 1:
			if ( spGetInput()->button[SP_PRACTICE_OK_NOWASD] )
			{
				spGetInput()->button[SP_PRACTICE_OK_NOWASD] = 0;
				highMode = 0;
				return 1;
			}
			break;
		case 2:
			if ( spGetInput()->button[SP_PRACTICE_OK_NOWASD] )
			{
				spGetInput()->button[SP_PRACTICE_OK_NOWASD] = 0;
				highMode = 0;
			}
			break;
	}
	if (highMode)
		return 0;
	if ( showScore )
	{
		if (spGetInput()->button[SP_BUTTON_R_NOWASD])
		{
			spGetInput()->button[SP_BUTTON_R_NOWASD] = 0;
			if (year != momYear || month != momMonth + 1)
			{
				month++;
				if (month > 12)
				{
					month = 1;
					year++;
				}
				updateScore();
			}
		}
		if (spGetInput()->button[SP_BUTTON_L_NOWASD])
		{
			spGetInput()->button[SP_BUTTON_L_NOWASD] = 0;
			month--;
			if (month <= 0)
			{
				month = 12;
				year--;
			}
			updateScore();
		}		if ( spGetInput()->axis[1] > 0)
		{
			scorePosition+=steps*1024;
			if (spFixedToInt(scorePosition) > scoreCount-2)
				scorePosition = spIntToFixed(scoreCount-2)+SP_ONE-1;
		}
		if (spGetInput()->axis[1] < 0)
		{
			scorePosition-=steps*1024;
			if (spFixedToInt(scorePosition) <0)
				scorePosition = 0;
		}
	}
	else
	{
		int i;
		for (i = 0; filter[i] != 0; i++)
		{
			if ('a' <= filter[i] && filter[i] <= 'z')
				filter[i] += 'A'-'a';
		}

		if (selectedGame && (spGetInput()->button[SP_BUTTON_START_NOWASD] ||
		   (spGetVirtualKeyboardState() != SP_VIRTUAL_KEYBOARD_ALWAYS && spGetInput()->button[SP_PRACTICE_OK_NOWASD])))
		{
			spGetInput()->button[SP_BUTTON_START_NOWASD] = 0;
			spGetInput()->button[SP_PRACTICE_OK_NOWASD] = 0;
			spStopKeyboardInput();
			time_t rawtime;
			struct tm * ptm;
			time ( &rawtime );
			ptm = gmtime ( &rawtime );	
			momYear = ptm->tm_year+1900;
			momMonth = ptm->tm_mon+1;
			year = momYear;
			month = momMonth+1;
			updateScore();
		}	
		
		if (r_pressed)
			r_time -= steps;
		if ((spGetVirtualKeyboardState() == SP_VIRTUAL_KEYBOARD_ALWAYS && spGetInput()->button[SP_BUTTON_R_NOWASD]) ||
		    (spGetVirtualKeyboardState() != SP_VIRTUAL_KEYBOARD_ALWAYS && spGetInput()->axis[1] > 0))
		{
			if ( (!r_pressed) ||
				 ( r_pressed && r_time<0))
			{
				pos = (pos + 1) % gameCount;
				updateSelectedGame();
				if (!r_pressed) //first
					r_time = 300;
				else
					r_time = 100;
				r_pressed = 1;
			}
		}
		else
		{
			r_pressed = 0;
			r_time = 0;
		}
		
		if (l_pressed)
			l_time -= steps;
		if ((spGetVirtualKeyboardState() == SP_VIRTUAL_KEYBOARD_ALWAYS && spGetInput()->button[SP_BUTTON_L_NOWASD]) ||
		    (spGetVirtualKeyboardState() != SP_VIRTUAL_KEYBOARD_ALWAYS && spGetInput()->axis[1] < 0))
		{
			if ( (!l_pressed) ||
				 ( l_pressed && l_time<0))
			{
				pos = (pos - 1 + gameCount) % gameCount;
				updateSelectedGame();
				if (!l_pressed) //first
					l_time = 300;
				else
					l_time = 100;
				l_pressed = 1;
			}
		}
		else
		{
			l_pressed = 0;
			l_time = 0;
		}
		if (!selectedGame || !filter_matches(selectedGame->longname))
		{
			selectedGame = gameList;
			beforeGame[0] = NULL;
			beforeGame[1] = NULL;
			pos = 0;
			while (selectedGame)
			{
				if (filter_matches(selectedGame->longname))
					break;
				pos++;
				selectedGame = selectedGame->next;
			}
		}
	}
	return 0;
}

void start_highscore()
{
	beforeGame[0] = NULL;
	beforeGame[1] = NULL;
	selectedGame = NULL;
	pos = 0;
	highMode = 0;
	showScore = 0;
	gameCount = 0;
	right_after_task = 0;	
	serverTask = 0;
	if (spNetC4AGetGame(&gameList,TIME_OUT) == 0)
		right_after_task = 1;
}

void finish_highscore()
{
	spNetC4ADeleteGames(&gameList);
}
