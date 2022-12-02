segment .data
        format  db      "%d",0x0a,0
segment .text
        global  main            ; let the linker know about main
        extern  printf, puts         ; resolve printf from libc
main:
        push    ebp             ; calling convention
        mov     ebp, esp
        mov     edi, dword[ebp+8]    ; get argc into edi
        mov     esi, dword[ebp+12]   ; get first argv string into esi
print_argc:
        push    dword edi             ; not sure about that
        push    format
        call    printf
start_loop:
        push    dword [esi]     ; must dereference esi; points to argv
        call    puts
        add     esi, 4          ; advance to the next pointer in argv
        dec     edi             ; decrement edi from argc to 0
        cmp     edi, 0          ; when it hits 0, we're done
        jnz     start_loop      ; end with NULL pointer
end_loop:
        leave
        ret