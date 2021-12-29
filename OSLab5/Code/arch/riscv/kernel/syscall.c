#include "syscall.h"
#include "proc.h"
extern struct task_struct *current;
void sys_write(unsigned int fd, const char* buf, unsigned long count) {
    for (unsigned long i = 0; i < count; i++) {
        sbi_ecall(fd, 0, buf[i], 0, 0, 0, 0, 0);
    }
}

unsigned long sys_getpid() {
    return current->pid;
}