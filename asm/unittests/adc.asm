
; Assembler should use basic 6502 instructions
	processor 6502

; zero page data
	org $0080

ZP_0
	.byte $00
ZP_1
	.byte $01
ZP_80
	.byte $80
ZP_ff
	.byte $ff

; Some vars
	org $0400

DATA_0
	.byte $00
DATA_1
	.byte $01
DATA_80
	.byte $80
DATA_ff
	.byte $ff

DATA_ZP_0
	.word ZP_0
DATA_ZP_1
	.word ZP_1
DATA_ZP_80
	.word ZP_80
DATA_ZP_ff
	.word ZP_ff

; Some code
	org $1000

; --- Immediate ---

Start_adc_imm_01
	adc #$01
	brk

Start_adc_imm_80
	adc #$80
	brk

Start_adc_imm_ff
	adc #$ff
	brk

; --- Zero Page ---

Start_adc_zp_1
	adc ZP_1
	brk

Start_adc_zp_80
	adc ZP_80
	brk

Start_adc_zp_ff
	adc ZP_ff
	brk

; --- Zero Page, X ---

Start_adc_zp_1x
	adc ZP_1,X
	brk

Start_adc_zp_80x
	adc ZP_80,X
	brk

Start_adc_zp_ffx
	adc ZP_ff,X
	brk

; --- Absolute ---

Start_adc_abs_1
	adc DATA_1
	brk

Start_adc_abs_80
	adc DATA_80
	brk

Start_adc_abs_ff
	adc DATA_ff
	brk

; --- Absolute, X ---

; --- Absolute, Y ---

; --- Indirect, X ---

; --- Indirect, Y ---

