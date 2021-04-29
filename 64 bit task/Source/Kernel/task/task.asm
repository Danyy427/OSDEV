
BITS 64

global setStack
global pushRegisters
global popRegisters
global PUSH_64
global read_rip


setStack:
	pop qword[addrs3]
	mov rbp, rcx
	mov rsp, rbp
	push qword[addrs3]
    ret  

pushRegisters:
	pop qword[addrs2]
	push    qword[rcx+0x110]
	push    qword[rcx+0x108]
	push    qword[rcx+0x100]
	push    qword[rcx+0x98]
	push    qword[rcx+0x90]
	push    qword[rcx+0x88]
	push    qword[rcx+0x80]
	push    qword[rcx+0x78]
    push    qword[rcx+0x70]
    push    qword[rcx+0x68]
    push    qword[rcx+0x60]
    push    qword[rcx+0x58]
    push    qword[rcx+0x50]
    push    qword[rcx+0x48]
    push    qword[rcx+0x40]
    push    qword[rcx+0x38]
    push    qword[rcx+0x30]
    push    qword[rcx+0x28]
    push    qword[rcx+0x20]
    push    qword[rcx+0x18]
    push    qword[rcx+0x10]
    push    qword[rcx+0x8]
    push    qword[rcx]
	push qword[addrs2]
    ret
	
PUSH_64:
	pop qword[addrs4]
	push rcx
	push qword[addrs4]
    ret

read_rip:
	pop rax
	jmp rax
	



addrs4: resb 8
addrs3: resb 8
addrs2: resb 8
addrs1: resb 8
