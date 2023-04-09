
#include <stdio.h>
extern int main();

extern unsigned int etext;

extern unsigned int sdata;
extern unsigned int edata;

extern unsigned int sbss;
extern unsigned int ebss;

extern unsigned int _data_lma;

extern void _delay(int nCnt);

void startup()
{
#if 0    
    _delay(20000000);
    printf("DATA: %X, %X <-- %X, %X\n",
        &sdata, &edata, &_data_lma, *(unsigned int*)&_data_lma);
    printf("ZI: %X, %X\n", &sbss, &ebss);
#endif
    memcpy(&sdata, &_data_lma, (unsigned int)&edata - (unsigned int)&sdata);
    memset(&sbss, 0, (unsigned int)&ebss - (unsigned int)&sbss);
    main();
}