OUTPUT(ELF64-x86-64);
ENTRY(_kentry)
. = 0x10000;
SECTIONS {
	.must_be_top = 0x10000;
	.kernel :
	{
		*(.text*)
		*(.rodata*)
		*(.rdata*)
		*(.data)
	_bss_start = .;
		*(.bss)
		*(COMMON)
		_bss_end = .;
	}
	/DISCARD/ : {
		*(.eh_frame)
		*(.comment)
	}
}