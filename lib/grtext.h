/*
 THE COMPUTER CODE CONTAINED HEREIN IS THE SOLE PROPERTY OF OUTRAGE
 ENTERTAINMENT, INC. ("OUTRAGE").  OUTRAGE, IN DISTRIBUTING THE CODE TO
 END-USERS, AND SUBJECT TO ALL OF THE TERMS AND CONDITIONS HEREIN, GRANTS A
 ROYALTY-FREE, PERPETUAL LICENSE TO SUCH END-USERS FOR USE BY SUCH END-USERS
 IN USING, DISPLAYING,  AND CREATING DERIVATIVE WORKS THEREOF, SO LONG AS
 SUCH USE, DISPLAY OR CREATION IS FOR NON-COMMERCIAL, ROYALTY OR REVENUE
 FREE PURPOSES.  IN NO EVENT SHALL THE END-USER USE THE COMPUTER CODE
 CONTAINED HEREIN FOR REVENUE-BEARING PURPOSES.  THE END-USER UNDERSTANDS
 AND AGREES TO THE TERMS HEREIN AND ACCEPTS THE SAME BY USE OF THIS FILE.
 COPYRIGHT 1996-2000 OUTRAGE ENTERTAINMENT, INC.  ALL RIGHTS RESERVED.
 */
 

#ifndef GRTEXT_H
#define GRTEXT_H

#include "pstypes.h"
#include "ddvid.h"

#define GRTEXT_BUFLEN				16384
#define GRTEXT_FORMAT_CHAR			0x2
#define GRTEXT_FORMAT_SCALAR		4					// value mulitplied to formatting value in string.

#define GRTEXTFLAG_SATURATE		1
#define GRTEXTFLAG_SHADOW			2

struct tFontTemplate;

extern int Grtext_spacing;

//	sets text clipping parameters
void grtext_SetParameters(int left, int top, int right, int bottom, int tabspace=4);

//	gets text clipping parameters
void grtext_GetParameters(int *left, int *top, int *right, int *bottom, int *tabspace);

//	 clears text buffer. doesn't render.
void grtext_Reset();

// sets the color for text
void grtext_SetColor(ddgr_color col);

//	returns color set by grtext_SetColor (does not count intra-string color changes)
ddgr_color grtext_GetColor();

//	sets fancy color for text
void grtext_SetFancyColor(ddgr_color col1, ddgr_color col2, ddgr_color col3, ddgr_color col4);

//	sets the alpha value for text
void grtext_SetAlpha(ubyte alpha);

//	gets font alpha
ubyte grtext_GetAlpha();

// toggles text saturation 
void grtext_SetFlags(int flags);

// sets the font for text
void grtext_SetFont(int font_handle);

//	sets font scale (1.0 = normal, 0.5 = 1/2, 2.0 = twice as large.
void grtext_SetFontScale(float scale);

// gets the current font
int grtext_GetFont();

// returns the height of a string in current font.
int grtext_GetTextHeight(const char *str);
#define grtext_GetHeight(_s) grtext_GetTextHeight(_s)

// returns height of text using a font template
int grtext_GetTextHeightTemplate(tFontTemplate *ft, const char *str);
#define grtext_GetHeightTemplate(_t, _s) grtext_GetTextHeightTemplate(_t,_s)

//	returns width of text in current font.
int grtext_GetTextLineWidth(const char *str);
#define grtext_GetLineWidth(_s) grtext_GetTextLineWidth(_s)

// returns width of text using a font template
int grtext_GetTextLineWidthTemplate(const tFontTemplate *ft, const char *str);
#define grtext_GetLineWidthTemplate(_t, _s) grtext_GetTextLineWidthTemplate(_t, _s)

//	puts a formatted string in the text buffer
void grtext_Printf(int x, int y, const char *fmt, ...);

//	puts a string on the buffer
void grtext_Puts(int x, int y, const char *str);

//	puts a character down
void grtext_PutChar(int x, int y, int ch);

//	puts a centered string in the text buffer.
void grtext_CenteredPrintf(int xoff, int y, const char *fmt, ...);

//	renders all text in buffer and clears buffer.  USUALLY CALL THIS TO RENDER TEXT
void grtext_Flush();

//	renders all text but DOESN'T flush buffer
void grtext_Render();


// gets character information for the current string
//		ch is a SINGLE CHARACTER
//		col is the color of the current string as reported through formatting, or GR_NULL if no info
//		x is the old x before call and the new potential adjusted x after the call.
//		newline if it's a newline.
//		line start x;
typedef struct tGetCharInfo
{
	int sx;										// this will not be modified. the initial x on the same line as the string

	ddgr_color col;							// these values will be modified
	int x, w;									// x of char, and width of char.
	bool newline;								// reached a newline?
	bool font_char;							// character exists in font?
	char ch;
}
tGetCharInfo;

const char *grtext_GetChar(const char *str, tGetCharInfo *ci);


//////////////////////////////////////////////////////////////////////////////
//	font functions!

// we can load a font template into this structure.  call grfont_FreeTemplate(tFontTemplate) to free memory here.
typedef struct tFontTemplate
{
	ushort min_ascii, max_ascii;
	ubyte *ch_widths;
	ubyte *kern_data;
	ubyte ch_height;
	ubyte ch_maxwidth;								// max width of character in font.
	bool proportional;								// is this font proportional? if so use array of widths, else use maxwidth
	bool uppercase;									// uppercase font?
	bool monochromatic;								// font is monochromatic?
	bool newstyle;										// new style 4444 font.
	bool ffi2;											// new font info added.

// ffi2 style (font file info 2)
	sbyte ch_tracking;								// global tracking for font.
}
tFontTemplate;


//	clears out font buffer.
void grfont_Reset();

//	returns a handle to a loaded font.
int grfont_Load(char *fname);

//	frees a loaded font
void grfont_Free(int handle);

// loads a font template
bool grfont_LoadTemplate(char *fname, tFontTemplate *ft);

// frees a font template
void grfont_FreeTemplate(tFontTemplate *ft);

//	returns a character's width
int grfont_GetCharWidth(int font, int ch);

//	returns a font's height
int grfont_GetHeight(int font);

//	returns a character's width
int grfont_GetKernedSpacing(int font, int ch1, int ch2);

//	returns a character's width
int grfont_GetKernedSpacingTemp(const tFontTemplate *ft, int ch1, int ch2);

// converts a key code to ascii version, and checks if in font.  converts lowercase to upper if font
// doesn't have a lowercase set.  useful alternative to ddio_KeyToAscii.
int grfont_KeyToAscii(int font, int key);

// returns the raw bitmap data for a character in a font, its width and height
// returned data should be in 565 hicolor format if (*mono) is false.  if (*mono) is true,
//	then a bitmask will be returned, and you should treat a bit as a pixel.
ushort *grfont_GetRawCharacterData(int font, int ch, int *w, int *h, bool *mono);


/////////////////////////////////////////////////////////////////////
//EDITING FUNCTIONS ONLY

// sets a template to a font, be careful.
bool grfont_SetTemplate(const char *pathname, const tFontTemplate *ft);

// sets a font's template without saving...
bool grfont_SetKerning(int font, ubyte *kern_data);

// sets a font's tracking
bool grfont_SetTracking(int font, int tracking);
int grfont_GetTracking(int font);


/////////////////////////////////////////////////////////////////////

//Init functions -- call before anything is rendered!
void grtext_Init(void);

#ifndef RELEASE
void grfont_Spew(int font, int x, int y);
#endif

typedef struct tFontFileInfo2		// to maintain compatibility with older fonts (64 bytes long)
{
	short tracking;
	char reserved[62];
}
tFontFileInfo2;

//	font data structure internal to library but available for font editors
typedef struct tFontFileInfo 
{
	short width, height;				// width of widest character and height of longest char
	short flags;						// flags used by the character renderer
	short baseline;					// pixels given to lowercase below script line start at baseline
	ubyte min_ascii;					// minimum ascii value used by font
	ubyte max_ascii;					// max ascii value used by the font
	short byte_width;					// width of a character in the font in bytes
	ubyte *raw_data;					// pixel, map data.
	ubyte **char_data;				// pointers to each character
	ubyte *char_widths;				// individual pixel widths of each character
	ubyte *kern_data;					// kerning information for specific letter combos

// FFI2 (newstyle) data
	tFontFileInfo2 ffi2;

//	misc.
	float brightness;					// this IS NOT in the file, but a part of the baseline element. (upper 8bits)
} tFontFileInfo;


#define FT_COLOR				1
#define FT_PROPORTIONAL		2
#define FT_KERNED				4
#define FT_GRADIENT			8
#define FT_FMT4444			16
#define FT_FFI2				32		// all fonts made after D3 should have this flag set.

#endif