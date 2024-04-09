.loop
    CALL .move_top_pop_1
    DLY 1000
    CALL .move_top_pop_0
    DLY 1000
    JMP .loop

.move_top_pop_1
    PSHL 1
    TOP
    POP
    RET

.move_top_pop_0
    PSHL 0
    TOP
    POP
    RET