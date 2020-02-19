# 6502 Per cycle activity

Taken from visual6502.org

|Opcode|Mnemonic|AddrMode|Prefetch|Exe1|Exe2|Exe3|Exe4|Exe5|Exe6|Exe7|link|
|---|---|---|---|---|---|---|---|---|---|---|---|
|0x69|ADC|Immediate|[FetchPC++]|[Temp=PC++]|[[FetchPC++][Acc+=Temp]||||||http://www.visual6502.org/JSSim/expert.html?loglevel=2&a=0000&d=ea6901ea00|

LDA #$00	0x69 0x00
LDX #$00	0xa2 0x00
LDY #$00	0xa0 0x00
CLC			0x18
CLI			0x58
CLV			0xb8
CLD			0xd8
NOP			0xea

http://www.visual6502.org/JSSim/expert.html?loglevel=2&a=0000&d=a900a200a0001858b8d8ea