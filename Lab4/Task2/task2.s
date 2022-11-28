global x_size
global x_num
global print_multi

segment .data
    x_struct: dd 5                      ;declaring struct
    x_num: db 0xaa, 1,2,0x44,0x4f

    format      db      "%02hhx",0x0a,0


segment .text
        global  main            ; let the linker know about main
        extern  printf         ; resolve printf from libc

main:
        push    ebp             ; prepare stack frame for main
        mov     ebp, esp
        
        push    x_num           ;function call
        push    x_struct
        call    print_multi
        
        leave                   ;end main convention
        ret

print_multi:
        push    ebp             ; calling convention
        mov     ebp, esp

        push    esi             ;store registers before using them
        push    edi

        mov     esi, dword [ebp+8]      ;getting first agrument: x_num address
        mov     esi, [esi]              ;retrieving data from address// esi = x_num
        mov     edi, dword [ebp+12]     ;edi = x_struct



start_loop:
        push    dword [edi]     ;print x_struct[i]
        push    format
        call    printf


        dec     esi             ; decrement esi from size to 0
        inc     edi             ; get next value to edi
        cmp     esi, 0          ; when it hits 0, we're done
        jnz     start_loop      ; end
end_loop:
        pop     edi             ;restore registers
        pop     esi
        mov     esp, ebp        ; restore stack frame
        pop     ebp
        ret                     ; return


