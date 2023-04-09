#include <stdio.h>

void _delay(int nCnt)
{
    while(--nCnt > 0)
    {
        asm("nop\r\n");
    }
}

extern int gnVal;
extern const unsigned int gnConst;

int main(void)
{
    _delay(20000000);   // Wait USB CDC ready.
    printf("Hello World : %s\n", __DATE__);

    printf("Const %X\n", gnConst);
    while(1)
    {
        printf("Loop ZI %d\n", gnVal);
        _delay(20000000);
        gnVal ++;
    }
    return 0;
}
