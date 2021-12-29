#pragma once

#define SYS_WRITE   64
#define SYS_GETPID  172

void sys_write(unsigned int fd, const char* buf, unsigned long count);
unsigned long sys_getpid();