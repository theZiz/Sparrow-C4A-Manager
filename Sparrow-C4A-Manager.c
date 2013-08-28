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

#define FONT_LOCATION "./font/CabinCondensed-Regular.ttf"
#define FONT_SIZE 12
#define FONT_SIZE_SMALL 10
#define BACKGROUND_COLOR spGetRGB(48,0,48)

SDL_Surface* screen;
spFontPointer font = NULL;
spFontPointer font_small = NULL;
SDL_Surface* banner;

#define TIME_OUT 15000

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

void draw( void )
{
	spClearTarget( BACKGROUND_COLOR );
	spRotozoomSurface( screen->w/6, 2*screen->h/36+spFixedToInt(banner->h*spGetSizeFactor()/4)/2+2, 0, banner, spGetSizeFactor()/4, spGetSizeFactor()/4,0);
	spFontDrawRight( screen->w-2, 2, 0, "[E] Exit", font_small );
	spFontDrawMiddle( screen->w/2, 2, 0, "[B] Enter letter", font_small );
	spFontDraw( 2, 2, 0, "[L] & [R] Row", font_small );
	switch (mode)
	{
		case 0:
			spFontDrawMiddle( 2*screen->w/3, 1*screen->h/9, 0, ">>> Account Creating <<<", font);
			spFontDrawMiddle( 2*screen->w/3, 2*screen->h/11, 0, "Press [S] to create the account", font_small);
			break;		
		case 1:
			spFontDrawMiddle( 2*screen->w/3, 1*screen->h/9, 0, ">>> Account Editing <<<", font);
			spFontDrawMiddle( 2*screen->w/3, 2*screen->h/11, 0, "Press [S] to edit the account", font_small);
			spFontDrawMiddle( 2*screen->w/3, 2*screen->h/8, 0, "Press [X] to delete the account", font_small);
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
	spFlip();
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

int calc(Uint32 steps)
{
	if ( spGetInput()->button[SP_BUTTON_SELECT_NOWASD] )
		return 1;
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
	if (askMode)
		return 0;
	
	int i;
	for (i = 0; shortName[i] != 0; i++)
	{
		if ('a' <= shortName[i] && shortName[i] <= 'z')
			shortName[i] += 'A'-'a';
	}
		
	if ( spGetInput()->button[SP_BUTTON_R_NOWASD])
	{
		line = (line + 1) % 4;
		spGetInput()->button[SP_BUTTON_R_NOWASD] = 0;
		blink = 0;
		switch (line)
		{
			case 0: spPollKeyboardInput(shortName,3,SP_PRACTICE_OK_NOWASD_MASK); break;
			case 1: spPollKeyboardInput(longName,256,SP_PRACTICE_OK_NOWASD_MASK); break;
			case 2: spPollKeyboardInput(password,256,SP_PRACTICE_OK_NOWASD_MASK); break;
			case 3: spPollKeyboardInput(mail,256,SP_PRACTICE_OK_NOWASD_MASK); break;
		}
	}
	if ( spGetInput()->button[SP_BUTTON_L_NOWASD])
	{
		line = (line + 3) % 4;
		spGetInput()->button[SP_BUTTON_L_NOWASD] = 0;
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
	if ( mode == 1 && spGetInput()->button[SP_PRACTICE_CANCEL_NOWASD] )
	{
		 spGetInput()->button[SP_PRACTICE_CANCEL_NOWASD] = 0;
		 askMode = 1;
		 spStopKeyboardInput();
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
}

int main(int argc, char **argv)
{
	srand(time(NULL));
	spSetDefaultWindowSize(800,480);
	spInitCore();
	spSetAffineTextureHack(0); //We don't need it :)
	spInitMath();
	screen = spCreateDefaultWindow();
	resize(screen->w,screen->h);
	
	spSetZSet(0);
	spSetZTest(0);
	banner = spLoadSurface("images/banner.png");

	spPollKeyboardInput(shortName,3,SP_PRACTICE_OK_NOWASD_MASK);
	
	profile = spNetC4AGetProfile();
	if (profile)
	{
		memcpy(longName,profile->longname,256);
		memcpy(shortName,profile->shortname,4);
		memcpy(password,profile->password,256);
		memcpy(mail,profile->email,256);
		mode = 1;
	}

	spLoop( draw, calc, 10, resize, NULL );

	spNetC4AFreeProfile(profile);
	
	spDeleteSurface(banner);

	spFontDelete(font);
	spFontDelete(font_small);
	spQuitCore();
	return 0;
}
