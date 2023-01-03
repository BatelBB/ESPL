segment .data
        format  db      "%d",0x0a,0
segment .text
        global  main            ; let the linker know about main
        extern  printf



main:
        push    ebp             ; calling convention
        mov     ebp, esp
        pushad                  ; Save some more caller state


        ;mov     edi, dword[ebp+8]    ; get argc into edi
        mov     edi, ebp
        add     edi, 8
        mov     esi, dword[ebp+12]   ; get first argv string into esi

    PRINT_ARGC:
        add     byte [edi], '0'
        mov     eax, 4          ;4 is write number
        mov     ebx, 1          ;1 is STDOUT number
        mov     ecx, edi       ;ecx = argc
        mov     edx, 2
        ;add     edx, 4          ; + 4 for argc
        int     0x80

        PRINT_ARGV:
        mov     eax, 4          ;4 is write number
        mov     ebx, 1          ;1 is STDOUT number
        mov     ecx, [esi]       ;ecx = argc
        mov     edx, 10
        
        int     0x80

        mov     esp, ebp        ; restore stack frame
        pop     ebp
        ret