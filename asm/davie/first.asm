	processor 6502
	include "vcs.h"
	include "macro.h"

	SEG
	ORG $F000

Reset

StartOfFrame
	; Start of vertical blank processing
	lda #0
	sta VBLANK

	lda #2
	sta VSYNC

	; 3 scanlines of VSYNC signal
	sta WSYNC
	sta WSYNC
	sta WSYNC