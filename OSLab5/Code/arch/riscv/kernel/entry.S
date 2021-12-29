    .section .text.entry
    .align 2
    .globl _traps
_traps:
    # YOUR CODE HERE
    # -----------
    # 1. save 32 registers and sepc to stack
    addi x2, x2, -256
    sd x1, 0(x2)
    sd x2, 8(x2)
    sd x3, 16(x2)
    sd x4, 24(x2)
    sd x5, 32(x2)
    sd x6, 40(x2)
    sd x7, 48(x2)
    sd x8, 56(x2)
    sd x9, 64(x2)
    sd x10, 72(x2)
    sd x11, 80(x2)
    sd x12, 88(x2)
    sd x13, 96(x2)
    sd x14, 104(x2)
    sd x15, 112(x2)
    sd x16, 120(x2)
    sd x17, 128(x2)
    sd x18, 136(x2)
    sd x19, 144(x2)
    sd x20, 152(x2)
    sd x21, 160(x2)
    sd x22, 168(x2)
    sd x23, 176(x2)
    sd x24, 184(x2)
    sd x25, 192(x2)
    sd x26, 200(x2)
    sd x27, 208(x2)
    sd x28, 216(x2)
    sd x29, 224(x2)
    sd x30, 232(x2)
    sd x31, 240(x2)
    csrr t0, sepc
    sd t0, 248(x2)
    csrr t0, sstatus
    sd t0, 256(x2)
    addi sp, sp, -8
    mv a2, sp
    csrr t0, sscratch
    beqz t0, noUser1
    csrrw sp, sscratch, sp
noUser1: 
    # -----------
    # 2. call trap_handler
    csrr a0, scause
    csrr a1, sepc
    call trap_handler
    csrr t0, sscratch
    beqz t0, noUser2
    csrrw sp, sscratch, sp
    # -----------
    # 3. restore sepc and 32 registers (x2(x2) should be restore last) fromstack
noUser2:
    addi x2, x2, 8
    ld t0, 248(x2)
    csrw sepc, t0
    ld x1, 0(x2)
    ld x3, 16(x2)
    ld x4, 24(x2)
    ld x5, 32(x2)
    ld x6, 40(x2)
    ld x7, 48(x2)
    ld x8, 56(x2)
    ld x9, 64(x2)
    ld x10, 72(x2)
    ld x11, 80(x2)
    ld x12, 88(x2)
    ld x13, 96(x2)
    ld x14, 104(x2)
    ld x15, 112(x2)
    ld x16, 120(x2)
    ld x17, 128(x2)
    ld x18, 136(x2)
    ld x19, 144(x2)
    ld x20, 152(x2)
    ld x21, 160(x2)
    ld x22, 168(x2)
    ld x23, 176(x2)
    ld x24, 184(x2)
    ld x25, 192(x2)
    ld x26, 200(x2)
    ld x27, 208(x2)
    ld x28, 216(x2)
    ld x29, 224(x2)
    ld x30, 232(x2)
    ld x31, 240(x2)
    ld x2, 8(x2)
    addi x2, x2, 256
    # -----------
    # 4. return from trap
    sret
    # -----------
    .globl __dummy
__dummy:

    csrrw sp, sscratch, sp

    la t0, 0
    csrw sepc, t0

    sret

    .globl __switch_to
__switch_to:
# save state to prev process
    # YOUR CODE HERE
    sd ra, 40(a0)
    sd sp, 48(a0)
    sd s0, 56(a0)
    sd s1, 64(a0)
    sd s2, 72(a0)
    sd s3, 80(a0)
    sd s4, 88(a0)
    sd s5, 96(a0)
    sd s6, 104(a0)
    sd s7, 112(a0)
    sd s8, 120(a0)
    sd s9, 128(a0)
    sd s10, 136(a0)
    sd s11, 144(a0)

    csrr s11, sepc
    sd s11, 152(a0)
    csrr s11, sstatus
    sd s11, 160(a0)
    csrr s11, sscratch
    sd s11, 168(a0)
    
#change page table
    ld s11, 176(a1)
    srl s11, s11, 12
    li t2, 0x8000000000000000
    add s11, s11, t2
    csrw satp, s11
    sfence.vma x0,x0

# restore state from next process
    # YOUR CODE HERE
    ld s11, 152(a1)
    csrw sepc, s11
    ld s11, 160(a1)
    csrw sstatus, s11
    ld s11, 168(a1)
    csrw sscratch, s11

    ld ra, 40(a1)
    ld sp, 48(a1)
    ld s0, 56(a1)
    ld s1, 64(a1)
    ld s2, 72(a1)
    ld s3, 80(a1)
    ld s4, 88(a1)
    ld s5, 96(a1)
    ld s6, 104(a1)
    ld s7, 112(a1)
    ld s8, 120(a1)
    ld s9, 128(a1)
    ld s10, 136(a1)
    ld s11, 144(a1)
    

    ret