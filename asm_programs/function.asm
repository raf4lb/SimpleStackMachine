; sum(x <int>, y <int>){
;     z <int>
;     z = 10  
;     ret x + y + z
; } int
; main(){
;     result <int>
;     result = sum(2, 3)
;     print(result)
; }

CALL .main
HLT

.sum
; function params
VAR sum__y
POPA $sum__y
VAR sum__x
POPA $sum__x

; function variables
VAR sum__z
PSHL 10
POPA $sum__z

PSH $sum__x
PSH $sum__y
ADD
PSH $sum__z
ADD
RET

.main
VAR result
PSHL 2
PSHL 3
CALL .sum
POPA $result
PSH $result
TOP
RET