
BITS 64

global enablePaging
enablePaging:
	push rbp
	mov rbp, rsp
	mov rax, [rbp + 10]
    mov cr3, rax
	
    mov rax, cr0
	
    or rax, 0x80000001
    mov cr0, rax   
    mov rsp, rbp
	
	pop rbp
	ret

