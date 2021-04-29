BITS 64
global __gdt_flush

__gdt_flush:
	
	lgdt [rcx]
    mov     ax, 0x10      ; 0x10 is the offset in the GDT to our data segment
    mov     ds, ax        ; Load all data segment selectors
    mov     es, ax
    mov     fs, ax
    mov     gs, ax
    mov     ss, ax
	
	
	ret