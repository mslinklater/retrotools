
; Assembler should use basic 6502 instructions
	processor 6502

; Some code
	org $1000

Start_sec
	sec
	brk

Start_sed
	sed
	brk

Start_sei
	sei
	brk
