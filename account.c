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

#include "account.h"
#include "defines.h"

int mode = 0; //0 no prof file found, 1 profile file exists
int nextMode = 0;
int askMode = 0;
int line = 0;
char shortName[4] = "";
char longName[256] = "";
char password[256] = "";
char mail[256] = "";
int blink = 0;
spNetC4AProfilePointer profile;

void draw_account(spFontPointer font,spFontPointer font_small)
{
	SDL_Surface* screen = spGetWindowSurface();
	spFontDrawRight( screen->w-2, 2, 0, "[X] Back", font_small );
	if (spGetVirtualKeyboardState() == SP_VIRTUAL_KEYBOARD_ALWAYS)	
		spFontDrawMiddle( screen->w*2/3, 2, 0, "[B] Enter letter", font_small );
	if (spGetVirtualKeyboardState() == SP_VIRTUAL_KEYBOARD_ALWAYS)
		spFontDraw( 2, 2, 0, "[L] & [R]: Select Row", font_small );
	else
		spFontDraw( 2, 2, 0, SP_PAD_NAME": Select Row", font_small );
	switch (mode)
	{
		case 0:
			spFontDrawMiddle( 2*screen->w/3, 1*screen->h/9, 0, ">>> Account Creating <<<", font);
			spFontDrawMiddle( 2*screen->w/3, 2*screen->h/11, 0, "Press [S] to create the account", font_small);
			break;		
		case 1:
			spFontDrawMiddle( 2*screen->w/3, 1*screen->h/9, 0, ">>> Account Editing <<<", font);
			spFontDrawMiddle( 2*screen->w/3, 2*screen->h/11, 0, "Press [S] to save the changes", font_small);
			spFontDrawMiddle( 2*screen->w/3, 2*screen->h/8, 0, "Press [E] to delete the account", font_small);
			break;		
	}
	
	char* mom_line = NULL;
	switch (line)
	{
		case 0: mom_line = shortName; break;
		case 1: mom_line = longName; break;
		case 2: mom_line = password; break;
		case 3: mom_line = mail; break;
	}
	spLine( 2*screen->w/3 + spFontWidth(mom_line,font)/2+1, (line*2+6)*screen->h/18, 0,
	        2*screen->w/3 + spFontWidth(mom_line,font)/2+1, (line*2+7)*screen->h/18, 0, ((blink/512)&1)?0:65535);
	
	spFontDrawRight( screen->w/3, 3*screen->h/9, 0, "3 Letter Nick:", font);
	spFontDrawMiddle( 2*screen->w/3, 3*screen->h/9, 0, shortName, font);
	spLine( screen->w/3+10, 7*screen->h/18, 0, screen->w-10, 7*screen->h/18,0,65535);
	spFontDrawMiddle( 2*screen->w/3, 7*screen->h/18, 0, "(e.g. JHN)", font_small);

	spFontDrawRight( screen->w/3, 4*screen->h/9, 0, "Display Nick:", font);
	spFontDrawMiddle( 2*screen->w/3, 4*screen->h/9, 0, longName, font);
	spLine( screen->w/3+10, 9*screen->h/18, 0, screen->w-10, 9*screen->h/18,0,65535);
	spFontDrawMiddle( 2*screen->w/3, 9*screen->h/18, 0, "(e.g. JohnSmith)", font_small);

	spFontDrawRight( screen->w/3, 5*screen->h/9, 0, "Password:", font);
	spFontDrawMiddle( 2*screen->w/3, 5*screen->h/9, 0, password, font);
	spLine( screen->w/3+10, 11*screen->h/18, 0, screen->w-10, 11*screen->h/18,0,65535);
	spFontDrawMiddle( 2*screen->w/3, 11*screen->h/18, 0, "(alphanumeric, e.g. aBc123)", font_small);

	spFontDrawRight( screen->w/3, 6*screen->h/9, 0, "E-Mail address:", font);
	spFontDrawMiddle( 2*screen->w/3, 6*screen->h/9, 0, mail, font);
	spLine( screen->w/3+10, 13*screen->h/18, 0, screen->w-10, 13*screen->h/18,0,65535);
	spFontDrawMiddle( 2*screen->w/3, 13*screen->h/18, 0, "(for score being beaten notification)", font_small);
	if (spIsKeyboardPolled() && spGetVirtualKeyboardState() == SP_VIRTUAL_KEYBOARD_ALWAYS)
		spBlitSurface(screen->w/2,screen->h-spGetVirtualKeyboard()->h/2,0,spGetVirtualKeyboard());
	
	switch (askMode)
	{
		case 1:
			spInterpolateTargetToColor(0,3*SP_ONE/4);
			spFontDrawMiddle( screen->w/2, screen->h/2-font->maxheight/2, 0, "Are you sure to delete your profile forever?", font);
			spFontDrawMiddle( screen->w/2, screen->h/2+font->maxheight/2, 0, "[S] Yes...     [X] Hell No!", font);
			break;
		case 2:
			spInterpolateTargetToColor(0,3*SP_ONE/4);
			spFontDrawMiddle( screen->w/2, screen->h/2-font->maxheight/2, 0, "Your 3 Letter Nick needs 3 alphanumeric letters.", font);
			spFontDrawMiddle( screen->w/2, screen->h/2+font->maxheight/2, 0, "[B] Okay...", font);
			break;
		case 3:
			spInterpolateTargetToColor(0,3*SP_ONE/4);
			spFontDrawMiddle( screen->w/2, screen->h/2-font->maxheight/2, 0, "Only numbers and characters are allowed for your Nick!", font);
			spFontDrawMiddle( screen->w/2, screen->h/2+font->maxheight/2, 0, "[B] Uuups...", font);
			break;
		case 4:
			spInterpolateTargetToColor(0,3*SP_ONE/4);
			spFontDrawMiddle( screen->w/2, screen->h/2-font->maxheight/2, 0, "Only numbers and characters are allowed for passwords!", font);
			spFontDrawMiddle( screen->w/2, screen->h/2+font->maxheight/2, 0, "[B] Okay...", font);
			break;
		case 5:
			spInterpolateTargetToColor(0,3*SP_ONE/4);
			spFontDrawMiddle( screen->w/2, screen->h/2-font->maxheight/2, 0, "IF you enter a e-mail, make sure it contains an @.", font);
			spFontDrawMiddle( screen->w/2, screen->h/2+font->maxheight/2, 0, "[B] Okay...", font);
			break;
		case 6:
			spInterpolateTargetToColor(0,3*SP_ONE/4);
			spFontDrawMiddle( screen->w/2, screen->h/2-font->maxheight/2, 0, "Couldn't connect to Server! Check your connection.", font);
			spFontDrawMiddle( screen->w/2, screen->h/2+font->maxheight/2, 0, "[B] Ok", font);
			break;
		case 7:
			spInterpolateTargetToColor(0,3*SP_ONE/4);
			spFontDrawMiddle( screen->w/2, screen->h/2-font->maxheight/2, 0, "Account created successfully", font);
			spFontDrawMiddle( screen->w/2, screen->h/2+font->maxheight/2, 0, "[B] Ok", font);
			break;
		case 8:
			spInterpolateTargetToColor(0,3*SP_ONE/4);
			spFontDrawMiddle( screen->w/2, screen->h/2-font->maxheight/2, 0, "Account edited successfully", font);
			spFontDrawMiddle( screen->w/2, screen->h/2+font->maxheight/2, 0, "[B] Ok", font);
			break;
		case 9:
			spInterpolateTargetToColor(0,3*SP_ONE/4);
			spFontDrawMiddle( screen->w/2, screen->h/2-font->maxheight/2, 0, "Account deleted successfully", font);
			spFontDrawMiddle( screen->w/2, screen->h/2+font->maxheight/2, 0, "[B] Ok", font);
			break;
	}	
	if (spNetC4AGetStatus() > 0)
	{
		spInterpolateTargetToColor(0,3*SP_ONE/4);
		spFontDrawMiddle( screen->w/2, screen->h/2-font->maxheight/2, 0, "Connecting to server...", font);
		char buffer[256];
		sprintf(buffer,"Timeout in %i.%i",spNetC4AGetTimeOut()/1000,(spNetC4AGetTimeOut()/100)%10);
		spFontDrawMiddle( screen->w/2, screen->h/2+font->maxheight/2, 0, buffer, font);
	}
}

int alpha_numeric(char c)
{
	if ('a' <= c && c <= 'z')
		return 1;
	if ('A' <= c && c <= 'Z')
		return 1;
	if ('0' <= c && c <= '9')
		return 1;
	return 0;
}

int capital(char c)
{
	if ('A' <= c && c <= 'Z')
		return 1;
	if ('0' <= c && c <= '9')
		return 1;
	return 0;
}

int check_shortname()
{
	int i;
	for (i = 0; i < 3; i++)
		if (!capital(shortName[i]) || shortName[i]==0)
			return 1;
	return 0;
}

int check_longname()
{
	if (longName[0] == 0)
		return 1;
	int i;
	for (i = 0; longName[i] != 0; i++)
		if (!alpha_numeric(longName[i]))
			return 1;
	return 0;
}

int check_password()
{
	if (password[0] == 0)
		return 1;
	int i;
	for (i = 0; password[i] != 0; i++)
		if (!alpha_numeric(password[i]))
			return 1;
	return 0;
}

int check_mail()
{
	if (mail[0] == 0)
		return 0;
	int i;
	for (i = 0; mail[i] != 0; i++)
		if (mail[i] == '@')
			return 0;
	return 1;
}

int right_after_status = 0;
int last_task = 0;

int calc_account(Uint32 steps)
{
	blink+=steps;
	if (spNetC4AGetStatus() > 0)
	{
		right_after_status = 1;
		return 0;
	}
	if (right_after_status)
	{
		if (spNetC4AGetTaskResult() == 0)
		{
			mode = nextMode;
			askMode = last_task+7;
		}
		else
		{
			askMode = 6;
			spStopKeyboardInput();
		}
			
	}
	right_after_status = 0;
	if (askMode)
	{
		switch ( askMode )
		{
			case 1:
				if ( spGetInput()->button[SP_BUTTON_START_NOWASD] )
				{
					spGetInput()->button[SP_BUTTON_START_NOWASD] = 0;
					if (spNetC4ADeleteAccount(&profile,1,TIME_OUT) == 0)
					{
						right_after_status = 1;
						last_task = 2;
					}
					nextMode = 0;
					sprintf(longName,"");
					sprintf(shortName,"");
					sprintf(password,"");
					sprintf(mail,"");
					askMode = 0;
					switch (line)
					{
						case 0: spPollKeyboardInput(shortName,3,SP_PRACTICE_OK_NOWASD_MASK); break;
						case 1: spPollKeyboardInput(longName,256,SP_PRACTICE_OK_NOWASD_MASK); break;
						case 2: spPollKeyboardInput(password,256,SP_PRACTICE_OK_NOWASD_MASK); break;
						case 3: spPollKeyboardInput(mail,256,SP_PRACTICE_OK_NOWASD_MASK); break;
					}
				}
				if ( spGetInput()->button[SP_PRACTICE_CANCEL_NOWASD] )
				{
					spGetInput()->button[SP_PRACTICE_CANCEL_NOWASD] = 0;
					askMode = 0;
					switch (line)
					{
						case 0: spPollKeyboardInput(shortName,3,SP_PRACTICE_OK_NOWASD_MASK); break;
						case 1: spPollKeyboardInput(longName,256,SP_PRACTICE_OK_NOWASD_MASK); break;
						case 2: spPollKeyboardInput(password,256,SP_PRACTICE_OK_NOWASD_MASK); break;
						case 3: spPollKeyboardInput(mail,256,SP_PRACTICE_OK_NOWASD_MASK); break;
					}
				}
				break;
			default:
				if ( spGetInput()->button[SP_PRACTICE_OK_NOWASD] )
				{
					spGetInput()->button[SP_PRACTICE_OK_NOWASD] = 0;
					askMode = 0;
					switch (line)
					{
						case 0: spPollKeyboardInput(shortName,3,SP_PRACTICE_OK_NOWASD_MASK); break;
						case 1: spPollKeyboardInput(longName,256,SP_PRACTICE_OK_NOWASD_MASK); break;
						case 2: spPollKeyboardInput(password,256,SP_PRACTICE_OK_NOWASD_MASK); break;
						case 3: spPollKeyboardInput(mail,256,SP_PRACTICE_OK_NOWASD_MASK); break;
					}
				}
		}
		return 0;
	}
	if ( spGetInput()->button[SP_PRACTICE_CANCEL_NOWASD] )
	{
		spGetInput()->button[SP_PRACTICE_CANCEL_NOWASD] = 0;
		spStopKeyboardInput();
		return 1;
	}
	
	int i;
	for (i = 0; shortName[i] != 0; i++)
	{
		if ('a' <= shortName[i] && shortName[i] <= 'z')
			shortName[i] += 'A'-'a';
	}
		
	if (spGetInput()->button[SP_BUTTON_R_NOWASD] ||
	   (spGetVirtualKeyboardState() != SP_VIRTUAL_KEYBOARD_ALWAYS && spGetInput()->axis[1] > 0))
	{
		line = (line + 1) % 4;
		spGetInput()->button[SP_BUTTON_R_NOWASD] = 0;
		if (spGetVirtualKeyboardState() != SP_VIRTUAL_KEYBOARD_ALWAYS)
			spGetInput()->axis[1] = 0;
		blink = 0;
		switch (line)
		{
			case 0: spPollKeyboardInput(shortName,3,SP_PRACTICE_OK_NOWASD_MASK); break;
			case 1: spPollKeyboardInput(longName,256,SP_PRACTICE_OK_NOWASD_MASK); break;
			case 2: spPollKeyboardInput(password,256,SP_PRACTICE_OK_NOWASD_MASK); break;
			case 3: spPollKeyboardInput(mail,256,SP_PRACTICE_OK_NOWASD_MASK); break;
		}
	}
	if (spGetInput()->button[SP_BUTTON_L_NOWASD] ||
	   (spGetVirtualKeyboardState() != SP_VIRTUAL_KEYBOARD_ALWAYS && spGetInput()->axis[1] < 0))
	{
		line = (line + 3) % 4;
		spGetInput()->button[SP_BUTTON_L_NOWASD] = 0;
		if (spGetVirtualKeyboardState() != SP_VIRTUAL_KEYBOARD_ALWAYS)
			spGetInput()->axis[1] = 0;
		blink = 0;
		switch (line)
		{
			case 0: spPollKeyboardInput(shortName,3,SP_PRACTICE_OK_NOWASD_MASK); break;
			case 1: spPollKeyboardInput(longName,256,SP_PRACTICE_OK_NOWASD_MASK); break;
			case 2: spPollKeyboardInput(password,256,SP_PRACTICE_OK_NOWASD_MASK); break;
			case 3: spPollKeyboardInput(mail,256,SP_PRACTICE_OK_NOWASD_MASK); break;
		}
	}
	if ( spGetInput()->button[SP_BUTTON_START_NOWASD] )
	{
		spGetInput()->button[SP_BUTTON_START_NOWASD] = 0;
		if (check_shortname())
		{
			askMode = 2;
			spStopKeyboardInput();
		}
		else
		if (check_longname())
		{
			askMode = 3;
			spStopKeyboardInput();
		}
		else
		if (check_password())
		{
			askMode = 4;
			spStopKeyboardInput();
		}
		else
		if (check_mail())
		{
			askMode = 5;
			spStopKeyboardInput();
		}
		else
		{
			if (mode == 0)
			{
				if (spNetC4ACreateProfile(&profile,longName,shortName,password,mail,TIME_OUT) == 0)
				{
					right_after_status = 1;
					last_task = 0;
				}
				nextMode = 1;
			}
			else
			{
				if (spNetC4AEditProfile(&profile,longName,shortName,password,mail,TIME_OUT) == 0)
				{
					right_after_status = 1;
					last_task = 1;
				}
				nextMode = 1;
			}
		}
	}
	if ( mode == 1 && spGetInput()->button[SP_BUTTON_SELECT_NOWASD] )
	{
		 spGetInput()->button[SP_BUTTON_SELECT_NOWASD] = 0;
		 askMode = 1;
		 spStopKeyboardInput();
	}
	return 0;
}

void start_account()
{
	spPollKeyboardInput(shortName,3,SP_PRACTICE_OK_NOWASD_MASK);
	mode = 0;
	line = 0;
	profile = spNetC4AGetProfile();
	if (profile)
	{
		memcpy(longName,profile->longname,256);
		memcpy(shortName,profile->shortname,4);
		memcpy(password,profile->password,256);
		memcpy(mail,profile->email,256);
		mode = 1;
	}	
}

void finish_account()
{
	spNetC4AFreeProfile(profile);
}
