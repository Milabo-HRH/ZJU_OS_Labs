#include "types.h"
#include "sbi.h"
#define uintptr_t unsigned int


struct sbiret sbi_ecall(int ext, int fid, uint64 arg0,
                        uint64 arg1, uint64 arg2,
                        uint64 arg3, uint64 arg4,
                        uint64 arg5)
{
    // unimplemented
    struct sbiret ret;
    __asm__ volatile(
//            "mv a7, %[ext]\n"
//            "mv a6, %[fid]\n"
//            "mv a5, %[arg5]\n"
//            "mv a4, %[arg4]\n"
//            "mv a3, %[arg3]\n"
//            "mv a2, %[arg2]\n"
//            "mv a1, %[arg1]\n"
//            "mv a0, %[arg0]\n"
//            "ecall\n"
        "mv a7, %[ext]\n"
        "mv a6, %[fid]\n"
        "mv a0, %[arg0]\n"
        "mv a1, %[arg1]\n"
        "mv a2, %[arg2]\n"
        "mv a3, %[arg3]\n"
        "mv a4, %[arg4]\n"
        "mv a5, %[arg5]\n"
        "ecall\n"
            "mv %[ret0], a0\n"
            "mv %[ret1], a1"
            : [ret0] "=r" (ret.error), [ret1] "=r" (ret.value)
            : [arg0] "r" (arg0), [arg1] "r" (arg1), [arg2] "r" (arg2), [arg3] "r" (arg3),\
            [arg4] "r" (arg4), [arg5] "r" (arg5), [ext] "r" (ext), [fid] "r" (fid)
//            : [ext] "r" (ext), [fid] "r" (fid),[arg0] "r" (arg0),[arg1] "r" (arg1),\
//              [arg2] "r" (arg2),[arg3] "r" (arg3),[arg4] "r" (arg4),[arg5] "r" (arg5)
            : "memory"
            );
    return ret;
}
//struct sbiret sbi_ecall(int ext, int fid, uint64 arg0,
//                        uint64 arg1, uint64 arg2,
//                        uint64 arg3, uint64 arg4,
//                        uint64 arg5)
//{
//    struct sbiret ret;
//    __asm__ volatile (
//    "mv a7, %[ext]\n"
//    "mv a6, %[fid]\n"
//    "mv a0, %[arg0]\n"
//    "mv a1, %[arg1]\n"
//    "mv a2, %[arg2]\n"
//    "mv a3, %[arg3]\n"
//    "mv a4, %[arg4]\n"
//    "mv a5, %[arg5]\n"
//    "ecall\n"
//    "mv %[error],a0\n"
//    "mv %[value],a1"
//    : [error] "=r" (ret.error),[value] "=r" (ret.value)
//    : [ext] "r" (ext), [fid] "r" (fid),[arg0] "r" (arg0),[arg1] "r" (arg1),\
//          [arg2] "r" (arg2),[arg3] "r" (arg3),[arg4] "r" (arg4),[arg5] "r" (arg5)
//    : "memory"
//    );
//    return ret;
//    // unimplemented
//}