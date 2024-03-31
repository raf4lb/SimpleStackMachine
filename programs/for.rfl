; For example
; int i
; int max = 10
; for i = 0; i < max; i++
;   print i

VAR i
VAR max
VAR inc

.main
    PSHL 0
    POPA $i
    PSHL 10
    POPA $max
    PSHL 1
    POPA $inc
    
.for
    PSH $i
    TOP
    PSH $inc
    ADD
    POPA $i
    PSH $i
    PSH $max
    CPGE
    PJIF .for 
    HLT
