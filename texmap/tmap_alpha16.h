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
#ifdef ACTIVATE_INNER_LOOP

fix u = LeftU;
fix v = LeftV;
int cu, cv;
int srcptr;
ushort pix;
int texture_width = bm_w(Tex_bitmap, Current_mip);
ushort *src16 = bm_data(Tex_bitmap, Current_mip);

dest = (destptr + x1);

int i, cl;

for (i = 0; i < width; i++) {

  cu = FixToInt(u);
  cv = FixToInt(v);

  srcptr = (cv * texture_width) + cu;

  // read from screen
  pix = *dest;

  // get alpha
  cl = src16[srcptr];

  *dest++ = (TexShadeTable16[cl][pix >> 8]) + TexShadeTable8[cl][pix & 0xFF];
  u += dudx;
  v += dvdx;
}
#endif
