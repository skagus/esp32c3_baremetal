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

		.option push
		.option norelax
		la gp, __global_pointer$
		.option pop
/*
		lw		sp, __stack_top
*/
# B extention Test --> Not supported.
#		andn   a4, a1, a2
#		clmulh a4, a1, a2
#		bclr  a4, a1, a2
#		sh1add a4, a1, a2
		j		startup

.L4:
		j		.L4
