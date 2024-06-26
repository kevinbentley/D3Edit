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

#ifndef RADIOSITY_H
#define RADIOSITY_H

#include "pserror.h"
#include "pstypes.h"
#include "3d.h"
#include "gr.h"

// Shooting methods
#define SM_HEMICUBE 0
#define SM_RAYCAST 1
// This method makes a switch from RAYCAST to HEMICUBE after all satellites have
// been used for lightsources
#define SM_SWITCH_AFTER_SATELLITES 2

#define MAX_VOLUME_ELEMENTS 500

typedef struct {
  float r, g, b;
} spectra;

// element flags
#define EF_IGNORE 1
#define EF_SMALL 2 // Don't blend this one into the lightmap - it will corrupt!

typedef struct {
  vector *verts;
  spectra exitance;
  float area;
  ubyte num_verts;
  ubyte flags; // see above
} rad_element;

#define VEF_REVERSE_SHOOT 1

typedef struct {
  spectra color;
  vector pos;
  ubyte flags;
} volume_element;

#define ST_ROOM 0           // This is a room surface
#define ST_TERRAIN 1        // This is a terrain surface
#define ST_SATELLITE 2      // This is a sun on the terrain
#define ST_PORTAL 3         // This surface is a portal, don't consider it
#define ST_ROOM_OBJECT 4    // This surface is part of an object
#define ST_TERRAIN_OBJECT 5 // This surface is part of terrain object
#define ST_EXTERNAL_ROOM 6  // This surface is a room outside

#define SF_TOUCHES_TERRAIN 1
#define SF_LIGHTSOURCE 2

typedef struct {
  float area;
  spectra emittance;
  spectra exitance;
  float reflectivity; // how much light bounces off
  ubyte xresolution;  // how many elements (resolution x resolution) for this face
  ubyte yresolution;
  rad_element *elements; // list of elements for this surface
  vector normal;         // normal of this surface
  vector *verts;

  ubyte surface_type; // See ST_ types above

  int facenum; // facenumber of room
  int roomnum; // The roomnumber or terrain segment number
  ubyte num_verts;
  ubyte flags;

  float surface_area, element_area;

  ubyte x1, y1, x2, y2; // Where in the lightmap our bounds are
} rad_surface;

typedef struct {
  vector pos;
  ubyte code;

} rad_point;

extern float *Room_strongest_value[][4];

extern int Ignore_terrain;
extern int Ignore_satellites;

extern float Ignore_limit;
extern float rad_TotalFlux;
extern float rad_Convergence;

extern int rad_StepCount;
extern int rad_MaxStep;
extern int rad_DoneCalculating;

extern float rad_TotalUnsent;

extern rad_surface *rad_MaxSurface;

extern int rad_NumSurfaces;
extern int rad_NumElements;

extern float *rad_FormFactors;
extern rad_surface *rad_Surfaces;
extern volume_element *Volume_elements[];

extern int UseVolumeLights;
extern int Calculate_specular_lighting;

extern int Shoot_from_patch;

// Tells radiosity renderer to do volume lighting
extern int Do_volume_lighting;

int DoRadiosityRun(int method, rad_surface *light_surfaces, int count);
// Sets up our radiosity run
void InitRadiosityRun();

// Initializes the exitances for all surfaces
void InitExitance();

// Gets the spectral emittance for a surface
void GetEmittance(rad_surface *surf, spectra *dest);

// Sets all the elements of a surface to their initial exitance values
void SetExitanceForSurface(rad_surface *surf);

// Find the surface we want to shoot from
void UpdateUnsentValues();

// Does one iteration of ray-casting radiosity
int DoRadiosityIteration();

// Finds the world coordinate center of a element
void GetCenterOfElement(rad_element *ep, vector *dest);

// Finds the world coordinate center of a surface
void GetCenterOfSurface(rad_surface *ep, vector *dest);

// Returns 1 if a src vector can hit dest vector unobstructed
int ShootRayFromPoint(vector *src, vector *dest, rad_surface *src_surf, rad_surface *dest_surf);

// Shoots a ray from the center of the max surface to center of every other element
// Also updates the exitances of elements that get hit

void ShootMaxSurface();

// Does all the radiosity calculations until done
void CalculateRadiosity();

// returns the amount of unsent flux from a surface
float GetUnsentFlux(rad_surface *surface);

// Shuts down the radiosity stuff, freeing memory, etc
void CloseRadiosityRun();

// Initalizes memory for form factors
void SetupFormFactors();

// Calculates the area of the surfaces and elements in our environment
void CalculateArea();

// Counts the total number of elements we have to work with
void CountElements();

// Returns max color
float GetMaxColor(spectra *sp);

#endif
