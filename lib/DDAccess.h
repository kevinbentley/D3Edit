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

#ifndef DDACCESS_H
#define DDACCESS_H

//	allows access to DD system information, such as OS specific info.

#define DD_ACCESS_RING

//	Win32 region of DD_ACCESS
//		Include the standard windows header since API calls are available in DD_ACCESS_RING
//		Define any additional messages too.

#if defined(WIN32)

//	Headers
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

//	Messages
#define WM_SYNCMOUSEACQUIRE (WM_USER + 0)

#endif // WIN32

#endif
