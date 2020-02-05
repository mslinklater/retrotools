
; Assembler should use basic 6502 instructions
	processor 6502

; Some zero page data	
	org $0080

ZP_ZERO
	.byte $00
ZP_ONE
	.byte $01
ZP_TWO
	.byte $02

ZP_D0
	.word DATA_ZERO
ZP_D1
	.word DATA_ONE
ZP_D2
	.word DATA_TWO
ZP_D3
	.word DATA_THREE
ZP_DP
	.word PAGE_NEXT_ROOT

; Some non zero page data
	org $400

DATA_ZERO
	.byte $00
DATA_ONE
	.byte $01
DATA_TWO
	.byte $02
DATA_THREE
	.byte $03
	.byte $04

	org $5c0
PAGE_NEXT_ROOT
	.byte $00
	org $610
PAGE_NEXT
	.byte $24

; Some code
	org $1000

IMM_VALUE_NOFLAGS	= $40
IMM_VALUE_ZEROFLAG	= $00
IMM_VALUE_NEGFLAG	= $80

Start_imm	
			lda	#IMM_VALUE_NOFLAGS
			brk

Start_zero	
			lda	#IMM_VALUE_ZEROFLAG
			brk

Start_neg	
			lda	#IMM_VALUE_NEGFLAG
			brk

Start_zp	
			lda ZP_ONE
			brk

Start_zpx1	
			ldx #$ff
			lda ZP_ONE,x
			brk

Start_zpx2	
			ldx #$01
			lda ZP_ONE,x
			brk

Start_abs
			lda DATA_ONE
			brk

Start_absx1
			ldx #$01
			lda DATA_ONE,x
			brk
Start_absx2
			ldx #PAGE_NEXT-PAGE_NEXT_ROOT
			lda PAGE_NEXT_ROOT,x
			brk
Start_absy1
			ldy #$01
			lda DATA_ONE,y
			brk
Start_absy2
			ldy #PAGE_NEXT-PAGE_NEXT_ROOT
			lda PAGE_NEXT_ROOT,y
			brk

Start_indx
			ldx #ZP_D2-ZP_D0
			lda (ZP_D0,x)
			brk
Start_indy1
			ldy #$01
			lda (ZP_D3),y
			brk
Start_indy2
			ldy #PAGE_NEXT-PAGE_NEXT_ROOT
			lda (ZP_DP),y
			brk
