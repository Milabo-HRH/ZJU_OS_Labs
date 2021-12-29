// arch/riscv/kernel/vm.c
#include "../include/defs.h"
/* early_pgtbl: 用于 setup_vm 进行 1GB 的 映射。 */
extern char _stext[];
extern char _etext[];
extern char _srodata[];
extern char _erodata[];
extern char _sdata[];
extern char _edata[];
extern char _ekernel[];
extern char uapp_start[];
unsigned long  early_pgtbl[512] __attribute__((__aligned__(0x1000)));

void setup_vm(void) {
    /* 
    1. 由于是进行 1GB 的映射 这里不需要使用多级页表 
    2. 将 va 的 64bit 作为如下划分： | high bit | 9 bit | 30 bit |
        high bit 可以忽略
        中间9 bit 作为 early_pgtbl 的 index
        低 30 bit 作为 页内偏移 这里注意到 30 = 9 + 9 + 12， 即我们只使用根页表， 根页表的每个 entry 都对应 1GB 的区域。 
    3. Page Table Entry 的权限 V | R | W | X 位设置为 1
    */
//    printk("final\n");
    early_pgtbl[2] = 0x000000002000000F;
    early_pgtbl[384] = 0x000000002000000F;
}

/* swapper_pg_dir: kernel pagetable 根目录， 在 setup_vm_final 进行映射。 */
unsigned long  swapper_pg_dir[512] __attribute__((__aligned__(0x1000)));

void setup_vm_final(void) {
    printk("..setup_vm_final "); 
    memset(swapper_pg_dir, 0x0, PGSIZE);

    // No OpenSBI mapping required
    // printk("final\n");
    // mapping kernel text X|-|R|V

    create_mapping(swapper_pg_dir, _stext, _stext-PA2VA_OFFSET, (unsigned long)_srodata-(unsigned long)_stext, 11);
    
    // mapping kernel rodata -|-|R|V
    create_mapping(swapper_pg_dir, _srodata, _srodata-PA2VA_OFFSET, (unsigned long)_sdata-(unsigned long)_srodata, 3);
    
    // mapping other memory -|W|R|V
    create_mapping(swapper_pg_dir, _sdata, _sdata-PA2VA_OFFSET, PHY_SIZE-PGROUNDUP(0x3000), 7);
    
    // set satp with swapper_pg_dir
    
    //YOUR CODE HERE
    unsigned long swp = (unsigned long)swapper_pg_dir-PA2VA_OFFSET;
    asm volatile (
        "mv t0, %[addr]\n"
        "li t1, 0x8000000000000000\n"
        "srl t0, t0, 12\n"
        "add t0, t0, t1\n"
        "csrrw x0, satp, t0\n"
        :   
        :[addr] "r" (swp)
        :"memory"
    );
    // flush TLB
    // printk("done");
    asm volatile ("sfence.vma zero, zero");
    printk("done!\n");
    return;
}


/* 创建多级页表映射关系 */
create_mapping(unsigned long *pgtbl, unsigned long va, unsigned long pa, unsigned long sz, int perm) {
    /*
    pgtbl 为根页表的基地址
    va, pa 为需要映射的虚拟地址、物理地址
    sz 为映射的大小
    perm 为映射的读写权限

    创建多级页表的时候可以使用 kalloc() 来获取一页作为页表目录
    可以使用 V bit 来判断页表项是否存在
    */
//    printk("start mapping\n");
    unsigned long *pg2nd, *pg3rd;
    unsigned long tb1, tb2, tb3;
    unsigned long size = sz;
    while (size <0x8000000000000000 && size!=0)
   {
       /* code */
    //    printk("size = %x\n", size);
        tb1 = (va >> 30) & 0x1ff;
        tb2 = (va >> 21) & 0x1ff;
        tb3 = (va >> 12) & 0x1ff;
        if (!(pgtbl[tb1]&0x1)) {
                pg2nd = (unsigned long*)(kalloc());
                memset(pg2nd, 0x0, PGSIZE);
                pgtbl[tb1] = ((((unsigned long)pg2nd-PA2VA_OFFSET)&(~0xfffl))>>2)|0x1;
        } else {
            pg2nd = ((pgtbl[tb1]&(0xFFFFFFFFFFFFFC00))<<2)+PA2VA_OFFSET;
        }
        
        if (!(pg2nd[tb2]&0x1)) {
                pg3rd = (unsigned long*)(kalloc());
                memset(pg3rd, 0x0, PGSIZE);
                pg2nd[tb2] = ((((unsigned long)pg3rd-PA2VA_OFFSET)&(~0xfffl))>>2)|0x1;
                pg3rd[tb3] = ((pa&(~0xfffl))>>2)|perm;
        } else {
            pg3rd = ((pg2nd[tb2]&(0xFFFFFFFFFFFFFC00))<<2)+PA2VA_OFFSET;
            pg3rd[tb3] = ((pa&(~0xfffl))>>2)|perm;
        }
        va += PGSIZE;
        pa += PGSIZE;
        size -= PGSIZE;
   }  
//    printk("end mapping\n");
   
}

unsigned long* user_paging_init() {
    unsigned long* pgtbl = (unsigned long*) kalloc();
    unsigned long* userStack = (unsigned long*) kalloc();
    for (unsigned long i = 0; i < 512; i++)
    {
        pgtbl[i] = swapper_pg_dir[i];
    }
    
    create_mapping(pgtbl, 0, (unsigned long)uapp_start - PA2VA_OFFSET, PGSIZE, 0x1F);
    create_mapping(pgtbl, USER_END-PGSIZE, (unsigned long)userStack - PA2VA_OFFSET, PGSIZE, 0x1F);
    return pgtbl;
}