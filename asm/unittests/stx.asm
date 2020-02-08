
; Assembler should use basic 6502 instructions
	processor 6502

; zero page
	org $80
ZPNEG
	.byte 00
ZP0
	.byte $00
ZP1
	.byte $00
ZP_ADDR0
	.word DATA0
ZP_ADDR1
	.word DATA1
ZP_ADDR2
	.word DATA2

; normal data
	org 0400
DATA0
	.byte $00
DATA1
	.byte $00
DATA2
	.byte $00

; Some code
	org $1000

Start_sta_zp
	sta ZP0
	brk

Start_sta_zpx
	sta ZP0,x
	brk

Start_sta_abs
	sta DATA0
	brk

Start_sta_absx
	sta DATA0,x
	brk

Start_sta_absy
	sta DATA0,y
	brk

Start_sta_indx
	sta (ZP_ADDR1,x)
	brk

Start_sta_indy
	brk

