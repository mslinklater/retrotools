
; Assembler should use basic 6502 instructions
	processor 6502

; Some zero page data	
	org $0080

ZP_ONE
	.byte $01

	org $1000

Start_imm	lda	#$40
			brk

Start_zp	lda ZP_ONE
			brk