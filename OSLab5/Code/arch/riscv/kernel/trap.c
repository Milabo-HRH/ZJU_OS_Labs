#include "printk.h"
#include "../include/clock.h"
#include "../include/syscall.h"

struct pt_regs {
    unsigned long x[32];
    unsigned long sepc;
    unsigned long sstatus;
};

void trap_handler(unsigned long scause, unsigned long sepc, struct pt_regs *regs) {
    // 通过 `scause` 判断trap类型
    // 如果是interrupt 判断是否是timer interrupt
    // 如果是timer interrupt 则打印输出相关信息, 并通过 `clock_set_next_event()` 设置下一次时钟中断
    // `clock_set_next_event()` 见 4.5 节
    // 其他interrupt / exception 可以直接忽略

    // YOUR CODE HERE

    long scause_1 = (long)scause;
    if(scause_1 < 0) {
        if (scause == 0x8000000000000005) {
            clock_set_next_event();
            do_timer();
        }
    } else {
        if (scause == 8) {
            if (regs->x[17] == SYS_WRITE) {
                sys_write(regs->x[10], regs->x[11], regs->x[12]);
            } else if (regs->x[17] == SYS_GETPID) {
                regs->x[10] = sys_getpid();
            }
            regs->sepc += 4;
        }
    }
}

