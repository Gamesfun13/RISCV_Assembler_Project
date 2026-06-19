function:
        addi    sp,sp,-224
        sw      s0,220(sp)
        addi    s0,sp,224
        sw      a0,-212(s0)
        li      a5,1
        sw      a5,-20(s0)
        j       .L2
.L7:
        sw      zero,-24(s0)
        j       .L3
.L6:
        lw      a5,-24(s0)
        slli    a5,a5,2
        addi    a5,a5,-16
        add     a5,a5,s0
        lw      a4,-52(a5)
        lw      a5,-24(s0)
        slli    a5,a5,2
        addi    a5,a5,-16
        add     a5,a5,s0
        lw      a3,-68(a5)
        lw      a5,-24(s0)
        slli    a5,a5,2
        addi    a5,a5,-16
        add     a5,a5,s0
        lw      a2,-84(a5)
        lw      a5,-40(s0)
        add     a5,a2,a5
#         div     a5,a3,a5
        sub     a5,a4,a5
        sw      a5,-44(s0)
        sw      zero,-28(s0)
        lw      a5,-44(s0)
        beq     a5,zero,.L4
        lw      a5,-24(s0)
        slli    a5,a5,2
        addi    a5,a5,-16
        add     a5,a5,s0
        lw      a4,-184(a5)
        lw      a5,-44(s0)
#         div     a5,a4,a5
        sw      a5,-28(s0)
        lw      a4,-48(s0)
        lw      a5,-28(s0)
        bge     a4,a5,.L5
        lw      a5,-48(s0)
        sw      a5,-28(s0)
.L5:
        lw      a4,-52(s0)
        lw      a5,-28(s0)
        ble     a4,a5,.L4
        lw      a5,-52(s0)
        sw      a5,-28(s0)
.L4:
        lw      a5,-24(s0)
        slli    a5,a5,2
        addi    a5,a5,-16
        add     a5,a5,s0
        lw      a4,-116(a5)
        lw      a5,-24(s0)
        slli    a5,a5,2
        addi    a5,a5,-16
        add     a5,a5,s0
        lw      a3,-132(a5)
        lw      a5,-28(s0)
#         mul     a5,a3,a5
        add     a4,a4,a5
        lw      a5,-24(s0)
        slli    a5,a5,2
        addi    a5,a5,-16
        add     a5,a5,s0
        lw      a5,-84(a5)
#         mul     a4,a4,a5
        lw      a5,-24(s0)
        slli    a5,a5,2
        addi    a5,a5,-16
        add     a5,a5,s0
        lw      a5,-148(a5)
        add     a4,a4,a5
        lw      a5,-24(s0)
        slli    a5,a5,2
        addi    a5,a5,-16
        add     a5,a5,s0
        lw      a3,-164(a5)
        lw      a5,-24(s0)
        slli    a5,a5,2
        addi    a5,a5,-16
        add     a5,a5,s0
        lw      a2,-132(a5)
        lw      a5,-28(s0)
#         mul     a5,a2,a5
        add     a5,a3,a5
#         div     a4,a4,a5
        lw      a5,-24(s0)
        slli    a5,a5,2
        addi    a5,a5,-16
        add     a5,a5,s0
        sw      a4,-100(a5)
        lw      a5,-24(s0)
        slli    a5,a5,2
        addi    a5,a5,-16
        add     a5,a5,s0
        lw      a4,-100(a5)
        lw      a5,-24(s0)
        slli    a5,a5,2
        addi    a5,a5,-16
        add     a5,a5,s0
        lw      a5,-84(a5)
        sub     a4,a4,a5
        lw      a5,-28(s0)
#         mul     a3,a4,a5
        lw      a5,-24(s0)
        slli    a5,a5,2
        addi    a5,a5,-16
        add     a5,a5,s0
        lw      a4,-84(a5)
        lw      a5,-24(s0)
        addi    a5,a5,1
        add     a4,a3,a4
        slli    a5,a5,2
        addi    a5,a5,-16
        add     a5,a5,s0
        sw      a4,-84(a5)
        lw      a5,-24(s0)
        addi    a5,a5,1
        sw      a5,-24(s0)
.L3:
        lw      a4,-24(s0)
        lw      a5,-36(s0)
        blt     a4,a5,.L6
        lw      a5,-20(s0)
        addi    a5,a5,1
        sw      a5,-20(s0)
.L2:
        lw      a4,-20(s0)
        lw      a5,-32(s0)
        ble     a4,a5,.L7