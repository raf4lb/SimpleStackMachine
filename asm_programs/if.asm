; main(){
;     condition <int>
;     condition = 1
;     result <int>
;     ? (condition){
;         result = 10
;     }
;     ! {
;         result = 20
;     }
; } void

CALL .main
HLT

.main
    VAR condition
    PSHL 0
    POPA $condition
    VAR result
    PSH $condition
    PSHL 1
    CPE
    PJIF .condition_false
    JMP .condition_true

.condition_true
    PSHL 10
    POPA $result
    JMP .out

.condition_false
    PSHL 20
    POPA $result
    JMP .out

.out
    RET