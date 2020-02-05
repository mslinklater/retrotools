ASSEMBLER=dasm

lda.prg: lda.asm
	$(ASSEMBLER) lda.asm -olda.prg -llda.lst -slda.sym

ldx.prg: ldx.asm
	$(ASSEMBLER) ldx.asm -oldx.prg -lldx.lst -sldx.sym

ldy.prg: ldy.asm
	$(ASSEMBLER) ldy.asm -oldy.prg -lldy.lst -sldy.sym

all: lda.prg ldx.prg ldy.prg
