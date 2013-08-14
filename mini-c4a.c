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

int mode = 0; //0 no prof file found, 1 profile file exists
int askMode = 0;
int line = 0;
char shortName[256] = "";
char longName[256] = "";
char password[256] = "";
char mail[256] = "";
int blink = 0;
spNetC4AProfilePointer profile;

void draw( void )
{
	spClearTarget( BACKGROUND_COLOR );
	spRotozoomSurface( screen->w/6, 3*screen->h/36+spFixedToInt(banner->h*spGetSizeFactor()/4)/2+2, 0, banner, spGetSizeFactor()/4, spGetSizeFactor()/4,0);
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
	
	spFontDrawRight( screen->w/3, 3*screen->h/9, 0, "3 Letter Nick: ", font);
	spFontDrawMiddle( 2*screen->w/3, 3*screen->h/9, 0, shortName, font);
	spLine( screen->w/3+2, 7*screen->h/18, 0, screen->w-2, 7*screen->h/18,0,65535);
	spFontDrawMiddle( 2*screen->w/3, 7*screen->h/18, 0, "(e.g. EVD)", font_small);

	spFontDrawRight( screen->w/3, 4*screen->h/9, 0, "Display Nick: ", font);
	spFontDrawMiddle( 2*screen->w/3, 4*screen->h/9, 0, longName, font);
	spLine( screen->w/3+2, 9*screen->h/18, 0, screen->w-2, 9*screen->h/18,0,65535);
	spFontDrawMiddle( 2*screen->w/3, 9*screen->h/18, 0, "(e.g. EvilDragon)", font_small);

	spFontDrawRight( screen->w/3, 5*screen->h/9, 0, "Password: ", font);
	spFontDrawMiddle( 2*screen->w/3, 5*screen->h/9, 0, password, font);
	spLine( screen->w/3+2, 11*screen->h/18, 0, screen->w-2, 11*screen->h/18,0,65535);
	spFontDrawMiddle( 2*screen->w/3, 11*screen->h/18, 0, "(alphanumeric, e.g. aBc123)", font_small);

	spFontDrawRight( screen->w/3, 6*screen->h/9, 0, "E-Mail address: ", font);
	spFontDrawMiddle( 2*screen->w/3, 6*screen->h/9, 0, mail, font);
	spLine( screen->w/3+2, 13*screen->h/18, 0, screen->w-2, 13*screen->h/18,0,65535);
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
			spFontDrawMiddle( screen->w/2, screen->h/2-font->maxheight/2, 0, "Your 3 Letter Nick needs exactly 3 BIG letters.", font);
			spFontDrawMiddle( screen->w/2, screen->h/2+font->maxheight/2, 0, "[B] Okay...", font);
			break;
		case 3:
			spInterpolateTargetToColor(0,3*SP_ONE/4);
			spFontDrawMiddle( screen->w/2, screen->h/2-font->maxheight/2, 0, "You let your Display name empty!", font);
			spFontDrawMiddle( screen->w/2, screen->h/2+font->maxheight/2, 0, "[B] Uuups...", font);
			break;
		case 4:
			spInterpolateTargetToColor(0,3*SP_ONE/4);
			spFontDrawMiddle( screen->w/2, screen->h/2-font->maxheight/2, 0, "Only numbers and characters are allowed for passwords!", font);
			spFontDrawMiddle( screen->w/2, screen->h/2+font->maxheight/2, 0, "[B] Okay...", font);
			break;
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
	return 0;
}

int check_shortname()
{
	if (capital(shortName[0]) && 
	    capital(shortName[1]) &&
	    capital(shortName[2]) &&
	    shortName[3] == 0)
		return 0;
	return 1;
}

int check_longname()
{
	if (longName[0] == 0)
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

int calc(Uint32 steps)
{
	if ( spGetInput()->button[SP_BUTTON_SELECT] )
		return 1;
	blink+=steps;
	switch ( askMode )
	{
		case 1:
			if ( spGetInput()->button[SP_BUTTON_START] )
			{
				spGetInput()->button[SP_BUTTON_START] = 0;
				spNetC4ADeleteAccount(profile);
				spNetC4ADeleteProfileFile();
				profile = NULL;
				mode = 0;
				sprintf(longName,"");
				sprintf(shortName,"");
				sprintf(password,"");
				sprintf(mail,"");
				askMode = 0;
			}
			if ( spGetInput()->button[SP_BUTTON_X] )
			{
				spGetInput()->button[SP_BUTTON_X] = 0;
				askMode = 0;
			}
			break;
		case 2: case 3: case 4:
			if ( spGetInput()->button[SP_BUTTON_B] )
			{
				spGetInput()->button[SP_BUTTON_B] = 0;
				askMode = 0;
			}
			break;
	}
	if (askMode)
		return 0;
	if ( spGetInput()->button[SP_BUTTON_R])
	{
		line = (line + 1) % 4;
		spGetInput()->button[SP_BUTTON_R] = 0;
		blink = 0;
		switch (line)
		{
			case 0: spPollKeyboardInput(shortName,256,SP_BUTTON_B_MASK); break;
			case 1: spPollKeyboardInput(longName,256,SP_BUTTON_B_MASK); break;
			case 2: spPollKeyboardInput(password,256,SP_BUTTON_B_MASK); break;
			case 3: spPollKeyboardInput(mail,256,SP_BUTTON_B_MASK); break;
		}
	}
	if ( spGetInput()->button[SP_BUTTON_L])
	{
		line = (line + 3) % 4;
		spGetInput()->button[SP_BUTTON_L] = 0;
		blink = 0;
		switch (line)
		{
			case 0: spPollKeyboardInput(shortName,256,SP_BUTTON_B_MASK); break;
			case 1: spPollKeyboardInput(longName,256,SP_BUTTON_B_MASK); break;
			case 2: spPollKeyboardInput(password,256,SP_BUTTON_B_MASK); break;
			case 3: spPollKeyboardInput(mail,256,SP_BUTTON_B_MASK); break;
		}
	}
	if ( spGetInput()->button[SP_BUTTON_START] )
	{
		spGetInput()->button[SP_BUTTON_START] = 0;
		if (mode == 0)
		{
			if (check_shortname())
				askMode = 2;
			else
			if (check_longname())
				askMode = 3;
			else
			if (check_password())
				askMode = 4;
			else
			{
				spInterpolateTargetToColor(0,3*SP_ONE/4);
				spFontDrawMiddle( screen->w/2, screen->h/2, 0, "Connecting to server...", font);
				spFlip();
				profile = spNetC4ACreateProfile(longName,shortName,password,mail);
				mode = 1;
			}
		}
		else
			spNetC4AEditProfile(profile,longName,shortName,password,mail);
	}
	if ( mode == 1 && spGetInput()->button[SP_BUTTON_X] )
	{
		 spGetInput()->button[SP_BUTTON_X] = 0;
		 askMode = 1;
	}
	if ( spGetInput()->button[SP_BUTTON_START] )
	{
		spGetInput()->button[SP_BUTTON_START] = 0;
		if (mode == 0)
		{
			profile = spNetC4ACreateProfile(longName,shortName,password,mail);
			mode = 1;
		}
		else
			spNetC4AEditProfile(profile,longName,shortName,password,mail);
	}
	return 0;
}

void resize(Uint16 w,Uint16 h)
{
	//Settings up the onboard keyboard:
	if (spGetSizeFactor() <= SP_ONE)
		spSetVirtualKeyboard(SP_VIRTUAL_KEYBOARD_ALWAYS,0,h-w*48/320,w,w*48/320,spLoadSurface("./images/keyboard320.png"),spLoadSurface("./images/keyboardShift320.png"));
	else
	if (spGetSizeFactor() <= 2*SP_ONE)
		spSetVirtualKeyboard(SP_VIRTUAL_KEYBOARD_ALWAYS,0,h-w*48/320,w,w*48/320,spLoadSurface("./images/keyboard640.png"),spLoadSurface("./images/keyboardShift640.png"));
	else
		spSetVirtualKeyboard(SP_VIRTUAL_KEYBOARD_ALWAYS,0,h-w*48/320,w,w*48/320,spLoadSurface("./images/keyboard1280.png"),spLoadSurface("./images/keyboardShift1280.png"));
  //Setup of the new/resized window
  spSelectRenderTarget(spGetWindowSurface());
  spSetPerspective(50.0,(float)spGetWindowSurface()->w/(float)spGetWindowSurface()->h,1.0,100);
	spFontShadeButtons(1);

	//Font Loading
	spFontSetShadeColor(BACKGROUND_COLOR);
	if (font)
		spFontDelete(font);
	font = spFontLoad(FONT_LOCATION,FONT_SIZE*spGetSizeFactor()>>SP_ACCURACY);
	spFontAdd(font,SP_FONT_GROUP_ASCII,65535);//whole ASCII
	spFontAddBorder(font,BACKGROUND_COLOR);
	spFontMulWidth(font,15<<SP_ACCURACY-4);
	spFontAddButton( font, 'A', SP_BUTTON_LEFT_NAME, spGetRGB(230,230,230), spGetRGB(64,64,64));
	spFontAddButton( font, 'B', SP_BUTTON_RIGHT_NAME, spGetRGB(230,230,230), spGetRGB(64,64,64));
	spFontAddButton( font, 'X', SP_BUTTON_DOWN_NAME, spGetRGB(230,230,230), spGetRGB(64,64,64));
	spFontAddButton( font, 'Y', SP_BUTTON_UP_NAME, spGetRGB(230,230,230), spGetRGB(64,64,64));
	spFontAddButton( font, 'L', SP_BUTTON_L_NAME, spGetRGB(230,230,230), spGetRGB(64,64,64));
	spFontAddButton( font, 'R', SP_BUTTON_R_NAME, spGetRGB(230,230,230), spGetRGB(64,64,64));
	spFontAddButton( font, 'S', SP_BUTTON_START_NAME, spGetRGB(230,230,230), spGetRGB(64,64,64));
	spFontAddButton( font, 'E', SP_BUTTON_SELECT_NAME, spGetRGB(230,230,230), spGetRGB(64,64,64));

	spFontSetShadeColor(BACKGROUND_COLOR);
	if (font_small)
		spFontDelete(font_small);
	font_small = spFontLoad(FONT_LOCATION,FONT_SIZE_SMALL*spGetSizeFactor()>>SP_ACCURACY);
	spFontAdd(font_small,SP_FONT_GROUP_ASCII,spGetRGB(192,192,192));//whole ASCII
	spFontAddBorder(font_small,BACKGROUND_COLOR);
	spFontMulWidth(font_small,15<<SP_ACCURACY-4);
	spFontAddButton( font_small, 'A', SP_BUTTON_LEFT_NAME, spGetRGB(230,230,230), spGetRGB(64,64,64));
	spFontAddButton( font_small, 'B', SP_BUTTON_RIGHT_NAME, spGetRGB(230,230,230), spGetRGB(64,64,64));
	spFontAddButton( font_small, 'X', SP_BUTTON_DOWN_NAME, spGetRGB(230,230,230), spGetRGB(64,64,64));
	spFontAddButton( font_small, 'Y', SP_BUTTON_UP_NAME, spGetRGB(230,230,230), spGetRGB(64,64,64));
	spFontAddButton( font_small, 'L', SP_BUTTON_L_NAME, spGetRGB(230,230,230), spGetRGB(64,64,64));
	spFontAddButton( font_small, 'R', SP_BUTTON_R_NAME, spGetRGB(230,230,230), spGetRGB(64,64,64));
	spFontAddButton( font_small, 'S', SP_BUTTON_START_NAME, spGetRGB(230,230,230), spGetRGB(64,64,64));
	spFontAddButton( font_small, 'E', SP_BUTTON_SELECT_NAME, spGetRGB(230,230,230), spGetRGB(64,64,64));
}

int main(int argc, char **argv)
{
	srand(time(NULL));
	spInitCore();
	spSetAffineTextureHack(0); //We don't need it :)
	spInitMath();
	screen = spCreateDefaultWindow();
	resize(screen->w,screen->h);
	
	spSetZSet(0);
	spSetZTest(0);
	banner = spLoadSurface("images/banner.png");

	spPollKeyboardInput(shortName,256,SP_BUTTON_B_MASK);
	
	profile = spNetC4AGetProfile();
	if (profile)
	{
		memcpy(longName,profile->longname,256);
		memcpy(shortName,profile->shortname,256);
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
