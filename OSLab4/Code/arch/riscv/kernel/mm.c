#include "defs.h"
#include "string.h"
#include "mm.h"

#include "printk.h"

extern char _ekernel[];

struct {
    struct run *freelist;
} kmem;

unsigned long kalloc() {
    struct run *r;

    r = kmem.freelist;
    kmem.freelist = r->next;
    
    memset((void *)r, 0x0, PGSIZE);
    return (unsigned long) r;
}

void kfree(unsigned long addr) {
    struct run *r;

    // PGSIZE align 
    addr = addr & ~(PGSIZE - 1);

    memset((void *)addr, 0x0, (unsigned long)PGSIZE);

    r = (struct run *)addr;
    r->next = kmem.freelist;
    kmem.freelist = r;

    return ;
}

void kfreerange(char *start, char *end) {
    char *addr = (char *)PGROUNDUP((unsigned long)start);
    for (; (unsigned long)(addr) + PGSIZE <= (unsigned long)end; addr += PGSIZE) {
        kfree((unsigned long)addr);
    }
}

void mm_init(void) {
    printk("...mm_init ");
    kfreerange(_ekernel, (char *)(PHY_END + PA2VA_OFFSET));
    printk("done!\n");
}
