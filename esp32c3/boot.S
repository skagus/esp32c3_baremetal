    .section .text.entry
    .type _reset, @function
    .global _reset
    .extern _sbss
    .extern _ebss

     
 _reset:
        /*lui+addi  for loading a 32-bit constant into a register. 
        Used to surpass the signed problem of a single addi*/
        /*call    soc_init*/
        li      a5,0x600c0000  /* Cpu Conf.*/
        addi    a5,a5,8
        lw      a4,0(a5)
        andi    a4,a4,-4
        sw      a4,0(a5)
        lw      a4,0(a5)
        ori     a4,a4,5
        sw      a4,0(a5)

        li      a5,0x600c0000
        addi    a5,a5,88
        li      a4,0xa8000
        addi    a4,a4,1024
        sw      a4,0(a5)

/* set mstatus.mie */
        csrw   mstatus, 0x8

/* call __call_ets_update_cpu_frequency(160) */
        li      a5,0x40000000
        addi    a5,a5,1416
        li      a0,160
        jalr    a5

        /* wtd disable */

        li      a5,0x60008000
        addi    a5,a5,168
        li      a4,0x50d84000
        addi    a4,a4,-1375
        sw      a4,0(a5)

        li      a5,0x60008000
        addi    a5,a5,144
        sw      zero,0(a5)
        
        li      a5,0x60008000
        addi    a5,a5,140
        sw      zero,0(a5)
        
        li      a5,0x60008000
        addi    a5,a5,0xb0
        li      a4,0x8f1d3000
        addi    a4,a4,0x12a
        sw      a4,0(a5)
        
        li      a5,0x60008000
        addi    a5,a5,0xac
        lw      a3,0(a5)
        li      a4,0x80000000
        or      a4,a3,a4
        sw      a4,0(a5)

        li      a5,0x60008000
        addi    a5,a5,0xb4
        sw      zero,0(a5)

        li      a5,0x6001f000
        addi    a5,a5,0xfc
        lw      a4,0(a5)
        li      a5,0x6001f000
        addi    a5,a5,0xfc
        andi    a4,a4,0xfffffdff
        sw      a4,0(a5)

        li      a5,0x6001f000
        addi    a5,a5,0x48
        sw      zero,0(a5)

        li      a5,0x60020000
        addi    a5,a5,0x48
        sw      zero,0(a5)

        call    startup
.L4:
        j       .L4
