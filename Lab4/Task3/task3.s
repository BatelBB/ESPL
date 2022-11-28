global print_multi
global add_multi

segment .data
    x_struct: dd 5                      ;declaring struct
    x_num: db 0xaa, 1,2,0x44,0x4f

    y_struct: dd 6                      ;declaring struct
    y_num: db 0xaa, 1,2,3,0x44,0x4f

    format      db      "%02hhx",0x0a,0


segment .text
        global  main                   ; let the linker know about main
        extern  printf, malloc         ; resolve printf from libc

main:
        push    ebp             ; prepare stack frame for main
        mov     ebp, esp
        
        push    x_num           ;function call
        push    x_struct
        push    y_num
        push    y_struct           
        call    add_multi
        
        leave                   ;end main convention
        ret



add_multi:
        push    ebp             ; calling convention
        mov     ebp, esp

        push edi                ;save registers
        push ebx
        push esi
        push ecx
        push edx
        push eax

        mov     edi, [ebp+8]    ;edi = first arg: y_num
        mov     edi, [edi]

        inc     edi          ;ans->size = y_num + 1
        mov     eax, 4          ;eax is the destination for the multiplying, mult places* 4 bytes
        mul     edi             ;eax holds the result of multiplying        
        
        add     eax, 4
        push    eax
        ;push    format
        ;call    printf
        call malloc             ;eax shoul hold a pointer to allocated memmory


        ;carry, x_struct pointer, y_struct pointer, edi = ans->size
       
        mov     ebx, dword 0    ;carry
        mov     esi, [ebp + 12] ;y_struct ptr p->size
        mov     ecx, [ebp + 16] 
        mov     ecx, [ecx]      ;x_num
        mov     edx, [ebp + 20] ;x_struct ptr - q->size

    start_loop2:

        push    edi
        push    format
        call    printf

;        dec     edi             ; get next value to edi
;        cmp     esi, 0          ; when it hits 0, we're done
;        jnz     start_loop2      ; end // jump if not zero

    end_loop2:
       
        pop eax
        pop edx
        pop ecx
        pop esi                ;restore registers
        pop ebx
        pop edi
        mov     esp, ebp        ; restore stack frame
        pop     ebp
        ret                     ; return






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

