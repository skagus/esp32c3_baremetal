    .section .text.entry
    .type _reset, @function
    .global _reset
    .extern _sbss
    .extern _ebss

     
 _reset:
        addi    sp,sp,-32
        sw      ra,28(sp)
        sw      s0,24(sp)
        addi    s0,sp,32
        /*lui+addi  for loading a 32-bit constant into a register. 
        Used to surpass the signed problem of a single addi*/
        /*call    soc_init*/
        li      a5,0x600c0000
        addi    a5,a5,8
        lw      a4,0(a5)
        li      a5,0x600c0000
        addi    a5,a5,8
        andi    a4,a4,-4
        sw      a4,0(a5)
        li      a5,0x600c0000
        addi    a5,a5,8
        lw      a4,0(a5)
        li      a5,0x600c0000
        addi    a5,a5,8
        ori     a4,a4,5
        sw      a4,0(a5)
        li      a5,0x600c0000
        addi    a5,a5,88
        li      a4,0xa8000
        addi    a4,a4,1024
        sw      a4,0(a5)
        csrw   mstatus, 0x8
        li      a5,0x40000000
        addi    a5,a5,1416
        li      a0,160
        jalr    a5
        lui     a5,%hi(_vector_table)
        addi    a5,a5,%lo(_vector_table)
        sw      a5,-20(s0)
        lw      a5,-20(s0)
        csrrw zero, mtvec, a5
        csrr a5, mtvec
        sw      a5,-24(s0)

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
        li      a5,0x60008000
        addi    a5,a5,0xac
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
        li      a5,0
        
        call    startup
.L4:
        j       .L4
