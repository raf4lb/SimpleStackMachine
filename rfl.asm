CALL .main
HLT
.sum
VAR y
POPA $y
VAR x
POPA $x

PSH $x
PSH $y
ADD

RET

.main

VAR condition
PSHL 1
POPA $condition
VAR result
PSHL 5
POPA $result
PSHL 0
                PSHL 0
                CPG
                PJIF .condition_false_0
                JMP .condition_true_0
            .condition_true_0
                PSH $result
PSHL 1
CALL .sum

POPA $result

                JMP .out_0
            .condition_false_0
                PSH $result
PSHL 2
CALL .sum

POPA $result

            .out_0
            PSH $condition
PSHL 1
ADD

                PSHL 0
                CPG
                PJIF .condition_false_1
                JMP .condition_true_1
            .condition_true_1
                PSH $result
PSHL 3
CALL .sum

POPA $result

                JMP .out_1
            .condition_false_1
                PSH $result
PSHL 4
CALL .sum

POPA $result

            .out_1
            PSH $condition
PSHL 1
ADD

                PSHL 0
                CPG
                PJIF .condition_false_2
                JMP .condition_true_2
            .condition_true_2
                PSH $result
PSHL 5
CALL .sum

POPA $result

                JMP .out_2
            .condition_false_2
                PSH $result
PSHL 6
CALL .sum

POPA $result

            .out_2
            
RET

