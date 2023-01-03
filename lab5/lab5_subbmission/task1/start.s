section .data
        CREATE EQU 64
        CLOSE EQU 6
        READONLY EQU 0
        WRITEONLY EQU 1
        OPEN EQU 5
        WRITE EQU 4
        READ EQU 3
        STDOUT EQU 1
        STDIN EQU 0
        isEncode: db 0
        in: dd 0
        out: dd 1
section .bss
        buffer: resb 1       
        flag_one: resb 2
        first: resb 4            ;points to the first encoding number
        cur: resb 4           ;points to the current encoding number
        file_name: resb 4
section .text
    global _start
    global system_call
_start:
    pop    dword ecx    ; ecx = argc
    mov    esi,esp      ; esi = argv
    ;; lea eax, [esi+4*ecx+4] ; eax = envp = (4*ecx)+esi+4
    mov     eax,ecx     ; put the number of arguments into eax
    shl     eax,2       ; compute the size of argv in bytes             ;????????????
    add     eax,esi     ; add the size to the address of argv 
    add     eax,4       ; skip NULL at the end of argv
    push    dword eax   ; char *envp[]
    push    dword esi   ; char* argv[]
    push    dword ecx   ; int argc

    call    main        

    mov     ebx,eax
    mov     eax,1
    int     0x80
    nop
        
system_call:
    push    ebp             ; Save caller state
    mov     ebp, esp
    sub     esp, 4          ; Leave space for local var on stack
    pushad                  ; Save some more caller state

    mov     eax, [ebp+8]    ; Copy function args to registers: leftmost...        
    mov     ebx, [ebp+12]   ; Next argument...
    mov     ecx, [ebp+16]   ; Next argument...
    mov     edx, [ebp+20]   ; Next argument...
    int     0x80            ; Transfer control to operating system
    mov     [ebp-4], eax    ; Save returned value...
    popad                   ; Restore caller state (registers)
    mov     eax, [ebp-4]    ; place returned value where caller can see it
    add     esp, 4          ; Restore caller state
    pop     ebp             ; Restore caller state
    ret                     ; Back to caller

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

main:
        push    ebp             ; calling convention
        mov     ebp, esp
        

        mov     edi, dword [ebp+8]
        mov     esi, dword [ebp+12]   ; get first argv string into esi

        add     esi, 4
        mov     eax, [esi]            ;holds the pointer to the first flag:
 
        ;CHECKING E
        cmp     word [eax], "+e"
        jne     NO_ENCODE
        mov     byte [isEncode], 1
 
        add     eax, 2                  ;make eax point at the first encoding val
        mov     dword [first], eax
        mov     dword [cur], eax
 
    NO_ENCODE:


    ;*****************CHECKING -I****************         
        add     esi, 4
        mov     eax, [esi]            ;holds the pointer to the second flag:
        
        cmp     word [eax], "-i"
        jne     NO_Input_File
        add     eax, 2 
        mov     dword [file_name], eax    

        
    Opening_File:
        mov     eax, OPEN
        mov     ebx, dword [file_name]
        mov     ecx, READONLY
        mov     edx, 0777
        int     0x80

        mov     [in], eax

        jmp     Done_Flags
    NO_Input_File:
        ;*****************CHECKING -O****************
        
        cmp     word [eax], "-o"
        jne     Done_Flags
        add     eax, 2 
        mov     dword [file_name], eax  

    Create_File_for_Output:
        mov     eax, OPEN
        mov     ebx, dword [file_name]
        mov     ecx, CREATE+WRITEONLY
        mov     edx, 0777
        int     0x80  

        mov     [out], eax
    Done_Flags:


    ;setting streams to esi and edi
    mov     esi, [in]
    mov     edi, [out]
    

    loop1: ;reading from STDIN
        mov     eax, READ
        mov     ebx, esi
        mov     ecx, buffer
        mov     edx, 1
        int     0x80            ;return value is the length of what we read
		
		cmp 	eax, 0			; end loop if read <= 0
		jle		done

        cmp     byte [isEncode], 0      ;if not +e then print unchanged
        je      Print
    Print_Encoded:
        ;encode char in buffer 
        mov     cl, [buffer]        ;buffer is a pointer to a cell which contains next char
        mov     edx, [cur]          ;cur is a pointer to a cell that has a pointer to current encoding number
        add     cl, byte [edx]      ;adding the ascci value of next encoding val to char in buffer
        sub     cl, '0'             ;minus '0' to convert ascci to integer
        mov     byte [buffer], cl   ;moving encoded value back to buffer

        ;DEALING WITH CYCLIC ENCODING    
        inc     dword [cur]
        mov     ebx, dword [cur]
        cmp     byte [ebx], 0x00
        je      Restore_Curr
        
        jmp     End_Cur_Check
    Restore_Curr:                   ;NOT WORKING ASK MATAN
        mov     ebx, [first]
        mov     [cur], ebx
    End_Cur_Check:

	Print:
        mov     edx, eax
        mov     eax, WRITE
        mov     ebx, edi
        mov     ecx, buffer
        int     0x80
		
		jmp		loop1
		
done:    
    Close_Files:
        mov     eax, CLOSE
        mov     ebx, [in]
        int     0x80
        mov     eax, CLOSE
        mov     ebx, [out]
        int     0x80
        
        mov    eax, 1
        mov    ebx, 0
        int    0x80



