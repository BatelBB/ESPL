section .data
        WRITE EQU 4
        READ EQU 3
        STDOUT EQU 1
        STDIN EQU 0
        isEncode: db 0
        counter: db 0        ;index of encoding number
        encoderFlag: db "+"
section .bss
        buffer: resb 1       ;1000 bytes buffer?
        flag_one: resb 2
        first: resb 4            ;points to the first encoding number
        cur: resb 4           ;points to the current encoding number
        size: resb 4             ;size of encoding cycle
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
 
        cmp     word [eax], "+e"
        jne     NO_ENCODE
        mov     byte [isEncode], 1
 
        add     eax, 2                  ;make eax point at the first encoding val
        mov     dword [first], eax
 
        call    my_strlen
        mov     ecx, eax
 
 
    NO_ENCODE:


    ;checking encode...
    ; cmp     byte [isEncode], 0
    ; je     PrintNotEqual
    ; Print0:
    ;     ;mov     edx, eax
    ;     mov     eax, WRITE
    ;     mov     ebx, STDOUT
    ;     mov     ecx,  dword [esi]
    ;     mov     edx, 1
    ;     int     0x80
    ; jmp     End_Print

    ; PrintNotEqual:
    ;     ;mov     edx, eax
    ;     mov     eax, WRITE
    ;     mov     ebx, STDOUT
    ;     mov     ecx, dword [esi]
    ;     mov     edx, 3
    ;     int     0x80

    ; End_Print:



    ; loop1: ;reading from STDIN
    ;     mov     eax, READ
    ;     mov     ebx, STDIN
    ;     mov     ecx, buffer
    ;     mov     edx, 1
    ;     int     0x80            ;return value is the length of what we read
		
	; 	cmp 	eax, 0			; end loop if read <= 0
	; 	jle		done


    ;     cmp     byte [isEncode], 0      ;if not +e then print unchanged
    ;     je      Print
    ; Print_Encoded:

        

	; Print:
    ;     mov     edx, eax
    ;     mov     eax, WRITE
    ;     mov     ebx, STDOUT
    ;     mov     ecx, buffer
    ;     int     0x80
		
	; 	jmp		loop1
		
done:    mov    eax, 1
         mov    ebx, 0
         int    0x80



;;;;;;;;    coppied from lecture    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
my_strlen: 
        push    ebp             ; Save caller state
        mov     ebp, esp
        
        mov ecx,0
    cont: cmp byte [eax], 0
        jz done2
        inc eax
        inc ecx
        jmp cont

        int     0x80
    done2:
        mov     ebp, esp
        pop     ebp 
        ret