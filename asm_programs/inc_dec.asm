; Count from 0 to 10 and from 10 to 0

VAR i
VAR MIN
VAR MAX

.main
    PSHL 0
    POPA $MIN
    PSHL 10
    POPA $MAX
    PSHL 0
    TOP
    POPA $i
    
.incr
    PSH $i
    PSHL 1
    ADD
    TOP
    POPA $i
    PSH $i
    PSH $MAX
    CPL
    PJIF .decr
    JMP .incr

.decr
    PSH $i
    PSHL 1
    SUB
    TOP
    POPA $i
    PSH $i
    PSH $MIN
    CPG
    PJIF .end 
    JMP .decr

.end
    HLT