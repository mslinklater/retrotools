
; Assembler should use basic 6502 instructions
	processor 6502

; Some zero page data	
	org $0080

ZP_ONE
	.byte $01

	org $1000

Start   lda	#$00
		sta $4000
		lda ZP_ONE
		sta $4001
		brk

 