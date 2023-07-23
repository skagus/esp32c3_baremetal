/*ENTRY(_start)*/
ENTRY(_reset)


MEMORY
{
	/*iache  (rwx)  : ORIGIN = 0X4037c000, LENGTH = 16k*/
	/*iram   (rwx)  : ORIGIN = 0x40380400, LENGTH = 32k */
	iram   (rwx)  : ORIGIN = 0x40380000, LENGTH = 32k 
	dram   (rw)   : ORIGIN = 0x3FC80000 + LENGTH(iram), LENGTH = 128k
}

SECTIONS
{
		/*
	.header : AT(0)
	{
		_iram_start = .;
		LONG(0xaedb041d)
		LONG(0xaedb041d)
	} > iram
	*/
	.text.entry : AT(0)
	{
		_iram_start = .;
		KEEP(*(.text.entry))
	} > iram

	.text :
	{
		*(.text .stub .text.* .gnu.linkonce.t.*)
		*(.rela.text .rela.text.* .rela.gnu.linkonce.t.*)
		*(.gnu.warning)
	} > iram

	. = ALIGN(4);
	PROVIDE (__etext = .);
	PROVIDE (_etext = .);
	PROVIDE (etext = .);
	_iram_size = . - _iram_start;

	/**
	dram is for read only data. 
	actually, dram and iram is same memory space.
	but to access via D bus, locate it to dram.
	So, not to overlap, shift _iram_size from dram origin.
	*/
	_dram_start = ORIGIN(dram) + _iram_size;
	/*_drom_start = .;*/
	.rodata _dram_start : AT(_iram_size)
	{
		*(.rodata .rodata.* .gnu.linkonce.r.*)
		*(.rela.data .rela.data.* .rela.gnu.linkonce.r.*)
	} > dram

	.rodata1 :
	{
		*(.rodata1)
	} > dram

	.init_array :
	{
		PROVIDE_HIDDEN (__init_array_start = .);
		KEEP (*(SORT_BY_INIT_PRIORITY(.init_array.*) SORT_BY_INIT_PRIORITY(.ctors.*)))
		KEEP (*(.init_array EXCLUDE_FILE (*crtbegin.o *crtbegin?.o *crtend.o *crtend?.o ) .ctors))
		PROVIDE_HIDDEN (__init_array_end = .);
	} > dram

	.fini_array :
	{
		PROVIDE_HIDDEN (__fini_array_start = .);
		KEEP (*(SORT_BY_INIT_PRIORITY(.fini_array.*) SORT_BY_INIT_PRIORITY(.dtors.*)))
		KEEP (*(.fini_array EXCLUDE_FILE (*crtbegin.o *crtbegin?.o *crtend.o *crtend?.o ) .dtors))
		PROVIDE_HIDDEN (__fini_array_end = .);
	} > dram

	.ctors :
	{
		KEEP (*crtbegin.o(.ctors))
		KEEP (*crtbegin?.o(.ctors))
		KEEP (*(EXCLUDE_FILE (*crtend.o *crtend?.o ) .ctors))
		KEEP (*(SORT(.ctors.*)))
		KEEP (*(.ctors))
	} > dram

	.dtors :
	{
		KEEP (*crtbegin.o(.dtors))
		KEEP (*crtbegin?.o(.dtors))
		KEEP (*(EXCLUDE_FILE (*crtend.o *crtend?.o ) .dtors))
		KEEP (*(SORT(.dtors.*)))
		KEEP (*(.dtors))
	} > dram

	_dram_size = . - _dram_start;

	.sdata : AT(_iram_size + _dram_size) 
	{
		__global_pointer$ = . + 0x800;
		_sdata = .;PROVIDE (sdata = .);
		*(.sdata .sdata.* .gnu.linkonce.s.*)
		*(.sdata2 .sdata2.* .gnu.linkonce.s2.*)
		*(.srodata.cst16) 
		*(.srodata.cst8) 
		*(.srodata.cst4) 
		*(.srodata.cst2) 
		*(.srodata .srodata.*)
	} > dram
	.data : 
	{
		*(.data .data.* .gnu.linkonce.d.*)
		*(.data.rel.ro.local* .gnu.linkonce.d.rel.ro.local.*) 
		*(.data.rel.ro .data.rel.ro.* .gnu.linkonce.d.rel.ro.*)
		SORT(CONSTRUCTORS)
	} > dram
	.data1 :
	{
		*(.data1)
	} > dram

	. = ALIGN(4);
	_edata = .; PROVIDE (edata = .);
	_data_lma = ORIGIN(iram) + LOADADDR(.sdata);
	_data_size = _edata - _sdata; PROVIDE (data_size = _edata - _sdata);

	_sbss = .; PROVIDE (sbss = .);
	.sbss :
	{
		*(.dynsbss)
		*(.sbss .sbss.* .gnu.linkonce.sb.*)
		*(.scommon)
	} > dram
	.bss :
	{
	*(.dynbss)
	*(.bss .bss.* .gnu.linkonce.b.*)
	*(COMMON)
	} > dram
	. = ALIGN(4);
	_ebss = .; PROVIDE (ebss = .);
	_end = .; PROVIDE (end = .);

	/* Stack */
	.stack :
	{
		__stack_bottom = .;
		__stack_top = ORIGIN(dram) + LENGTH(dram) - 4;
		__stack_size_min = 0x4000;
		ASSERT(__stack_bottom + __stack_size_min < __stack_top, "Error: no space for stack");
	} > dram

	/* Stabs debugging sections.  */
	.stab          0 : { *(.stab) }
	.stabstr       0 : { *(.stabstr) }
	.stab.excl     0 : { *(.stab.excl) }
	.stab.exclstr  0 : { *(.stab.exclstr) }
	.stab.index    0 : { *(.stab.index) }
	.stab.indexstr 0 : { *(.stab.indexstr) }
	.comment       0 : { *(.comment) }
	.gnu.build.attributes : { *(.gnu.build.attributes .gnu.build.attributes.*) }
	/* DWARF debug sections.
		Symbols in the DWARF debugging sections are relative to the beginning
		of the section so we begin them at 0.  */
	/* DWARF 1 */
	.debug          0 : { *(.debug) }
	.line           0 : { *(.line) }
	/* GNU DWARF 1 extensions */
	.debug_srcinfo  0 : { *(.debug_srcinfo) }
	.debug_sfnames  0 : { *(.debug_sfnames) }
	/* DWARF 1.1 and DWARF 2 */
	.debug_aranges  0 : { *(.debug_aranges) }
	.debug_pubnames 0 : { *(.debug_pubnames) }
	/* DWARF 2 */
	.debug_info     0 : { *(.debug_info .gnu.linkonce.wi.*) }
	.debug_abbrev   0 : { *(.debug_abbrev) }
	.debug_line     0 : { *(.debug_line .debug_line.* .debug_line_end) }
	.debug_frame    0 : { *(.debug_frame) }
	.debug_str      0 : { *(.debug_str) }
	.debug_loc      0 : { *(.debug_loc) }
	.debug_macinfo  0 : { *(.debug_macinfo) }
	/* SGI/MIPS DWARF 2 extensions */
	.debug_weaknames 0 : { *(.debug_weaknames) }
	.debug_funcnames 0 : { *(.debug_funcnames) }
	.debug_typenames 0 : { *(.debug_typenames) }
	.debug_varnames  0 : { *(.debug_varnames) }
	/* DWARF 3 */
	.debug_pubtypes 0 : { *(.debug_pubtypes) }
	.debug_ranges   0 : { *(.debug_ranges) }
	/* DWARF Extension.  */
	.debug_macro    0 : { *(.debug_macro) }
	.debug_addr     0 : { *(.debug_addr) }
	.gnu.attributes 0 : { KEEP (*(.gnu.attributes)) }
	/DISCARD/ : { *(.note.GNU-stack) *(.gnu_debuglink) *(.gnu.lto_*) }
}
