
bits 64

global _kentry
extern _kmain

section .must_be_top:
_kentry:
	
	push rbp    
    mov rbp, rsp
	;mov  edi, [ ebp +8 ] 	    
	;mov  eax, 0xFFFFFFFF   
	;stosd

	mov rdi, [ rbp +10 ]  
	
	call _kmain
	add rsp, 10
	
	pop rbp     ; minimal cleanup
	ret
	
	jmp $
