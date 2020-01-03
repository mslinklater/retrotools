
; Assembler should use basic 6502 instructions
	processor 6502
	
	org  $1000

Start   lda	#$00
		sta $4000
		brk

 