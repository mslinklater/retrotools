ASSEMBLER=dasm

adc.prg: adc.asm
	$(ASSEMBLER) adc.asm -oadc.prg -ladc.lst -sadc.sym

clx.prg: clx.asm
	$(ASSEMBLER) clx.asm -oclx.prg -lclx.lst -sclx.sym

lda.prg: lda.asm
	$(ASSEMBLER) lda.asm -olda.prg -llda.lst -slda.sym

ldx.prg: ldx.asm
	$(ASSEMBLER) ldx.asm -oldx.prg -lldx.lst -sldx.sym

ldy.prg: ldy.asm
	$(ASSEMBLER) ldy.asm -oldy.prg -lldy.lst -sldy.sym

sex.prg: sex.asm
	$(ASSEMBLER) sex.asm -osex.prg -lsex.lst -ssex.sym

stx.prg: stx.asm
	$(ASSEMBLER) stx.asm -ostx.prg -lstx.lst -sstx.sym

txx.prg: txx.asm
	$(ASSEMBLER) txx.asm -otxx.prg -ltxx.lst -stxx.sym

all: adc.prg lda.prg ldx.prg ldy.prg sex.prg txx.prg clx.prg stx.prg
