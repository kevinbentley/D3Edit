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
 

#include "ddgrWin32.h"
#include "ddgrWin32GDI.h"
#include "ddgrWin32DX.h"
#include "mono.h"
#include "pserror.h"


/*	Surface Functions
*/

/*	input:
		sf->name is optional.
		sf->w, sf->h, sf->bpp are mandatory
		sf->type and sf->flags are mandatory.
	output:
		sf->obj = surface object.
		sf->locks = 0
*/
bool 	ddgr_surf_Create(ddgr_surface *sf)
{
	bool success;

	switch(LIB_DATA(subsystem))
	{
		case DDGR_GDIX_SUBSYSTEM:
		case DDGR_GDI_SUBSYSTEM: success = ddgr_gdi_surf_Create(sf); break;
		case DDGR_DX_SUBSYSTEM: success = ddgr_dx_surf_Create(sf); break;
		default:	Int3();
	}
				
	if (!success) {
		ddgr_FatalError("Failed to create surface <%s>", sf->name);
	}

	return success;
}


void	ddgr_surf_Destroy(ddgr_surface *sf)
{
	switch(LIB_DATA(subsystem))
	{
		case DDGR_GDIX_SUBSYSTEM:
		case DDGR_GDI_SUBSYSTEM: ddgr_gdi_surf_Destroy(sf); break;
		case DDGR_DX_SUBSYSTEM: ddgr_dx_surf_Destroy(sf); break;
		default:	Int3();
	}
}


/*	retrieves a pointer to surface memory.  allowed to lock one surface multiple times.
		ptr is the returned pointer to surface memory.  used to unlock surface also
		rowsize is the size in bytes of one row of memory.	
*/
bool	ddgr_surf_Lock(ddgr_surface *sf, void **ptr, int *rowsize)
{
	bool success;

	switch(LIB_DATA(subsystem))
	{
		case DDGR_GDIX_SUBSYSTEM:
		case DDGR_GDI_SUBSYSTEM: success = ddgr_gdi_surf_Lock(sf, ptr, rowsize); break;
		case DDGR_DX_SUBSYSTEM: success = ddgr_dx_surf_Lock(sf, ptr, rowsize); break;
		default:	Int3();
	}
				
	if (!success) {
		ddgr_FatalError("Failed to lock surface <%s>", sf->name);
	}

	return success;
}


bool	ddgr_surf_Unlock(ddgr_surface *sf, void *ptr)
{
	bool success;

	switch(LIB_DATA(subsystem))
	{
		case DDGR_GDIX_SUBSYSTEM:
		case DDGR_GDI_SUBSYSTEM: success = ddgr_gdi_surf_Unlock(sf, ptr); break;
		case DDGR_DX_SUBSYSTEM: success = ddgr_dx_surf_Unlock(sf, ptr); break;
		default:	Int3();
	}
				
	if (!success) {
		ddgr_FatalError("Failed to unlock surface <%s>", sf->name);
	}

	return success;
}


//	attaches an OS handle to a surface
void ddgr_surf_AttachHandle(ddgr_surface *sf, unsigned handle)
{
	switch(LIB_DATA(subsystem))
	{
		case DDGR_GDIX_SUBSYSTEM:
		case DDGR_GDI_SUBSYSTEM: ddgr_gdi_surf_AttachHandle(sf, handle); break;
		case DDGR_DX_SUBSYSTEM: ddgr_dx_surf_AttachHandle(sf, handle); break;
		default:	Int3();
	}
}


/*	initializes a video screen from the given input surface which specifies the dimensions
	of the screen. 
		sf->name is optional
		sf->w, sf->h are mandatory
		sf->bpp is recommended.  if set to BPP_DEFAULT, it uses the current display bit depth
		sf->type = SURFTYPE_VIDEOSCREEN
		sf->flags = 0 or SURFFLAG_BACKBUFFER.  surfaces with backbuffers allow rendering to
						them and display via flip.  monopage surfaces don't have this luxury.
*/
bool 	ddgr_surf_InitVideo(ddgr_surface *sf)
{
	bool success;

	ASSERT(sf->type == SURFTYPE_VIDEOSCREEN);

	switch(LIB_DATA(subsystem))
	{
		case DDGR_GDIX_SUBSYSTEM:
		case DDGR_GDI_SUBSYSTEM: success = ddgr_gdi_surf_InitVideo(sf); break;
		case DDGR_DX_SUBSYSTEM: success = ddgr_dx_surf_InitVideo(sf); break;
		default:	Int3();
	}
				
	if (!success) {
		ddgr_FatalError("Failed to initialize video screen <%s>", sf->name);
	}

	return success;
}


/*	close video reverses the operation of init video for that surface.  the display should stay
	the same, but no more operations may occur to that surface through this library.
*/
void	ddgr_surf_CloseVideo(ddgr_surface *sf)
{
	switch(LIB_DATA(subsystem))
	{
		case DDGR_GDIX_SUBSYSTEM:
		case DDGR_GDI_SUBSYSTEM: ddgr_gdi_surf_CloseVideo(sf); break;
		case DDGR_DX_SUBSYSTEM: ddgr_dx_surf_CloseVideo(sf); break;
		default:	Int3();
	}
}


/*	flips the buffers in a surface.  really only useful for video screens
*/
bool	ddgr_surf_FlipVideo(ddgr_surface *sf)
{
	bool success;

//	don't report error if there's no backbuffer.
	if(!(sf->flags & SURFFLAG_BACKBUFFER)) return true;

	switch(LIB_DATA(subsystem))
	{
		case DDGR_GDIX_SUBSYSTEM:
		case DDGR_GDI_SUBSYSTEM: success = ddgr_gdi_surf_FlipVideo(sf); break;
		case DDGR_DX_SUBSYSTEM: success = ddgr_dx_surf_FlipVideo(sf); break;
		default:	Int3();
	}
				
	if (!success) {
		ddgr_FatalError("Failed to flip surface <%s>", sf->name);
	}

	return success;
}


/*	graphic primatives
*/
void ddgr_surf_Clear(ddgr_surface *dsf, ddgr_color col, int l, int t, int w, int h)
{
	switch(LIB_DATA(subsystem))
	{
		case DDGR_GDIX_SUBSYSTEM:
		case DDGR_GDI_SUBSYSTEM: ddgr_gdi_surf_Clear(dsf,col,l,t,w,h); break;
		case DDGR_DX_SUBSYSTEM: ddgr_dx_surf_Clear(dsf,col,l,t,w,h); break;
		default:	Int3();
	}
}


bool ddgr_surf_Blt(ddgr_surface *dsf, int dx, int dy, ddgr_surface *ssf, int sx, int sy, int sw, int sh)
{
	bool success;

	switch(LIB_DATA(subsystem))
	{
		case DDGR_GDIX_SUBSYSTEM:
		case DDGR_GDI_SUBSYSTEM: success = ddgr_gdi_surf_Blt(dsf, dx,dy,ssf,sx,sy,sw,sh); break;
		case DDGR_DX_SUBSYSTEM: success = ddgr_dx_surf_Blt(dsf, dx,dy,ssf,sx,sy,sw,sh); break;
		default:	Int3();
	}
				
	if (!success) {
		ddgr_FatalError("Failed to blt surface <%s> to <%s>", ssf->name, dsf->name);
	}

	return success;
}


//	returns internal information about an ddgr_surface (only to low level libraries, on par with the ddgr
//	library).
void ddgr_surf_GetPrivateData(ddgr_surface *sf, bool *ddraw_surf, uint *object_ptr)
{
	switch (LIB_DATA(subsystem))
	{
		case DDGR_DX_SUBSYSTEM:
		{
			tDXSurface *bm = (tDXSurface *)sf->obj;
			*ddraw_surf = true;
			*object_ptr = (bm->backbuffer == true) ? (uint)(bm->lpddsback) : (uint)(bm->lpdds);
			break;
		}
		default:
			Int3();
	}
}


//	returns aspect ratio of current display.
float ddgr_GetAspectRatio()
{
	float aspect_ratio = (float)((3.0 * GetSystemMetrics(SM_CXSCREEN))/(4.0 * GetSystemMetrics(SM_CYSCREEN)));
	return aspect_ratio;
}


unsigned ddgr_GetDDrawObject() 
{
	switch (LIB_DATA(subsystem))
	{
		case DDGR_GDIX_SUBSYSTEM:
		case DDGR_GDI_SUBSYSTEM: return (unsigned)(GDI_DATA(lpDD));
		case DDGR_DX_SUBSYSTEM: return (unsigned)(DX_DATA(lpDD));
		default:
			Int3();
	}

	return 0;
}

