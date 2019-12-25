
; Assembler should use basic 6502 instructions
	processor 6502
	
	org  $1000

Start   lda	#$00
		brk
	
; Here we skip to address $FFFC and define a word with the
; address of where the CPU should start fetching instructions.
; This also fills out the ROM size to $1000 (4k) bytes
;	org $1100
