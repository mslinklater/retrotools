
; Assembler should use basic 6502 instructions
	processor 6502

; Some code
	org $1000

Start_tax
	tax
	brk

Start_tay
	tay
	brk

Start_tsx
	tsx
	brk

Start_txa
	txa
	brk

Start_txs
	txs
	brk

Start_tya
	tya
	brk
