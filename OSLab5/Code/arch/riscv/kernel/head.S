.extern start_kernel
.extern early_pgtbl
.extern setup_vm
    .section .text.init
    .globl _start
_start:

    la sp, boot_stack
    
    call setup_vm
    call relocate
    # ------------------
    # - your code here -
    # set stvec = _traps
    
    la t0, _traps
    csrrw x0, stvec, t0
    # ------------------
    # set sie[STIE] = 1
    li t0, 0x00000020
    csrrs x0, sie, t0
    # ------------------
    # set first time interrupt
    call mm_init
    call setup_vm_final
    
    call task_init
    call clock_set_next_event
    # ------------------
    # set sstatus[SIE] = 1 (not in lab5)
    # li t1, 0x00000002
    # csrrs x0, sstatus, t1
    # ------------------
    # ------------------
    # - your lab1 code -
    # ------------------

    call start_kernel
    # ------------------
relocate:
    # set ra = ra + PA2VA_OFFSET
    # set sp = sp + PA2VA_OFFSET (If you have set the sp before)
   
    ###################### 
    #   YOUR CODE HERE   #
    li t0, 0xffffffdf80000000
    add ra, ra, t0
    add sp, sp, t0
    ######################

    # set satp with early_pgtbl
    
    ###################### 
    #   YOUR CODE HERE   #
    la  t0, early_pgtbl
    srl t0, t0, 12
    li  t1, 0x8000000000000000
    add t0, t0, t1
    csrrw x0, satp, t0
    ######################
    
    # flush tlb
    sfence.vma zero, zero

    ret

    .section .bss.stack
    .globl boot_stack
boot_stack:
    .space 4096 # <-- change to your stack size

    .globl boot_stack_top
boot_stack_top: