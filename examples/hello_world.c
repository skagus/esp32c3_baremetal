#include <stdio.h>

void _delay(int nCnt)
{
    while(--nCnt > 0)
    {
        asm("nop\r\n");
    }
}

extern int gnBss;
extern const unsigned int gnConst;
extern int gnData;

int main(void)
{
    int nLoop = 10;
    _delay(20000000);   // Wait USB CDC ready.
    printf("Hello World : %s\n", __DATE__);

    printf("Const   0x%X, %p\n", gnConst, &gnConst);
    printf("Data:   0x%X, %p\n", gnData, &gnData);
    printf("BSS :   0x%X\n", &gnBss);

    while(nLoop--)
    {
        printf("Loop ZI %d\n", gnBss);
        _delay(20000000);
        gnBss ++;
    }
    while(1);
    return 0;
}
