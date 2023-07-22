	.section .text.entry

	.extern __stack_top
	.extern _vector_table

	.global _reset
	.type _reset, @function

 _reset:
		/* Setup Vector table & enable machine interrupt */
		la		t0, _vector_table
		ori		t0, t0, 1
		csrw	mtvec, t0
		csrs	mstatus, t0
		csrw	mstatus, 0x8 /* Interrupt enable */

		.option push
		.option norelax
		la gp, __global_pointer$
		.option pop
/*
		lw		sp, __stack_top
*/
		j		startup

.L4:
		j       .L4
