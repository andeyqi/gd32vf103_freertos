#ifndef __ATOMIC_H__
#define __ATOMIC_H__

static inline void atomic_add(int i, unsigned int *p)
{
    unsigned int tmp;
    int result;

    asm volatile("# atomic_add\n"
    "1: lr.w    %[tmp], (%[p])\n"
    "   add %[tmp], %[i], %[tmp]\n"
    "   sc.w    %[result], %[tmp], (%[p])\n"
    "   bnez    %[result], 1b\n"
    : [result]"=&r" (result), [tmp]"=&r" (tmp), [p]"+r" (p)
    : [i]"r" (i)
    : "memory");
}


#endif /* end of __ATOMIC_H__ */