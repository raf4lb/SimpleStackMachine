; Use a button to control a led
; https://wokwi.com/projects/392567753366512641

PSHL 32          ; push 0b1000000 to stack
POPA $ddrb       ; set DDRB pin 5 output

.loop

    ; PORTB = PORTB | (1 << PINB1)
    PSH $portb  ; push PORTB to stack
    PSHL 1      ; push 1 to stack
    PSHL 5      ; push 1 to stack
    LSH         ; bitwise LEFT SHIFT operation
    OR          ; bitwise OR operation
    POPA $portb  ; pop result to address 0x1 (PORTB)
    DLY 1000

    ; PORTB = PORTB & ~(1 << PINB1)
    PSH $portb  ; push PORTB to stack
    PSHL 1      ; push 1 to stack
    PSHL 5      ; push 1 to stack
    LSH         ; bitwise LEFT SHIFT operation
    NOT         ; bitwise NOT operation
    AND         ; bitwise AND operation
    POPA $portb ; pop result to address 0x1 (PORTB)
    DLY 1000
    
    JMP .loop   ; jump to .loop
