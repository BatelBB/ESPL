segment .data
    Mask:   dd 0x002d
    State:  dd 0x1234
    format      db      "0x%02hhx",0x0a,0


segment .text
    global  main                   ; let the linker know about main
    extern  printf

main:
    push    ebp             ; prepare stack frame for main
    mov     ebp, esp
    mov     esi, 20

BEGIN_LOOP:
    call    rand_num

    push    dword [State]
    push    format
    call    printf

    dec     esi

    cmp     esi,0
    jnz     BEGIN_LOOP
    leave
    ret
rand_num:
    push    ebp             ; prepare stack frame for main
    mov     ebp, esp

    mov     eax, [State]
    and     eax, [Mask]
    xor     eax, [State]
    shr     eax, 1
    mov     [State], eax

    mov     esp, ebp        ; restore stack frame
    pop     ebp
    ret                     ; return

