function:
        addi    sp,sp,-48
        sw      s0,44(sp)
        addi    s0,sp,48
        sw      a0,-36(s0)
        sw      x0,-24(s0)
        lw      a5,-28(s0)
        beq     a5,x0,.L2
        lw      a4,-28(s0)
        li      a5,1
        bne     a4,a5,.L3
.L2:
        li      a5,1
        sw      a5,-24(s0)
.L3:
        li      a5,2
        sw      a5,-20(s0)
        j       .L4
.L7:
        lw      a4,-28(s0)
        lw      a5,-20(s0)
#         rem     a5,a4,a5
        bne     a5,x0,.L5
        li      a5,1
        sw      a5,-24(s0)
        j       .L6
.L5:
	lw      a5,-20(s0)
        addi    a5,a5,1
        sw      a5,-20(s0)
.L4:
        lw      a5,-28(s0)
        srli    a4,a5,31
        add     a5,a4,a5
        srai    a5,a5,1
        mv      a4,a5
        lw      a5,-20(s0)
        ble     a5,a4,.L7
.L6:
        li      a5,0
        mv      a0,a5
        lw      s0,44(sp)
        addi    sp,sp,48
        jr      ra
