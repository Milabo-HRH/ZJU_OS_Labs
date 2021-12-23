//
// Created by h3root on 11/13/21.
//
//arch/riscv/kernel/proc.c
#include "../include/proc.h"
#include "../../../include/rand.h"

extern void __dummy();

extern void __switch_to(struct task_struct *prev, struct task_struct *next);

struct task_struct *idle; // idle process
struct task_struct *current; // 指向当前运⾏线程的 `task_struct`
struct task_struct *task[NR_TASKS]; // 线程数组，所有的线程都保存在此
int flag=0;
void task_init() {
    // 1. 调⽤ kalloc() 为 idle 分配⼀个物理⻚
    idle = (struct task_struct *) kalloc();
    // 2. 设置 state 为 TASK_RUNNING;
    idle->state = TASK_RUNNING;
    // 3. 由于 idle 不参与调度 可以将其 counter / priority 设置为 0
    idle->counter = 0;
    idle->priority = 0;
    // 4. 设置 idle 的 pid 为 0
    idle->pid = 0;
    // 5. 将 current 和 task[0] 指向 idle
    current = idle;
    task[0] = idle;
    /* YOUR CODE HERE */
    // 1. 参考 idle 的设置, 为 task[1] ~ task[NR_TASKS - 1] 进⾏初始化
    // 2. 其中每个线程的 state 为 TASK_RUNNING, counter 为 0, priority 使⽤ rand() 来设置,pid 为该线程在线程数组中的下标。
    // 3. 为 task[1] ~ task[NR_TASKS - 1] 设置 `thread_struct` 中的 `ra` 和 `sp`,
    // 4. 其中 `ra` 设置为 __dummy （⻅ 4.3.2）的地址， `sp` 设置为 该线程申请的物理⻚的⾼地址
    /* YOUR CODE HERE */
    for (int i = 1; i < NR_TASKS; ++i) {
        task[i] = (struct task_struct *) kalloc();
        task[i]->state = TASK_RUNNING;
        task[i]->counter = 0;
        task[i]->pid = i;
        task[i]->priority = rand();
        task[i]->thread.ra = (unsigned long) &__dummy;
        task[i]->thread.sp = PGROUNDUP((unsigned long) task[i]);
    }
    printk("...proc_init done!\n");
}

void dummy() {
    unsigned long MOD = 1000000007;
    unsigned long auto_inc_local_var = 0;
    int last_counter = -1;
    while (1) {
        if (last_counter == -1 || current->counter != last_counter || flag) {
            last_counter = current->counter;
            auto_inc_local_var = (auto_inc_local_var + 1) % MOD;
            printk("[PID = %d] is running. thread space begin at = %lx\n", current->pid,
                   current);
            flag = 0;
        }
    }
}

void switch_to(struct task_struct *next) {
    /* YOUR CODE HERE */
    if (current->pid != next->pid) {
        struct task_struct* temp;
        temp = current;
        current = next;
        __switch_to(temp, next);
        flag = 1;
    }
}

void do_timer(void) {
    /* 1. 如果当前线程是 idle 线程 直接进⾏调度 */
    /* 2. 如果当前线程不是 idle 对当前线程的运⾏剩余时间减 1
    若剩余时间任然⼤于0 则直接返回 否则进⾏调度 */
    /* YOUR CODE HERE */
    if (idle->pid == current->pid) {
        schedule();
    } else {
        current->counter--;
        if (current->counter <= 0) {
            schedule();
        }
    }
}



#ifdef DSJF
void schedule(void) {
    int shortest = 0;
    for (int i = 1; i < NR_TASKS; ++i) {
        if (task[i]->state == TASK_RUNNING)
            if ((task[i]->counter < task[shortest]->counter && task[i]->counter > 0) || task[shortest]->counter <= 0) {
                shortest = i;
            }
    }
    if (task[shortest]->counter <= 0) {
        for (int i = 1; i < NR_TASKS; ++i) {
            task[i]->counter = rand();
            printk("SET [PID = %d COUNTER = %d]\n", i, task[i]->counter);
        }
        shortest = 0;
        for (int i = 1; i < NR_TASKS; ++i) {
            if (task[i]->state == TASK_RUNNING)
                if ((task[i]->counter < task[shortest]->counter && task[i]->counter > 0) ||
                    task[shortest]->counter <= 0) {
                    shortest = i;
                }
        }
    }
    printk("switch to [PID = %d COUNTER = %d]\n", shortest, task[shortest]->counter);
    switch_to(task[shortest]);
}
#else
void schedule(void) {
    int i, next, c;
    while (1) {
        c = -1;
        next = 0;
        i = NR_TASKS;
        while (--i) {
            if (task[i]->state!=TASK_RUNNING)
                continue;
            if ((int)(task[i]->counter) > c) {
                c = task[i]->counter;
                next = i;
            }
        }
        if (c>0) break;
        for(i = 1; i<NR_TASKS  ; ++i) {
            if (task[i]->state == TASK_RUNNING) {
                (task[i])->counter = ((task[i])->counter >> 1) +
                                     (task[i])->priority;
                printk("SET [PID = %d COUNTER = %d]\n", i, task[i]->counter);
            }
        }
    }
    printk("switch to [PID = %d COUNTER = %d]\n", next, task[next]->counter);
    switch_to(task[next]);
}
#endif

