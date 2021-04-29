
[ORG 0x7e00]
BITS 16

global _SecondStage

_SecondStage:
	
	; Lets take the drive number -------;
	mov [dev], dl                       ;
	; ----------------------------------;
	
	; Inform that Bootloader is gone ---;
	mov bx, WelcomeMessage              ;
	call print_string                   ;
	; ----------------------------------;
	
	
	mov ah, 0x41
	mov bx, 0x55AA
	int 13h
	jc disk_error
	
	
	
	;-----------------------------------;
	;mov ax, 0							;
	;mov ds, ax            				;
	;mov si, DAPACK						; address of "disk address packet"
	;mov ah, 0x42						; AL is unused
	;mov dl, [dev]		                ; drive number 0 (OR the drive # with 0x80)
	;int 0x13                            ;
	;jc disk_error                       ;
    ;                                    ;
	;push word[blkcnt]                   ;
	;call print_hex_word                 ;
	; ----------------------------------;
	
	jmp diskload
	
jmpbb:
	xor ax, ax
	mov es, ax
	jmp do_e820
jmpbs:
	;jmp $

	; Set Video Mode -------------------;
	                                    ;
	mov ax, 0                           ;
	mov es, ax                          ;
	mov ax, vbe_info_structure          ;
	mov di, ax                          ;
	                                    ;
	mov ax, 0x4F00                      ;
	                                    ;
	int 0x10                            ;
	                                    ;
	cmp ax, 0x004F                      ;
	jne Video_Error                     ;
                                        ;
	jmp SetMode                         ;
jmpback:	                            ;
     									;
										;
	; ----------------------------------;	

	
	
	; Go 32 Bit ------------------------;
	jmp Start32                         ;
    ; ----------------------------------;
	
	; Error when Getting VBE_INFO-------;
Video_Error:                            ;
	                                    ;
	mov bx, video_error_msg             ;
	call print_string                   ;
	                                    ;
	jmp $                               ;
	                                    ;
	; ----------------------------------;
	
Start32:
	
	; Load GDT--------------------------;
	lgdt [gdt_descriptor]               ;
	                                    ;
    cli                                 ;
	; ----------------------------------;
	
	; Enable Protected Mode-------------;
    mov eax , cr0                       ; To make the switch to protected mode , we set
    or eax , 0x1                        ; the first bit of CR0 , a control register
    mov cr0 , eax                       ; Update the control register
	; ----------------------------------;

	; Far Jump--------------------------;
    jmp CODE_SEG:Bit32                  ;
	; ----------------------------------;
	
	jmp $
	


DAPACK:
		db	0x10
		db	0
blkcnt:	dw	1		; int 13 resets this to # of blocks actually read/written
d_add:	dw	0x0000		; memory buffer destination address (0:7c00)
d_seg:	dw	0x1000		; in memory page zero
d_lba:	dd	5		; put the lba to read in this spot
		dd	0		; more storage bytes only for big lba's ( > 4 bytes )





diskload:
	push ax
	push dx
	push cx
	
	mov cx, 0
.loop1:
	mov ax, 0							;
	mov ds, ax            				;
	mov si, DAPACK						; address of "disk address packet"
	mov ah, 0x42						; AL is unused
	mov dl, [dev]		                ; drive number 0 (OR the drive # with 0x80)
	int 0x13                            ;
	jc disk_error                       ;
                                        ;
	push word[blkcnt]                   ;
	call print_hex_word                 ;
	
	add word[d_lba], 1
	add word[d_seg], 0x20
	
	cmp cx, 105
	je .end
	
	add cx, 1
	jmp .loop1
.end:
	pop cx
	pop bx
	pop ax
	jmp jmpbb
; Definitions -------------;
                           ;
disk_error:                ;

	mov bx, ax
	push word bx
	call print_hex_word

	mov bx, disk_error_msg ;
	call print_string      ;
	
	
	
	jmp $                  ;
; -------------------------;

; 16 Bit Print ------------;
print_string:	           ;
                           ;	
.loop:                     ;
    mov al,[bx]            ; 
	cmp al,0               ;
	je return              ;
	push bx                ;
	mov ah,0Eh             ;
	                       ;
	int 10h                ;
	pop bx                 ;
	inc bx                 ;
	jmp .loop              ;
	                       ;
return:                    ;
	ret	                   ;
; -------------------------;

; 16 Bit Print Hex--;
print_hex_word:     ;
    push bp         ;
    mov bp, sp      ; BP=SP, on 8086 can't use sp in memory operand
    push dx         ; Save all registers we clobber
    push cx         ;
    push bx         ;
    push ax         ;
                    ;
    mov cx, 0x0404  ; CH = number of nibbles to process = 4 (4*4=16 bits)
                    ; CL = Number of bits to rotate each iteration = 4 (a nibble)
    mov dx, [bp+4]  ; DX = word parameter on stack at [bp+4] to print
    mov bx, [bp+6]  ; BX = page / foreground attr is at [bp+6]
                    ;
.loop:              ;
    rol dx, cl      ; Roll 4 bits left. Lower nibble is value to print
    mov ax, 0x0e0f  ; AH=0E (BIOS tty print),AL=mask to get lower nibble
    and al, dl      ; AL=copy of lower nibble
    add al, 0x90    ; Work as if we are packed BCD
    daa             ; Decimal adjust after add.
                    ;    If nibble in AL was between 0 and 9, then CF=0 and
                    ;    AL=0x90 to 0x99
                    ;    If nibble in AL was between A and F, then CF=1 and
                    ;    AL=0x00 to 0x05
    adc al, 0x40    ; AL=0xD0 to 0xD9
                    ; or AL=0x41 to 0x46
    daa             ; AL=0x30 to 0x39 (ASCII '0' to '9')
                    ; or AL=0x41 to 0x46 (ASCII 'A' to 'F')
    int 0x10        ; Print ASCII character in AL
    dec ch          ;
    jnz .loop       ; Go back if more nibbles to process
                    ;
	mov al, ' '		; ##############################################		
	int 10h	        ; ##############################################
    pop ax          ; Restore registers
    pop bx          ;
    pop cx          ;
    pop dx          ;
    pop bp          ;
    ret             ;
; ------------------;


; Video List ------------------------------------------------------;
SetMode:                                                           ;
										                           ;
	mov ecx, 0                                                     ; Counter
.loop1:                                                            ;
	                                                               ;
	mov ax, vbe_info_structure+0x0E                                ; Address to the video modes list
	add ax, cx                                                     ;
	mov bx, ax                                                     ;
	;push word [bx]                                                ;
	;call print_hex_word                                           ; For debug
	;add sp, 2                                                     ; Add 2 because each video mode is a word not a byte
	add ecx, 2                          	                       ;
	                                                               ;
	cmp word [bx], 0xFFFF                                          ; Stop if the result is 0xFFFF means list ended
	je .done                                                       ;
	                                                               ;
	mov ax, [bx]                                                   ;
	mov word [current_mode], ax                                    ; Save the current mode for later use
	                                                               ;
	push cx                                                        ;
	                                                               ;
	mov ax, 0                                                      ;
	mov es, ax                                                     ;
	mov ax, vbe_mode_structure                                     ;  
	mov di, ax                                                     ;
	mov cx, [bx]                                                   ;
	mov ax, 0x4f01                                                 ;
	int 10h                                                        ; Get teh video modes into the vbe_mode_structure 
	                                                               ;
	mov ax, vbe_mode_structure+0x12                                ; vbe_mode_structure+0x12 is the window x
	mov bx, ax                                                     ;
	;push word [bx]                                                ;
	;call print_hex_word                                           ;
	;add sp, 2                                                     ;
	                                                               ;
	cmp word [bx], 780h                                            ; We are looking for 1920 pixels
	jnz .notmode                                                   ;
	                                                               ;
	mov ax, vbe_mode_structure+0x14                                ; vbe_mode_structure+0x12 is the window y
	mov bx, ax                                                     ;
	;push word [bx]                                                ;
	;call print_hex_word                                           ; For debug
	;add sp, 2                                                     ;
	                                                               ;
	cmp word [bx], 438h                                            ; We are looking for 1080 pixels
	jnz .notmode                                                   ;
	                                                               ;
	mov ax, vbe_mode_structure+0x19                                ; vbe_mode_structure+0x12 is the bits per pixel
	mov bx, ax                                                     ;
	mov ah, byte [bx]                                              ; Ah is the bits per pixel
	mov al, 0                                                      ; Al is not important
	;push ax                                                       ;
	;call print_hex_word                                           ; For debug
	;add sp, 2                                                     ;
	                                                               ;
	cmp ax, 0x2000                                                 ; We want 24-bits per pixel
	jnz .notmode                                                   ;
	                                                               ;
	mov ax, 0x4F02                                                 ; If all is good lets set the mode
	mov dx, [current_mode]                                         ; Take the current mode
	or dx, 0x4000                                                  ;
	mov bx, dx                                                     ;
	push bx                                                        ;
	call print_hex_word                                            ; For debug purposes can stay
	add sp, 2                                                      ;
	                                                               ;
	int 10h                                                        ; GO
                                                                   ;															   
	mov bx, vbe_mode_structure+0x28                                ; vbe_mode_structure+0x28 is the framebuffer address
	;push word [bx]                                                ;
	;call print_hex_word                                           ;
	;add sp, 2                                                     ;
	mov ax, [bx]                                                   ;
	                                                               ;
	mov word [framebuffer], ax                                     ;
	mov bx, vbe_mode_structure+0x2A                                ; vbe_mode_structure+0x2A is the framebuffer address
	;push word [bx]                                                ;
	;call print_hex_word                                           ;
	;add sp, 2                                                     ;
	mov ax, [bx]                                                   ;
	mov word [framebuffer+2], ax                                   ;
	                                                               ;
		                                                           ;
	jmp .done                                                      ;
.notmode:                                                          ;
                                                                   ;
	pop cx                                                         ;
	                                                               ; 
	jmp .loop1                                                     ; If it is not the mode we want restore cx and go back to the loop
.done:		                                                       ;
	jmp jmpback                                                    ;
	;ret                                                           ;
;------------------------------------------------------------------;

mmap_ent equ 0x8600             ; the number of entries will be stored at 0x8000
do_e820:
        mov di, 0x8604          ; Set di to 0x8004. Otherwise this code will get stuck in `int 0x15` after some entries are fetched 
	xor ebx, ebx		; ebx must be 0 to start
	xor bp, bp		; keep an entry count in bp
	mov edx, 0x0534D4150	; Place "SMAP" into edx
	mov eax, 0xe820
	mov [es:di + 20], dword 1	; force a valid ACPI 3.X entry
	mov ecx, 24		; ask for 24 bytes
	int 0x15
	jc short .failed	; carry set on first call means "unsupported function"
	mov edx, 0x0534D4150	; Some BIOSes apparently trash this register?
	cmp eax, edx		; on success, eax must have been reset to "SMAP"
	jne short .failed
	test ebx, ebx		; ebx = 0 implies list is only 1 entry long (worthless)
	je short .failed
	jmp short .jmpin
.e820lp:
	mov eax, 0xe820		; eax, ecx get trashed on every int 0x15 call
	mov [es:di + 20], dword 1	; force a valid ACPI 3.X entry
	mov ecx, 24		; ask for 24 bytes again
	int 0x15
	jc short .e820f		; carry set means "end of list already reached"
	mov edx, 0x0534D4150	; repair potentially trashed register
.jmpin:
	jcxz .skipent		; skip any 0 length entries
	cmp cl, 20		; got a 24 byte ACPI 3.X response?
	jbe short .notext
	test byte [es:di + 20], 1	; if so: is the "ignore this data" bit clear?
	je short .skipent
.notext:
	mov ecx, [es:di + 8]	; get lower uint32_t of memory region length
	or ecx, [es:di + 12]	; "or" it with upper uint32_t to test for zero
	jz .skipent		; if length uint64_t is 0, skip entry
	inc bp			; got a good entry: ++count, move to next storage spot
	add di, 24
.skipent:
	test ebx, ebx		; if ebx resets to 0, list is complete
	jne short .e820lp
.e820f:
	mov [mmap_ent], bp	; store the entry count
	clc			; there is "jc" on end of list to this point, so the carry must be cleared
	jmp jmpbs
.failed:
	stc			; "function unsupported" error exit
	jmp jmpbs

; Variables -------------------------------------------------------------;
WelcomeMessage db "Second Stage Bootloader taking over...",13,10,0       ;
disk_error_msg db "Disk Error",13,10,0                                   ;
dev: db 0                                                                ;
KERNEL_ADDR EQU 0x1000                                                  ;
video_error_msg db "Cannot retrieve video information",13,10,0           ;
done_msg db "Done",13,10,0                                               ;
space_msg db " ",0                                                       ;
vbe_info_structure:                                                      ;
	.signature db "VBE2"	; indicate support for VBE 2.0+              ;
	.table_data: resb 512-4	; reserve space for the table below          ;
vbe_mode_structure:                                                      ;
	.data_d: resb 256                                                    ;
current_mode: dw 0                                                       ;
framebuffer: dd 0                                                        ;
; -----------------------------------------------------------------------;
%define PAGE_PRESENT    (1 << 0)
%define PAGE_WRITE      (1 << 1)

[BITS 32]                  ; We are now in 32 bit   

Bit32:
	
	mov ax , DATA_SEG
    mov ds , ax 
    mov ss , ax 
    mov es , ax
    mov fs , ax
    mov gs , ax

    mov ebp , 0x90000 
    mov esp , ebp

	;mov eax, vbe_mode_structure ;[framebuffer]
	
	call SetupIdentityPaging
	
	lgdt 	[gdt_descriptor_64]
 
	
	jmp 	CODE_SEG_64:Start64Bit
	
	

	jmp $

; GDT -------------------------------------------------------------------;
gdt_start :                                                              ;
	gdt_null :                                                           ; the mandatory null descriptor
	dd 0x0                                                               ; 'dd ' means define double word ( i.e. 4 bytes )
	dd 0x0                                                               ;
	gdt_code :                                                           ;
	dw 0xffff                                                            ; Limit ( bits 0 -15)
	dw 0x0                                                               ; Base ( bits 0 -15)
	db 0x0                                                               ; Base ( bits 16 -23)
	db 10011010b                                                         ; 1st flags , type flags
	db 11001111b                                                         ; 2nd flags , Limit ( bits 16 -19)
	db 0x0                                                               ; Base ( bits 24 -31)
	gdt_data :                                                           ;
	dw 0xffff                                                            ; Limit ( bits 0 -15)
	dw 0x0                                                               ; Base ( bits 0 -15)
	db 0x0                                                               ; Base ( bits 16 -23)
	db 10010010b                                                         ; 1st flags , type flags
	db 11001111b                                                         ; 2nd flags , Limit ( bits 16 -19)
	db 0x0                                                               ; Base ( bits 24 -31)
gdt_end :                                                                ;
                                                                         ;
gdt_descriptor :                                                         ;
dw gdt_end - gdt_start - 1                                               ;
dd gdt_start                                                             ;
                                                                         ;
CODE_SEG equ gdt_code - gdt_start                                        ;
DATA_SEG equ gdt_data - gdt_start                                        ;
; -----------------------------------------------------------------------;





gdt_start_64:
gdt_null_64:
    dw      0x0000
    dw      0x0
    db      0x0
    db      0x0
    db      0x0
    db      0x0

gdt_code_64: 
    dw      0xFFFF
    dw      0x0
    db      0x0
    db      10011010b ; 0x9A
    db      10101111b ; 0xCF
    db      0x0

gdt_data_64:
    dw      0xFFFF
    dw      0x0
    db      0x0
    db      10010010b ; 0x92
    db      10101111b ; 0xAF
    db      0x0
	
gdt_user_code_64:
    dw      0xFFFF
    dw      0x0
    db      0x0
    db      11111010b ; 0xFA
    db      10101111b ; 0xAF
    db      0x0
	
gdt_user_data_64:
    dw      0xFFFF
    dw      0x0
    db      0x0
    db      11110010b ; 0xF2
    db      10101111b ; 0xAF
    db      0x0
	

gdt_end_64:

gdt_descriptor_64:
    dw gdt_end_64 - gdt_start_64 - 1
    dd gdt_start_64

CODE_SEG_64    equ gdt_code_64 - gdt_start_64
DATA_SEG_64    equ gdt_data_64 - gdt_start_64

SetupIdentityPaging:
	mov     edi, PageTableEntry
	mov     cr3, edi
	xor 	eax, eax
    mov 	ecx, 0x1000
    rep 	stosd
    mov 	edi, cr3

	mov     dword [edi], 0x2003
	add     edi, 0x1000
	mov     dword [edi], 0x3003
	add     edi, 0x1000
	mov     dword [edi], 0x4003
	add     edi, 0x1000

	mov     ebx, 0x00000003
	mov     ecx, 512

	.SetEntry:
		mov     dword [edi], ebx
		add     ebx, 0x1000
		add     edi, 8
		loop    .SetEntry

	mov     eax, cr4
	or      eax, 1 << 5
	mov     cr4, eax

	mov     ecx, 0xC0000080
	rdmsr
	or      eax, 1 << 8
	wrmsr
	
	mov     eax, cr0
	or      eax, 1 << 31
	mov     cr0, eax
	
	ret

PageTableEntry equ 0x1000

  


[BITS 64]

Start64Bit:
	cli

    mov     ax, DATA_SEG_64
    mov     ds, ax
    mov     ss, ax
    mov     es, ax
    mov     fs, ax
    mov     gs, ax


	mov rbp , 0x90000
    mov rsp , rbp
	
	mov rcx, vbe_mode_structure
	mov rdx, 0x8604
	call 0x10000
	
	jmp $
	
; Padding -----------------;
times 2048-($-$$) db 0     ;
; -------------------------;
