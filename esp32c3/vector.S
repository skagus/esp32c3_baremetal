/* This is the vector table. MTVEC points here.
*
* Use 4-byte intructions here. 1 instruction = 1 entry of the table.
* The CPU jumps to MTVEC (i.e. the first entry) in case of an exception,
* and (MTVEC & 0xfffffffc) + (mcause & 0x7fffffff) * 4, in case of an interrupt.
*/
	.section .text
	.extern EXC_Handler

	.balign 0x100
	.global _vector_table
	.type _vector_table, @function

_vector_table:
	.option push
	.option norvc /*Enables or disables the generation of compressed instructions. Instructions are opportunistically compressed by the RISC-V assembler when possible, but sometimes this behavior is not desirable, especially when handling alignments.*/
	.rept 31
		j EXC_Handler        /* 31 identical entries, all pointing to the interrupt handler */
	.endr
	.option pop
	.size _vector_table, .-_vector_table
