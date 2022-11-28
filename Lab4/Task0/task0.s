segment .data
    format  db      "%d",0x0a,0
segment .text
        global  main            ; let the linker know about main
        extern  printf, puts         ; resolve printf from libc
    main:
        push    ebp             ; prepare stack frame for main
        mov     ebp, esp
        sub     esp, 8

        mov     edi, dword[ebp+8]    ; get argc into edi
        mov     esi, dword[ebp+12]   ; get first argv string into esi

    print_argc:
        xor     eax, eax
        push    dword edi     ; must dereference esi; points to argv
        push    format
        call    printf
    print_argv_0:
        xor     eax, eax
        push    dword [esi]     ; must dereference esi; points to argv
        call    puts

    end_loop:
        xor     eax, eax
        leave
        ret