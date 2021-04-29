
bits 64

global _kentry
extern _kmain

section .must_be_top:
_kentry:
	
	
	
	
	
	call _kmain
	
	ret
	
	jmp $
