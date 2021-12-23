//
// Created by h3root on 10/29/21.
//
// clock.c

// QEMU中时钟的频率是10MHz, 也就是1秒钟相当于10000000个时钟周期。
#include "../include/clock.h"
#include "sbi.h"
uint64 TIMECLOCK = 10000000;

uint64 get_cycles() {
    // 使用 rdtime 编写内联汇编，获取 time 寄存器中 (也就是mtime 寄存器 )的值并返回
    uint64 res;
    __asm__ volatile(
    "rdtime a1\n"
    "mv %[ret0], a1\n"
    : [ret0] "=r"(res)
    :
    : "memory"
    );
    return res;
}

void clock_set_next_event() {
    // 下一次 时钟中断 的时间点
    uint64 next = get_cycles() + TIMECLOCK;
    sbi_ecall(0x0, 0x0, next, 0, 0, 0, 0, 0);
    // 使用 sbi_ecall 来完成对下一次时钟中断的设置

}
