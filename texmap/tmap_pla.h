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
			
			#define FIXUP32 012345678h
			
			dest=destptr+x1;
			fix fx_u,fx_v;
			ushort FPUCW,OldFPUCW;
					
			TexSrc=bm_data8(Tex_bitmap,Current_mip);
			Current_tex_palette=bm_palette (Tex_bitmap);
											
			float float_scaler=65536.0;
			float float_scaler8=4096.0;
			float one=1.0;
			float floattemp;

			float u_affine=fldudx*16;
			float v_affine=fldvdx*16;
			float z_affine=fldzdx*16;

			BigSteps=width/16;
			Leftovers=width%16;
			static int old_ander=-1;

			__asm 
			{
				; put the FPU in 32 bit mode
				fstcw   [OldFPUCW]                  ; store copy of CW
				mov     ax,OldFPUCW                 ; get it in ax
				and     eax, NOT 0x300			    ; 24 bit precision
				mov     [FPUCW],ax                  ; store it
				fldcw   [FPUCW]                     ; load the FPU

				mov     edi, dest				    ; edi = dest pointer
				mov		esi, TexSrc

				mov ecx, LeftMonoLight
				shr ecx, 8
								
				
				mov edx, Ander
				cmp edx, old_ander
				je StartCoords
				
				mov AFixer9-4, edx
				mov AFixer1-4, edx
				mov AFixer2-4, edx
				mov AFixer3-4, edx
				mov AFixer4-4, edx
				mov AFixer5-4, edx
				mov AFixer6-4, edx
				mov AFixer7-4, edx
				mov AFixer8-4, edx
				mov old_ander, edx
				
				StartCoords:
				    
				; calculate ULeft and VLeft         ; FPU Stack (ZL = ZLeft)
											        ; st0  st1  st2  st3  st4  st5  st6  st7
				fld     [FLeftV]					; V/ZL 
				fld     [FLeftU]		            ; U/ZL V/ZL 
				fld     [FLeftZ]		            ; 1/ZL U/ZL V/ZL 
				fld1                                ; 1    1/ZL U/ZL V/ZL 
				fdiv    st,st(1)                    ; ZL   1/ZL U/ZL V/ZL 
				fld     st                          ; ZL   ZL   1/ZL U/ZL V/ZL 
				fmul    st,st(4)                    ; VL   ZL   1/ZL U/ZL V/ZL 
				fxch    st(1)                       ; ZL   VL   1/ZL U/ZL V/ZL 
				fmul    st,st(3)                    ; UL   VL   1/ZL U/ZL V/ZL 

				fstp    st(5)                       ; VL   1/ZL U/ZL V/ZL UL	
				fstp    st(5)                       ; 1/ZL U/ZL V/ZL UL   VL

				; calculate right side OverZ terms  ; st0  st1  st2  st3  st4  st5  st6  st7

				fadd    [z_affine]			        ; 1/ZR U/ZL V/ZL UL   VL
				fxch    st(1)                       ; U/ZL 1/ZR V/ZL UL   VL
				fadd    [u_affine]			        ; U/ZR 1/ZR V/ZL UL   VL
				fxch    st(2)                       ; V/ZL 1/ZR U/ZR UL   VL
				fadd    [v_affine]		            ; V/ZR 1/ZR U/ZR UL   VL
    
				; calculate right side coords       ; st0  st1  st2  st3  st4  st5  st6  st7

				fld1                                ; 1    V/ZR 1/ZR U/ZR UL   VL
				fdiv    st,st(2)                    ; ZR   V/ZR 1/ZR U/ZR UL   VL
				fld     st                          ; ZR   ZR   V/ZR 1/ZR U/ZR UL   VL
				fmul    st,st(2)                    ; VR   ZR   V/ZR 1/ZR U/ZR UL   VL
				fxch    st(1)                       ; ZR   VR   V/ZR 1/ZR U/ZR UL   VL
				fmul    st,st(4)                    ; UR   VR   V/ZR 1/ZR U/ZR UL   VL
	
				cmp BigSteps,0                     ; check for any full spans
				je  DoLeftovers
    
				SpanLoop:

				; at this point the FPU contains    ; st0  st1  st2  st3  st4  st5  st6  st7
								                    ; UR   VR   V/ZR 1/ZR U/ZR UL   VL

			    ; convert left side coords

				fld     st(5)                       ; UL   UR   VR   V/ZR 1/ZR U/ZR UL   VL
				fmul    [float_scaler]		        ; UL16 UR   VR   V/ZR 1/ZR U/ZR UL   VL
				fistp   [fx_u]			            ; UR   VR   V/ZR 1/ZR U/ZR UL   VL
	
				fld     st(6)                       ; VL   UR   VR   V/ZR 1/ZR U/ZR UL   VL
				fmul    [float_scaler]              ; VL16 UR   VR   V/ZR 1/ZR U/ZR UL   VL
				fistp   [fx_v]				        ; UR   VR   V/ZR 1/ZR U/ZR UL   VL
		
				; calculate deltas                  ; st0  st1  st2  st3  st4  st5  st6  st7

			    fsubr   st(5),st                    ; UR   VR   V/ZR 1/ZR U/ZR dU   VL
				fxch    st(1)                       ; VR   UR   V/ZR 1/ZR U/ZR dU   VL
				fsubr   st(6),st                    ; VR   UR   V/ZR 1/ZR U/ZR dU   dV
				fxch    st(6)                       ; dV   UR   V/ZR 1/ZR U/ZR dU   VR
	
				fmul    [float_scaler8]             ; dV8  UR   V/ZR 1/ZR U/ZR dU   VR
				fistp   [dvdx]                      ; UR   V/ZR 1/ZR U/ZR dU   VR
	
			    fxch    st(4)                       ; dU   V/ZR 1/ZR U/ZR UR   VR
				fmul    [float_scaler8]             ; dU8  V/ZR 1/ZR U/ZR UR   VR
				fistp   [dudx]		                ; V/ZR 1/ZR U/ZR UR   VR

				; increment terms for next span     ; st0  st1  st2  st3  st4  st5  st6  st7
				; Right terms become Left terms---->; V/ZL 1/ZL U/ZL UL   VL

				fadd    [v_affine]		            ; V/ZR 1/ZL U/ZL UL   VL
				fxch    st(1)                       ; 1/ZL V/ZR U/ZL UL   VL
				fadd    [z_affine]			        ; 1/ZR V/ZR U/ZL UL   VL
				fxch    st(2)                       ; U/ZL V/ZR 1/ZR UL   VL
				fadd    [u_affine]		            ; U/ZR V/ZR 1/ZR UL   VL
				fxch    st(2)                       ; 1/ZR V/ZR U/ZR UL   VL
				fxch    st(1)                       ; V/ZR 1/ZR U/ZR UL   VL
	
				; calculate right side coords       ; st0  st1  st2  st3  st4  st5  st6  st7
		
				fld1	                            ; 1    V/ZR 1/ZR U/ZR UL   VL
				fdiv    st,st(2)                    ; ZR   V/ZR 1/ZR U/ZR UL   VL

				; set up affine registers

				; setup delta values
    
				mov ebx, dvdx
				shl ebx, 8
				mov eax, dudx
				shr eax, 8
				mov bx, ax					; ebx=DV:DU in 8:8 format

				; setup initial coordinates

				mov edx, fx_v
				shl edx, 8
				mov eax, fx_u
				shr eax, 8
				mov dx, ax					; edx=V:U in 8.8 format
    				
				mov MidSteps,2
				push    ebp                         ; free up another register
				mov     ebp, ebx
				mov		ebx,0

				; START OF BLITTING

					HereMe:
			
					mov		eax, edx					; eax=CV:CU in 8.8 format, edx=CV:CU
					shr		eax, 16						; ah=V integer
					mov     bh, ch						; save upper 8 bits of texel in ah
					mov     al, dh						; al=U integer
					and     eax, FIXUP32				; make sure our texture wraps
					AFixer1:mov bl, [esi+eax]			; get source texel
					mov     eax, Current_tex_palette
					add		edx, ebp					; add deltas to U and V			
					add     ecx, [Lighting8]
					mov     eax,[eax+ebx*2]
					mov     [edi], ax					; write out texel

					mov		eax, edx					; eax=CV:CU in 8.8 format, edx=CV:CU
					shr		eax, 16						; ah=V integer
					mov     bh, ch						; save upper 8 bits of texel in ah
					mov     al, dh						; al=U integer
					and     eax, FIXUP32				; make sure our texture wraps
					AFixer2:mov bl, [esi+eax]			; get source texel
					mov     eax, Current_tex_palette
					add		edx, ebp					; add deltas to U and V			
					add     ecx, [Lighting8]
					mov     eax,[eax+ebx*2]
					mov     [edi+2], ax					; write out texel


					mov		eax, edx					; eax=CV:CU in 8.8 format, edx=CV:CU
					shr		eax, 16						; ah=V integer
					mov     bh, ch						; save upper 8 bits of texel in ah
					mov     al, dh						; al=U integer
					and     eax, FIXUP32				; make sure our texture wraps
					AFixer3:mov bl, [esi+eax]			; get source texel
					mov     eax, Current_tex_palette
					add		edx, ebp					; add deltas to U and V			
					add     ecx, [Lighting8]
					mov     eax,[eax+ebx*2]
					mov     [edi+4], ax					; write out texel

					mov		eax, edx					; eax=CV:CU in 8.8 format, edx=CV:CU
					shr		eax, 16						; ah=V integer
					mov     bh, ch						; save upper 8 bits of texel in ah
					mov     al, dh						; al=U integer
					and     eax, FIXUP32				; make sure our texture wraps
					AFixer4:mov bl, [esi+eax]			; get source texel
					mov     eax, Current_tex_palette
					add		edx, ebp					; add deltas to U and V			
					add     ecx, [Lighting8]
					mov     eax,[eax+ebx*2]
					mov     [edi+6], ax					; write out texel

					mov		eax, edx					; eax=CV:CU in 8.8 format, edx=CV:CU
					shr		eax, 16						; ah=V integer
					mov     bh, ch						; save upper 8 bits of texel in ah
					mov     al, dh						; al=U integer
					and     eax, FIXUP32				; make sure our texture wraps
					AFixer5:mov bl, [esi+eax]			; get source texel
					mov     eax, Current_tex_palette
					add		edx, ebp					; add deltas to U and V			
					add     ecx, [Lighting8]
					mov     eax,[eax+ebx*2]
					mov     [edi+8], ax					; write out texel

					mov		eax, edx					; eax=CV:CU in 8.8 format, edx=CV:CU
					shr		eax, 16						; ah=V integer
					mov     bh, ch						; save upper 8 bits of texel in ah
					mov     al, dh						; al=U integer
					and     eax, FIXUP32				; make sure our texture wraps
					AFixer6:mov bl, [esi+eax]			; get source texel
					mov     eax, Current_tex_palette
					add		edx, ebp					; add deltas to U and V			
					add     ecx, [Lighting8]
					mov     eax,[eax+ebx*2]
					mov     [edi+10], ax				; write out texel

					mov		eax, edx					; eax=CV:CU in 8.8 format, edx=CV:CU
					shr		eax, 16						; ah=V integer
					mov     bh, ch						; save upper 8 bits of texel in ah
					mov     al, dh						; al=U integer
					and     eax, FIXUP32				; make sure our texture wraps
					AFixer7:mov bl, [esi+eax]			; get source texel
					mov     eax, Current_tex_palette
					add		edx, ebp					; add deltas to U and V			
					add     ecx, [Lighting8]
					mov     eax,[eax+ebx*2]
					mov     [edi+12], ax				; write out texel

					mov		eax, edx					; eax=CV:CU in 8.8 format, edx=CV:CU
					shr		eax, 16						; ah=V integer
					mov     bh, ch						; save upper 8 bits of texel in ah
					mov     al, dh						; al=U integer
					and     eax, FIXUP32				; make sure our texture wraps
					AFixer8:mov bl, [esi+eax]			; get source texel
					mov     eax, Current_tex_palette
					add		edx, ebp					; add deltas to U and V			
					add     ecx, [Lighting8]
					mov     eax,[eax+ebx*2]
					mov     [edi+14], ax				; write out texel
				
					add edi,16
					dec MidSteps
					jnz HereMe


					

				pop     ebp					                ; restore access to stack frame
    
				; the fdiv is done, finish right			; st0  st1  st2  st3  st4  st5  st6  st7
															; ZR   V/ZR 1/ZR U/ZR UL   VL

				fld     st					                ; ZR   ZR   V/ZR 1/ZR U/ZR UL   VL
				fmul    st,st(2)					        ; VR   ZR   V/ZR 1/ZR U/ZR UL   VL
				fxch    st(1)				                ; ZR   VR   V/ZR 1/ZR U/ZR UL   VL
				fmul    st,st(4)					        ; UR   VR   V/ZR 1/ZR U/ZR UL   VL
	
				//add     edi,16								; increment to next span
				dec     [BigSteps]				            ; decrement span count
				jnz     SpanLoop					        ; loop back

				DoLeftovers:

				cmp Leftovers,0
				je AllDone

				fld     st(5)                       ; UL   inv. inv. inv. inv. inv. UL   VL
			    fmul    [float_scaler]              ; UL16 inv. inv. inv. inv. inv. UL   VL
				fistp   [fx_u]		                ; inv. inv. inv. inv. inv. UL   VL

				fld     st(6)                       ; VL   inv. inv. inv. inv. inv. UL   VL
				fmul    [float_scaler]              ; VL16 inv. inv. inv. inv. inv. UL   VL
				fistp   [fx_v]                      ; inv. inv. inv. inv. inv. UL   VL

				dec     [Leftovers]			        ; calc how many steps to take
				jz      OnePixelSpan                ; just one, don't do deltas

				; calculate right edge coordinates  ; st0  st1  st2  st3  st4  st5  st6  st7
				; r -> R+1

			    ; @todo rearrange things so we don't need these two instructions
				fstp    [floattemp]                 ; inv. inv. inv. inv. UL   VL
				fstp    [floattemp]                 ; inv. inv. inv. UL   VL
		
				fld     FRightV		                ; V/Zr inv. inv. inv. UL   VL
				fsub    fldvdx			            ; V/ZR inv. inv. inv. UL   VL
				fld     FRightU		                ; U/Zr V/ZR inv. inv. inv. UL   VL
				fsub    fldudx				        ; U/ZR V/ZR inv. inv. inv. UL   VL
				fld     FRightZ				        ; 1/Zr U/ZR V/ZR inv. inv. inv. UL   VL
				fsub    fldzdx	  		            ; 1/ZR U/ZR V/ZR inv. inv. inv. UL   VL

				fdivr   [one]                       ; ZR   U/ZR V/ZR inv. inv. inv. UL   VL

				fmul    st(1),st                    ; ZR   UR   V/ZR inv. inv. inv. UL   VL
				fmulp   st(2),st                    ; UR   VR   inv. inv. inv. UL   VL

				; calculate deltas                  ; st0  st1  st2  st3  st4  st5  st6  st7
				fsubr   st(5),st                    ; UR   VR   inv. inv. inv. dU   VL
				fxch    st(1)                       ; VR   UR   inv. inv. inv. dU   VL
				fsubr   st(6),st                    ; VR   UR   inv. inv. inv. dU   dV
			    fxch    st(6)                       ; dV   UR   inv. inv. inv. dU   VR

				fidiv   [Leftovers]			        ; dv   UR   inv. inv. inv. dU   VR
				fmul    [float_scaler]              ; dv16 UR   inv. inv. inv. dU   VR
				fistp   [dvdx]                      ; UR   inv. inv. inv. dU   VR

				fxch    st(4)                       ; dU   inv. inv. inv. UR   VR
				fidiv   [Leftovers]                 ; du   inv. inv. inv. UR   VR
				fmul    [float_scaler]              ; du16 inv. inv. inv. UR   VR
				fistp   [dudx]                      ; inv. inv. inv. UR   VR

				; @todo gross!  these are to line up with the other loop
				fld     st(1)                       ; inv. inv. inv. inv. UR   VR
				fld     st(2)                       ; inv. inv. inv. inv. inv. UR   VR

				mov ebx, dvdx
				shl ebx, 8
				mov eax, dudx
				shr eax, 8
				mov bx, ax					; ebx=DV:DU in 8:8 format

				OnePixelSpan:

				; get starting coordinates for spane
				
				mov edx, fx_v
				shl edx, 8
				mov eax, fx_u
				shr eax, 8
				mov dx, ax					; edx=V:U in 8.8 format

				push ebp
				mov ebp, ebx
				mov ebx,0
				
				; leftover pixels loop
							
				LeftoverLoop:

				mov		eax, edx					; eax=CV:CU in 8.8 format, edx=CV:CU
				shr		eax, 16						; ah=V integer
				mov     bh, ch						; save upper 8 bits of texel in ah
				mov     al, dh						; al=U integer
				and     eax, FIXUP32				; make sure our texture wraps
				AFixer9:mov bl, [esi+eax]			; get source texel
				mov     eax, Current_tex_palette
				add		edx, ebp					; add deltas to U and V			
				add     ecx, [Lighting8]
				mov     eax,[eax+ebx*2]
				mov     [edi], ax				; write out texel
				
				add		edi,2
				dec     Leftovers	        ; decrement loop count
				jge		LeftoverLoop

				pop ebp
				

				AllDone:

			    ffree   st(0)
			    ffree   st(1)
				ffree   st(2)
				ffree   st(3)
				ffree   st(4)
				ffree   st(5)
				ffree   st(6)

				fldcw   [OldFPUCW]							; restore the FPU

			}
			
	
#endif