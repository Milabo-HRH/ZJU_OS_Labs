#include "printk.h"
#include "sbi.h"

extern void test();

int start_kernel() {
    //sbi_ecall(0x1, 0x0, 0x30, 0, 0, 0, 0, 0);

//    printk("Hello RISC-V\n");
//    printk("%d", 319010192);
#ifdef DSJF
    printk("DSJF\n");
#else
    printk("DPRIORITY\n");
#endif
    schedule();
    test(); // DO NOT DELETE !!!

	return 0;
}
