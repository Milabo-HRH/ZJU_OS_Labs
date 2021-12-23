#include "printk.h"
#include "../include/clock.h"
void trap_handler(unsigned long scause, unsigned long sepc) {
    // 通过 `scause` 判断trap类型
    // 如果是interrupt 判断是否是timer interrupt
    // 如果是timer interrupt 则打印输出相关信息, 并通过 `clock_set_next_event()` 设置下一次时钟中断
    // `clock_set_next_event()` 见 4.5 节
    // 其他interrupt / exception 可以直接忽略

    // YOUR CODE HERE

    long scause_1 = (long)scause;
    if(scause_1 < 0) {
        if (scause == 0x8000000000000005) {
//            printk("kernel is running!\n");
//            printk("[S] Supervisor Mode Timer Interrupt\n");
            clock_set_next_event();
            do_timer();
        }
    }
    // } else {
    //     if (scause == 15)
	// 	{
	// 		printk("Store Page Fault\n");
	// 	}
	// 	else if (scause == 13)
	// 	{
	// 		printk("Load Page Fault\n");
	// 	}
	// 	else if (scause == 12)
	// 	{
	// 		printk("Instruction Page Fault\n");
	// 	}
    // }

}

