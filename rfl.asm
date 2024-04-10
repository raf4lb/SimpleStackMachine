CALL .main
HLT
.main

VAR i
VAR sum
PSHL 0
POPA $sum
VAR condition
PSHL 0
POPA $condition

PSHL 0
POPA $i
.loop_0

PSH $condition
PSHL 0
CPG
PJIF .condition_false_0
PSH $sum
PSH $i
ADD

POPA $sum

JMP .out_0
.condition_false_0
PSH $sum
PSHL 1
ADD

POPA $sum

.out_0

PSH $i
PSHL 1
ADD
POPA $i
PSH $i
PSHL 10
CPGE
PJIF .loop_0

RET

