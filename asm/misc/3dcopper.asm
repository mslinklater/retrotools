; ===================================================================================
;
;	3D Copper Demo
;	D. Munsie 2003
;
;	Based on the 3D Copper demo for the GBA
;
; ===================================================================================

; ----------------------------------------------------------------------------------------------------
;
;	Revision History
;
;	06 Jul 03 - Implemented changes suggested by DEBRO to get 262 scan lines and insure 2600jr
;				compatibility
;	02 Jul 03 - Initial release
;
; ----------------------------------------------------------------------------------------------------

	processor 6502
	include vcs.h
	
; ----------------------------------------------------------------------------------------------------
;	RAM Variables
; ----------------------------------------------------------------------------------------------------

LineColor			= $80			; wow -- i'm pretty insane... i'm using over half of my RAM for a
									; color table... oh well, it is just a demo :)

Bar1Position		= $e0			; Middle line of bar 1
Bar2Position		= $e1			; Middle line of bar 2
Bar3Position		= $e2			; Middle line of bar 3
Bar4Position		= $e3			; Middle line of bar 4

Bar1Depth			= $e4			; Depth of bar 1
Bar2Depth			= $e5			; Depth of bar 2
Bar3Depth			= $e6			; Depth of bar 3
Bar4Depth			= $e7			; Depth of bar 4

Bar1Angle			= $e8			; Angle of bar 1
Bar2Angle			= $e9			; Angle of bar 2
Bar3Angle			= $ea			; Angle of bar 3
Bar4Angle			= $eb			; Angle of bar 4

Bar1Color			= $ec			; Color of bar 1
Bar2Color			= $ed			; Color of bar 2
Bar3Color			= $ee			; Color of bar 3
Bar4Color			= $ef			; Color of bar 4

TempVar				= $f0			; temporary storage space
BarTop				= $f1			; top of the bar currently being "drawn"
LineCount			= $f2			; number of lines left to "draw"
CurrentBar			= $f3			; current bar being "drawn"

; ----------------------------------------------------------------------------------------------------
;	Start of code
; ----------------------------------------------------------------------------------------------------

	seg
	org $f800

ROMStart:	sei						; disable interrupts
			cld						; turn off decimal mode
			ldx		#$ff			; setup the stack pointer
			txs
			lda		#0				; clear out zero page
ZeroMem:	pha						; by using the stack pointer
			dex						
			bne		ZeroMem
			pha						; get mem $00 as well, and reset the SP back to $ff

; ----------------------------------------------------------------------------------------------------
;	Variable initialization
; ----------------------------------------------------------------------------------------------------

			lda		#$c0
			sta		Bar1Color

			lda		#$80
			sta		Bar2Color

			lda		#$40
			sta		Bar3Color

			lda		#$f0
			sta		Bar4Color

			lda		#$0
			sta		Bar1Angle

			lda		#$40
			sta		Bar2Angle

			lda		#$80
			sta		Bar3Angle

			lda		#$c0
			sta		Bar4Angle

; ----------------------------------------------------------------------------------------------------
;	Main Loop
; ----------------------------------------------------------------------------------------------------

MainLoop:	jsr		Overscan		; start the overscan portion of the display
			jsr		UpdateBars		; update all the bar positions
			jsr		SortBars		; sort the bar depths
			jsr		VBlank			; make sure we do the vblank portion
			jsr		DrawBars		; update the color table
			jsr		Kernel			; draw everything out
			jmp		MainLoop		; let's do it all again
			
; ----------------------------------------------------------------------------------------------------
;	Update bar positions
; ----------------------------------------------------------------------------------------------------

UpdateBars:	ldy		#4				; 4 bars to update
			
UpdateLoop: dey						; move to the next bar
			ldx		Bar1Angle,Y		; get the bar angle
			dex						; move it
			stx		Bar1Angle,Y		; store it for next time

			lda		PositionTable,X	; get the postion
			sta		Bar1Position,Y

			lda		DepthTable,X	; get the depth
			sta		Bar1Depth,Y
			
			cpy		#0
			bne		UpdateLoop		; done?
			
			rts

; ----------------------------------------------------------------------------------------------------
;	Sort bar depths
;
;	This routine sorts the depths so that when the bars are "drawn", the ones with a smaller depth
;	are drawn first.  It does this by comparing depth 4 to depths 1-3, followed by depth 3 to depths 1-2,
;	and depth2 to depth 1.  After doing this, the bars will be sorted.
; ----------------------------------------------------------------------------------------------------

SortBars:	ldx		#4				; start comparin

OuterSort:	dex						; move to the next depth to check
			beq		ExitSort		; is this the end?
			txa						; move over the current depth for the inner loop
			tay
			
InnerSort:	dey						; check the next depth
			lda		Bar1Depth,x		; is this depth
			cmp		Bar1Depth,y		; less than this one?
			bcc		NoSwap			; if so, we don't need to swap

									; swap the depths
			sta		TempVar			; we already have Bar1Depth,x
			lda		Bar1Depth,y
			sta		Bar1Depth,x
			lda		TempVar
			sta		Bar1Depth,y

			lda		Bar1Color,x		; swap the colors
			sta		TempVar
			lda		Bar1Color,y
			sta		Bar1Color,x
			lda		TempVar
			sta		Bar1Color,y

			lda		Bar1Position,x	; swap the position
			sta		TempVar
			lda		Bar1Position,y
			sta		Bar1Position,x
			lda		TempVar
			sta		Bar1Position,y

			lda		Bar1Angle,x		; swap the angle
			sta		TempVar
			lda		Bar1Angle,y
			sta		Bar1Angle,x
			lda		TempVar
			sta		Bar1Angle,y
			
NoSwap:		cpy		#0				; did we just compare against the last depth?
			bne		InnerSort		; nope -- go back and compare some more
			beq		OuterSort		; yep -- go back to the outer loop

ExitSort:	rts

; ----------------------------------------------------------------------------------------------------
;	Update color table
;
;	This routine draws out the copper bars in order of depth from back to front.  For each bar
;	it sets the appropriate number of lines for the depth and position to the color
; ----------------------------------------------------------------------------------------------------

DrawBars:	ldx		#95				; we need to blank out the colors first
			lda		#1
EraseLoop:	sta		LineColor,X		; color be gone!
			dex
			bne		EraseLoop		; done?
			sta		LineColor,X

			ldx		#4				; start with the fourth bar
			stx		CurrentBar
			
BarsLoop:	ldx		CurrentBar		; load in the current bar
			dex						; adjust the pointer
			jsr		DrawBar
			dec		CurrentBar		; move to the next bar
			bne		BarsLoop		; done?
		
			rts

; ----------------------------------------------------------------------------------------------------
;	Draw Bar
;
;	Draws a single bar -- X points to the offset in the bar table to be drawn
; ----------------------------------------------------------------------------------------------------

DrawBar:	lda		Bar1Depth,X		; first, we calculate the position of the top line
			lsr						; divide by 2
			sta		TempVar			; save it off
			lda		Bar1Position,X	; get the position of the bar
			sbc		TempVar			; subtract the depth / 2
			sta		BarTop			; save off the position

			lda		Bar1Color,X		; get the bar color
			sta		TempVar			; and save it for later

			lda		Bar1Depth,X		; get the number of lines we need to draw
			sta		LineCount		; store it off for later
			tax
			dex						; subtract one since the table is 0-15
			lda		LumLookup,X		; get the offset into the lum table
			tax						; and move it to the index register
			ldy		BarTop			; get the position of the bar
DrawBarLoop:
			lda		TempVar			; get the line color
			adc		Lum16Table,X	; add the lumanance
			sta		LineColor,Y		; save off the color
			iny						; move to the next line
			inx						; move to the next color
			dec		LineCount		; subtract a line off our count
			bne		DrawBarLoop		; done yet?
			
			rts						; done

; ----------------------------------------------------------------------------------------------------
;	Vertical Blanking Code
; ----------------------------------------------------------------------------------------------------

VBlank:		lda		INTIM			; wait for the overscan to be done
			bne		VBlank
			ldx		#0				; set everything up
			lda		#2
			sta		WSYNC
			sta		VSYNC			; start the vsync
			sta		WSYNC			; first vsync line
			sta		WSYNC			; second vsync line
			
			sta		WSYNC			; third vsync line
			stx		VSYNC			; turn off vsync
			
			lda		#44				; setup the timer
			sta		TIM64T

			rts
			
; ----------------------------------------------------------------------------------------------------
;	Kernel code
; ----------------------------------------------------------------------------------------------------

Kernel:		lda		INTIM			; wait until it's time to start drawing
			bne		Kernel
			sta		WSYNC			; we're ready to start
			sta		VBLANK			; turn the display back on

; lines 0-47 -- blank at this time
			ldx		#48				; 48 blank lines
TopLoop:	sta		WSYNC			; move to the next line
			dex						; update line pointer
			bne		TopLoop

; lines 48-142 -- rotating copper bars and "ATARI 2600" text
;
; background color is stored in RAM, playfield color is based off background color.  Playfield is stored in
; table at the end of the ROM.  We draw the first half of the screen at the end of the prior line, and the
; other half between when the playfield was being displayed on the left and before it's displayed on the right
; In other words, typical 2600 kernel code :)

			ldx		#95				; (2 = 2) 95 lines with the copper bars in them
			nop		$00				; (3 = 5) just making sure the cycle counts are always the same
BarLoop:	ldy		$80,X			; (4 = 9) get the background color
			sty		COLUBK			; (3 = 12)
			iny						; (2 = 14) get the playfield color
			sty		COLUPF			; (3 = 17)

			lda		PF2FirstTable,X ; (4 = 21) get the value for PF2 on the left
			sta		PF2				; (3 = 24)

			; safe to write to PF0 after cycle 28
			lda		PF0SecondTable,X; (4 = 28) get the value for PF0
			sta		PF0				; (3 = 31)

			; safe to write to PF1 after cycle 39
			lda		PF1SecondTable,X ; (4 = 35) get the value for PF1
			nop						; (2 = 37)
			sta		PF1				; (3 = 40)

			; safe to write to PF2 after cycle 50
			lda		PF2SecondTable,X ; (4 = 44) get the value for PF2
			nop		$00				; (3 = 47)
			sta		PF2				; (3 = 50)

			; safe to write to PF0 after cycle 55
			lda		PF0FirstTable,X	; (4 = 54) get the value for PF0 on the left side
			sta		PF0				; (3 = 57)

			; safe to write to PF1 after cycle 65
			lda		PF1FirstTable,X	; (4 = 61) get the value for PF1
			nop						; (2 = 63)
			sta		PF1				; (3 = 66)

			; write to PF2 on the next line since we have the time to do it then
			sta		WSYNC			; (3 = 0) finish out this line, move to the next
			dex						; (2 = 2) update our line count
			bne		BarLoop			; (3 = 5) done?
		
			stx		COLUBK			; set the background back to black

; lines 143-191 -- blank at this time
			ldx		#49				; 49 blank lines
BottomLoop:	sta		WSYNC			; move to the next line
			dex						; update line pointer
			bne		BottomLoop		; done?
			
			lda		#2				; start up the vblank
			sta		VBLANK
			
			rts						; we're done for now

; ----------------------------------------------------------------------------------------------------
;	Overscan code
; ----------------------------------------------------------------------------------------------------

Overscan:	lda		#35				; set the timer to go off in 30 lines
			sta		TIM64T
			rts

; ----------------------------------------------------------------------------------------------------
;	End of code
; ----------------------------------------------------------------------------------------------------

; lumanance tables for the copper bars -- one table for each depth
Lum16Table:	.byte	$01, $03, $05, $07, $09, $0b, $0d, $0d, $0d, $0d, $0b, $09, $07, $05, $03, $01
Lum14Table:	.byte	$01, $03, $05, $07, $09, $0b, $0d, $0d, $0b, $09, $07, $05, $03, $01
Lum12Table:	.byte	$01, $03, $05, $07, $09, $0b, $0b, $09, $07, $05, $03, $01
Lum10Table:	.byte	$01, $03, $05, $07, $09, $09, $07, $05, $03, $01
Lum8Table:	.byte	$01, $03, $05, $07, $07, $05, $03, $01
Lum6Table:	.byte	$01, $03, $05, $05, $03, $01

; master lookup table -- the code uses this table to find the individual table above
LumLookup:	.byte	0, 0, 0, 0, 0, 60, 0, 52, 0, 42, 0, 30, 0, 16, 0, 0

; position for the bar, indexed by angle
PositionTable:
	.byte $30, $30, $31, $32, $33, $34, $35, $36, $37, $38, $39, $3a, $3b, $3c, $3d, $3e 
	.byte $3f, $40, $41, $41, $42, $43, $44, $45, $46, $47, $47, $48, $49, $4a, $4a, $4b 
	.byte $4c, $4c, $4d, $4e, $4e, $4f, $50, $50, $51, $51, $52, $52, $53, $53, $54, $54 
	.byte $54, $55, $55, $55, $56, $56, $56, $57, $57, $57, $57, $57, $57, $57, $57, $57 
	.byte $58, $57, $57, $57, $57, $57, $57, $57, $57, $57, $56, $56, $56, $55, $55, $55 
	.byte $54, $54, $54, $53, $53, $52, $52, $51, $51, $50, $50, $4f, $4e, $4e, $4d, $4c 
	.byte $4c, $4b, $4a, $4a, $49, $48, $47, $47, $46, $45, $44, $43, $42, $41, $41, $40 
	.byte $3f, $3e, $3d, $3c, $3b, $3a, $39, $38, $37, $36, $35, $34, $33, $32, $31, $30 
	.byte $30, $2f, $2e, $2d, $2c, $2b, $2a, $29, $28, $27, $26, $25, $24, $23, $22, $21 
	.byte $20, $1f, $1e, $1e, $1d, $1c, $1b, $1a, $19, $18, $18, $17, $16, $15, $15, $14 
	.byte $13, $13, $12, $11, $11, $10, $0f, $0f, $0e, $0e, $0d, $0d, $0c, $0c, $0b, $0b 
	.byte $0b, $0a, $0a, $0a, $09, $09, $09, $08, $08, $08, $08, $08, $08, $08, $08, $08 
	.byte $08, $08, $08, $08, $08, $08, $08, $08, $08, $08, $09, $09, $09, $0a, $0a, $0a 
	.byte $0b, $0b, $0b, $0c, $0c, $0d, $0d, $0e, $0e, $0f, $0f, $10, $11, $11, $12, $13 
	.byte $13, $14, $15, $15, $16, $17, $18, $18, $19, $1a, $1b, $1c, $1d, $1e, $1e, $1f 
	.byte $20, $21, $22, $23, $24, $25, $26, $27, $28, $29, $2a, $2b, $2c, $2d, $2e, $2f 

DepthTable:
	.byte $10, $0e, $0e, $0e, $0e, $0e, $0e, $0e, $0e, $0e, $0e, $0e, $0e, $0e, $0e, $0e 
	.byte $0e, $0e, $0e, $0e, $0e, $0e, $0e, $0e, $0e, $0e, $0e, $0e, $0e, $0e, $0e, $0e 
	.byte $0e, $0e, $0e, $0e, $0e, $0e, $0c, $0c, $0c, $0c, $0c, $0c, $0c, $0c, $0c, $0c 
	.byte $0c, $0c, $0c, $0c, $0c, $0c, $0c, $0c, $0a, $0a, $0a, $0a, $0a, $0a, $0a, $0a 
	.byte $0a, $0a, $0a, $0a, $0a, $0a, $0a, $0a, $0a, $08, $08, $08, $08, $08, $08, $08 
	.byte $08, $08, $08, $08, $08, $08, $08, $08, $08, $08, $08, $06, $06, $06, $06, $06 
	.byte $06, $06, $06, $06, $06, $06, $06, $06, $06, $06, $06, $06, $06, $06, $06, $06 
	.byte $06, $06, $06, $06, $06, $06, $06, $06, $06, $06, $06, $06, $06, $06, $06, $06 
	.byte $06, $06, $06, $06, $06, $06, $06, $06, $06, $06, $06, $06, $06, $06, $06, $06 
	.byte $06, $06, $06, $06, $06, $06, $06, $06, $06, $06, $06, $06, $06, $06, $06, $06 
	.byte $06, $06, $06, $06, $06, $06, $08, $08, $08, $08, $08, $08, $08, $08, $08, $08 
	.byte $08, $08, $08, $08, $08, $08, $08, $08, $0a, $0a, $0a, $0a, $0a, $0a, $0a, $0a 
	.byte $0a, $0a, $0a, $0a, $0a, $0a, $0a, $0a, $0a, $0c, $0c, $0c, $0c, $0c, $0c, $0c 
	.byte $0c, $0c, $0c, $0c, $0c, $0c, $0c, $0c, $0c, $0c, $0c, $0e, $0e, $0e, $0e, $0e 
	.byte $0e, $0e, $0e, $0e, $0e, $0e, $0e, $0e, $0e, $0e, $0e, $0e, $0e, $0e, $0e, $0e 
	.byte $0e, $0e, $0e, $0e, $0e, $0e, $0e, $0e, $0e, $0e, $0e, $0e, $0e, $0e, $0e, $0e 

; tables for the text in the middle of the display
	org $fd00
PF0FirstTable:
	.byte $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00
	.byte $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $20, $20, $20, $20
	.byte $20, $20, $20, $20, $20, $20, $20, $20, $20, $20, $20, $20, $e0, $e0, $e0, $e0
	.byte $e0, $e0, $e0, $e0, $20, $20, $20, $20, $20, $20, $20, $20, $c0, $c0, $c0, $c0
	.byte $c0, $c0, $c0, $c0, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00
	.byte $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00

PF1FirstTable:
	.byte $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00
	.byte $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $a9, $a9, $a9, $a9
	.byte $a9, $a9, $a9, $a9, $a9, $a9, $a9, $a9, $a9, $a9, $a9, $a9, $af, $af, $af, $af
	.byte $af, $af, $af, $af, $a9, $a9, $a9, $a9, $a9, $a9, $a9, $a9, $76, $76, $76, $76
	.byte $76, $76, $76, $76, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00
	.byte $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00

	org $fe00
PF2FirstTable:		; this table is offset by one because it is used one line later than the others
	.byte $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00
	.byte $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $52, $52, $52, $52, $52
	.byte $52, $52, $52, $52, $52, $52, $52, $52, $52, $52, $52, $4e, $4e, $4e, $4e, $4e
	.byte $4e, $4e, $4e, $52, $52, $52, $52, $52, $52, $52, $52, $4e, $4e, $4e, $4e, $4e
	.byte $4e, $4e, $4e, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00
	.byte $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00

PF0SecondTable:
	.byte $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00
	.byte $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $c0, $c0, $c0, $c0
	.byte $c0, $c0, $c0, $c0, $80, $80, $80, $80, $80, $80, $80, $80, $00, $00, $00, $00
	.byte $00, $00, $00, $00, $40, $40, $40, $40, $40, $40, $40, $40, $80, $80, $80, $80
	.byte $80, $80, $80, $80, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00
	.byte $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00

	org $ff00
PF1SecondTable:
	.byte $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00
	.byte $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $cc, $cc, $cc, $cc
	.byte $cc, $cc, $cc, $cc, $12, $12, $12, $12, $12, $12, $12, $12, $9e, $9e, $9e, $9e
	.byte $9e, $9e, $9e, $9e, $50, $50, $50, $50, $50, $50, $50, $50, $8c, $8c, $8c, $8c
	.byte $8c, $8c, $8c, $8c, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00
	.byte $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00

PF2SecondTable:
	.byte $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00
	.byte $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $22, $22, $22, $22
	.byte $22, $22, $22, $22, $55, $55, $55, $55, $55, $55, $55, $55, $55, $55, $55, $55
	.byte $55, $55, $55, $55, $55, $55, $55, $55, $55, $55, $55, $55, $22, $22, $22, $22
	.byte $22, $22, $22, $22, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00
	.byte $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00

	org $fffc
	.word ROMStart
	.word ROMStart
	end
