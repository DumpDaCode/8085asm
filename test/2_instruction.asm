;Normal Instructions

MOV A,B
MVI A,'H'
JMP
DATA: DB 05H
LXI H,2150H

; Instruction with irregular spaces
MOV    A, B
JMP
LXI   H,2150H

;Wrong Instructions

MOV A
JMP C,G
LXI H
