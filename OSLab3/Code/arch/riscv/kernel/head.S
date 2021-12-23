.extern start_kernel
    .section .text.init
    .globl _start
_start:
    # ------------------
    # - your code here -
    # set stvec = _traps
    la sp, boot_stack
    la t0, _traps
    csrrw x0, stvec, t0
    # ------------------
    # set sie[STIE] = 1
    li t0, 0x00000020
    csrrs x0, sie, t0
    # ------------------
    # set first time interrupt
    call mm_init
    call task_init
    call clock_set_next_event
    # ------------------
    # set sstatus[SIE] = 1
    li t1, 0x00000002
    csrrs x0, sstatus, t1
    # ------------------
    # ------------------
    # - your lab1 code -
    # ------------------

    call start_kernel
    # ------------------

    .section .bss.stack
    .globl boot_stack
boot_stack:
    .space 4096 # <-- change to your stack size

    .globl boot_stack_top
boot_stack_top: