
; Assembler should use basic 6502 instructions
	processor 6502

; Some code
	org $1000

Start_clc
	clc
	brk

Start_cld
	cld
	brk

Start_cli
	cli
	brk

Start_clv
	clv
	brk
