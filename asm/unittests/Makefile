ASSEMBLER=dasm

lda.prg: lda.asm
	$(ASSEMBLER) lda.asm -olda.prg -llda.lst -slda.sym

ldx.prg: ldx.asm
	$(ASSEMBLER) ldx.asm -oldx.prg -lldx.lst -sldx.sym

ldy.prg: ldy.asm
	$(ASSEMBLER) ldy.asm -oldy.prg -lldy.lst -sldy.sym

txx.prg: txx.asm
	$(ASSEMBLER) txx.asm -otxx.prg -ltxx.lst -stxx.sym

clx.prg: txx.asm
	$(ASSEMBLER) clx.asm -oclx.prg -lclx.lst -sclx.sym

all: lda.prg ldx.prg ldy.prg txx.prg clx.prg
