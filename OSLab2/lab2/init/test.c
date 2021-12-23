#include "printk.h"
#include "defs.h"

// Please do not modify

void test() {
    while (1){
        for(int i =0; i< 100000000;i++) {
        }
        printk("kernel is running!\n");

    }
}
