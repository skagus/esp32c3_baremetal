
#include <stdio.h>
#include <string.h>
extern int main();

extern unsigned char etext;
extern unsigned char sdata;
extern unsigned char edata;
extern unsigned char sbss;
extern unsigned char ebss;

extern unsigned int _data_lma;

extern unsigned char _vector_table;

extern void _delay(int nCnt);

void __attribute__((naked)) startup()
{
    asm volatile("la gp, __global_pointer$");
    asm volatile("la sp, __stack_top");

    asm volatile("csrw mtvec, %0":: "r"((uint8_t *)(&_vector_table) + 1));
#if 0
    _delay(20000000);
    printf("DATA: %X, %X <-- %X, %X\n",
        &sdata, &edata, &_data_lma, *(unsigned int*)&_data_lma);
    printf("ZI: %X, %X\n", &sbss, &ebss);
#endif
    memcpy(&sdata, &_data_lma, &edata - &sdata);
    memset(&sbss, 0, &ebss - &sbss);
    main();
}


