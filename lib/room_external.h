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

#ifndef _ROOM_EXTERNAL_H_
#define _ROOM_EXTERNAL_H_

#include "pstypes.h"
#include "vecmat_external.h"
#include "bnode.h"

#define MAX_FACES_PER_ROOM 3000  // max number of faces per room
#define MAX_VERTS_PER_ROOM 10000 // max vertices per room
#define MAX_VERTS_PER_FACE 64    // max vertices per face

// Face flags
// NOTE:  If you add a flag here, please check the function CopyFaceFlags()
#define FF_LIGHTMAP 0x0001        // Render this face with a lightmap on top
#define FF_VERTEX_ALPHA 0x0002    // This face has vertex alpha blending
#define FF_CORONA 0x0004          // This face has a lighting corona
#define FF_TEXTURE_CHANGED 0x0008 // The texture on this face has changed
#define FF_HAS_TRIGGER 0x0010     // This face has a trigger
#define FF_SPEC_INVISIBLE 0x0020  // This face needs to be not rendered during specularity pass
#define FF_FLOATING_TRIG 0x0040   // This face only exists as a floating trigger
#define FF_DESTROYED 0x0080       // This face has been blown up
#define FF_VOLUMETRIC 0x0100      // This face is a volumetric face
#define FF_TRIANGULATED 0x0200    // ??
#define FF_VISIBLE 0x0400         // This face is visible this frame (Valid only during render)
#define FF_NOT_SHELL 0x0800       // This face is not part of the room shell
#define FF_TOUCHED 0x1000         // This face has been touched by fvi_QuickDistFaceList
#define FF_GOALFACE 0x2000        // This face is a goal texture face
#define FF_NOT_FACING 0x4000      // This face is not facing us this frame (Valid only during render)
#define FF_SCORCHED 0x8000        // This face has one or more scorch marks
// NOTE:  If you add a flag here, please check the function CopyFaceFlags()

// UVLs for room verts
typedef struct roomUVL {
  float u, v; // texture coordinates
  float u2, v2;
  ubyte alpha; // alpha for this vertex
} roomUVL;

// an n-sided polygon used as part of a room or portal
typedef struct face {
  ushort flags;     // flags for this face (see above)
  ubyte num_verts;  // how many vertices in this face
  sbyte portal_num; // which portal this face is part of, or -1 if none

  short *face_verts;       // index into list of vertices for this face
  roomUVL *face_uvls;      // index into list of uvls for this face
  vector normal;           // the surface normal of this face
  short tmap;              // texture numbers for this face
  ushort lmi_handle;       // the lightmap info number for this face
  short special_handle;    // the index into the special_faces array
  ubyte renderframe;       // what frame this face was last rendered (for lighting)
  ubyte light_multiple;    // what multiple to times by
  vector min_xyz, max_xyz; // min & max extents of this face (for FVI)
} face;

// Portal flags
#define PF_RENDER_FACES 1        // render the face(s) in the portal
#define PF_RENDERED_FLYTHROUGH 2 // allow flythrough of rendered faces
#define PF_TOO_SMALL_FOR_ROBOT 4 // Too small for a robot to use for path following (like a small window)
#define PF_COMBINED 8            // This portal has been combined with another for rendering purposes
#define PF_CHANGED 16            // Used for multiplayer - this portal has been changed
#define PF_BLOCK 32
#define PF_BLOCK_REMOVABLE 64

// a connection between two rooms
typedef struct portal {
  int flags;         // flags for this portal
  short portal_face; // the face for this portal
  short croom;       // the room this portal connects to
  short cportal;     // the portal in croom this portal connects to
  short bnode_index;
  int combine_master; // For rendering combined portals
  vector path_pnt;    // Point used by the path system
} portal;

// Room flags
#define RF_FUELCEN 1                // room is a refueling center
#define RF_DOOR (1 << 1)            // a 3d door is here.
#define RF_EXTERNAL (1 << 2)        // this is an external room (i.e. a building)
#define RF_GOAL1 (1 << 3)           // this room is goal 1
#define RF_GOAL2 (1 << 4)           // this room is goal 2
#define RF_TOUCHES_TERRAIN (1 << 5) // this room should recieve lighting from satellites
#define RF_SORTED_INC_Y (1 << 6)    // Faces are sorted with increasing y
#define RF_GOAL3 (1 << 7)           // this room is goal 3
#define RF_GOAL4 (1 << 8)           // this room is goal 4
#define RF_FOG (1 << 9)             // this room is fogged
#define RF_SPECIAL1 (1 << 10)       // This room is a special room
#define RF_SPECIAL2 (1 << 11)       // This room is a special room
#define RF_SPECIAL3 (1 << 12)       // This room is a special room
#define RF_SPECIAL4 (1 << 13)       // This room is a special room
#define RF_SPECIAL5 (1 << 14)       // This room is a special room
#define RF_SPECIAL6 (1 << 15)       // This room is a special room
#define RF_MIRROR_VISIBLE (1 << 16) // The mirror is this room is visible
#define RF_TRIANGULATE (1 << 17)    // All the faces in this room should be drawn with triagulation on
#define RF_STROBE (1 << 18)         // This room strobes with pulse lighting
#define RF_FLICKER (1 << 19)        // This room flickers with pulse lighting
#define RFM_MINE                                                                                                       \
  ((1 << 20) | (1 << 21) | (1 << 22) | (1 << 23) |                                                                     \
   (1 << 24)) // Mine index of this room (we support up to 32 individual mines without a problem)
#define RF_INFORM_RELINK_TO_LG (1 << 25) // Informs the level goal system on player relinking to this room
#define RF_MANUAL_PATH_PNT (1 << 26)     // The room path_pnt has been set manually (i.e. by the designer)
#define RF_WAYPOINT (1 << 27)            // This room has a waypoint in it
#define RF_SECRET (1 << 28)              // This room is a secret room
#define RF_NO_LIGHT (1 << 29)            // This room does not get lit

#define GOALROOM (RF_GOAL1 | RF_GOAL2 | RF_GOAL3 | RF_GOAL4)

#define ROOM_NAME_LEN 19 // how long a room name can be (not counting null terminator)

struct doorway;

// the basic building-block of a Descent 3 level
typedef struct room {
  int flags; // various room flags

  int num_faces;   // how many poygons in this room
  int num_portals; // how many connections in this room
  int num_verts;   // how many verts in the room
  face *faces;     // pointer to list of faces
  portal *portals; // pointer to list of portals
  vector *verts;   // array of vertices for this room
  vector4 *verts4; // array of 16byte vertices for this room

  doorway *doorway_data;   // pointer to this room's doorway data, or NULL if not a doorway
  char *name;              // name of this room, or NULL
  int objects;             // index of first object in this room
  vector max_xyz, min_xyz; // for external room visibility checking

  float last_render_time; // Last time we rendered this room

  // Hierarchical bounding boxes for this room
  vector bbf_min_xyz;
  vector bbf_max_xyz;
  short num_bbf_regions;
  short pad1;
  short **bbf_list;
  short *num_bbf;
  vector *bbf_list_min_xyz;
  vector *bbf_list_max_xyz;
  ubyte *bbf_list_sector;

  bn_list bn_info;

  short wpb_index;           // world point buffer index - where this room starts
  ubyte pulse_time;          // each room can has a pulse time
  ubyte pulse_offset;        // each room has a timer offset for which it pulses
  vector wind;               // Wind vector for the room
  int ambient_sound;         // Index of ambient sound pattern for this room, or -1 if none
  short vis_effects;         // index of first visual effect in this room
  short mirror_face;         // Index of face that this room is to be mirrored by
  ubyte num_mirror_faces;    // Number of faces in this room that have the same texture as the mirror
  ushort *mirror_faces_list; // the list of faces in this room that have the same texture as the mirror
  float damage;              // The damage per second applied to players (& maybe others) in room

  vector path_pnt;      // Point used by the path system
  ubyte *volume_lights; // Pointer to memory for our volumetric lighting
  short volume_width;   // The dimensions of our volumetric room
  short volume_height;
  short volume_depth;
  float fog_depth;           // How far until fog is totally opaque
  float fog_r, fog_g, fog_b; // Fog color

  ubyte env_reverb;        // environmental reverb preset
  ubyte room_change_flags; // For multiplayer, detects what characteristics have to be sent
  ubyte damage_type;       // What type of damage this rooms does (for sound) if damage > 0
  ubyte used;              // is this room holding data?

} room;

#endif