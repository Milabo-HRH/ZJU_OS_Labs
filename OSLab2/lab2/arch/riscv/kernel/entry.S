    .section .text.entry
    .align 2
    .globl _traps
_traps:
    # YOUR CODE HERE
    # -----------
    # 1. save 32 registers and sepc to stack
    sd x1, 0(x2)
    sd x2, -8(x2)
    sd x3, -16(x2)
    sd x4, -24(x2)
    sd x5, -32(x2)
    sd x6, -40(x2)
    sd x7, -48(x2)
    sd x8, -56(x2)
    sd x9, -64(x2)
    sd x10, -72(x2)
    sd x11, -80(x2)
    sd x12, -88(x2)
    sd x13, -96(x2)
    sd x14, -104(x2)
    sd x15, -112(x2)
    sd x16, -120(x2)
    sd x17, -128(x2)
    sd x18, -136(x2)
    sd x19, -144(x2)
    sd x20, -152(x2)
    sd x21, -160(x2)
    sd x22, -168(x2)
    sd x23, -176(x2)
    sd x24, -184(x2)
    sd x25, -192(x2)
    sd x26, -200(x2)
    sd x27, -208(x2)
    sd x28, -216(x2)
    sd x29, -224(x2)
    sd x30, -232(x2)
    sd x31, -240(x2)
    csrr t0, sepc
    sd t0, -248(x2)
    addi x2, x2, -256

    # -----------
    # 2. call trap_handler
    csrr a0, scause
    csrr a1, sepc
    call trap_handler
    # -----------
    # 3. restore sepc and 32 registers (x2(x2) should be restore last) fromstack


    addi x2, x2, 256
    ld t0, -248(x2)
    csrw sepc, t0
    ld x1, -0(x2)
    ld x3, -16(x2)
    ld x4, -24(x2)
    ld x5, -32(x2)
    ld x6, -40(x2)
    ld x7, -48(x2)
    ld x8, -56(x2)
    ld x9, -64(x2)
    ld x10, -72(x2)
    ld x11, -80(x2)
    ld x12, -88(x2)
    ld x13, -96(x2)
    ld x14, -104(x2)
    ld x15, -112(x2)
    ld x16, -120(x2)
    ld x17, -128(x2)
    ld x18, -136(x2)
    ld x19, -144(x2)
    ld x20, -152(x2)
    ld x21, -160(x2)
    ld x22, -168(x2)
    ld x23, -176(x2)
    ld x24, -184(x2)
    ld x25, -192(x2)
    ld x26, -200(x2)
    ld x27, -208(x2)
    ld x28, -216(x2)
    ld x29, -224(x2)
    ld x30, -232(x2)
    ld x31, -240(x2)
    ld x2, -8(x2)
    # -----------
    # 4. return from trap
    sret
    # -----------