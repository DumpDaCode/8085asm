;Assembly directives

SIZE: DW '012D'
ORG 1055H
ARRAY: DB 05H, 04H, 03H, 02H, 01H
RAJIV MACRO p1,p2,p3
MOV p3,p2
ENDM
WIRJASNA MACRO p1,p2,p3
MOV p3,p2
ENDM
RAJIV: MACRO A,B,C
RANJAN: MACRO B,C,A
;Wrong syntax

DAA: EQU 1001H
SIZE DB 05
END
