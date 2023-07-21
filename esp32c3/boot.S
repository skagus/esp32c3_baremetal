	.section .text.entry
	.type _reset, @function
	.global _reset
	.extern _sbss
	.extern _ebss
	.extern __stack_top
	.extern _vector_table

 _reset:
		j		startup

		.option push
		.option norelax
		la gp, __global_pointer$
		.option pop


		lw		sp, __stack_top

		/* Setup Vector table */
		csrs	mstatus, t0
		la		t0, _vector_table
		ori		t0, t0, 1
		csrw	mtvec, t0

		j		startup

/* set mstatus.mie */
#        csrw   mstatus, 0x8

		j    startup
.L4:
		j       .L4
