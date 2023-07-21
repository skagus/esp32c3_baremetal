
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "esp32c3.h"
extern int main();

extern unsigned char etext;
extern unsigned char sdata;
extern unsigned char edata;
extern unsigned char sbss;
extern unsigned char ebss;

extern unsigned int _data_lma;

extern unsigned char _vector_table;

extern void _delay(int nCnt);

#if 0
extern void _reset();
void __attribute__((section (".text.entry"))) start() 
{
    _reset();
}
#endif

typedef struct {
    int     baut_rate;
    int data_bits;
    int  exist_parity;
    int   parity;    // chip size in byte
    int  stop_bits;
    int     flow_ctrl;
    uint8_t          buff_uart_no;  //indicate which uart use tx/rx buffer
} UartDevice;

extern void mydelay(int nCnt);
extern int uart_tx_one_char(char nCh);
extern int usb_uart_tx_one_char(char nCh);
extern void disable_default_watchdog();
extern void uart_tx_flush(uint8_t uart_no);

extern void ets_install_uart_printf();
extern int ets_printf(const char *fmt, ...);

int gnData = 5;
int gnZero;

void __attribute__((naked)) startup()
{
	int nData;
	int nData2;
	int nZero, nZero2;
	soc_init();
	wdt_disable();

	nData = gnData;
	nZero = gnZero;

	// bootloader load non-zero (.data) area from flash.
//	memcpy(&sdata, &_data_lma, &edata - &sdata);
	memset(&sbss, 0, &ebss - &sbss);

	nData2 = gnData;
	nZero2 = gnZero;

	ets_install_uart_printf();
	mydelay(10000000);

	gpio_output(LED0);
	gpio_output(LED1);
	gpio_toggle(LED0);

	int nCnt = 0;
	while (nCnt < 10)
	{
		gpio_toggle(LED0);
		gpio_toggle(LED1);
		mydelay(1000000);
		ets_printf("\t\tSeq: %d\n", nCnt++);
		ets_printf("DAT5 %X -> %X, ZI %X -> %X\n", 
					nData, nData2, nZero, nZero2);
		ets_printf("sdata %X %d, sbss: %X, %d\n",
				&sdata, &edata - &sdata, &sbss, &ebss - &sbss);

		ets_printf("gnData %X, gnZero: %X\n", &gnData, &gnZero);
	}

	main();
}

__attribute__((interrupt("machine"))) void EXC_IRQHandler()
{
	uint32_t nSrc;
	asm("csrr %0, mcause" : "=r"(nSrc));
	uint32_t nPC;
	asm("csrr %0, mepc":"=r"(nPC));

	switch (nSrc)
	{
		case 0: // Inst addr misaligned.
		{
			uint8_t *aInst = (uint8_t *)nPC;
			printf("Inst Align Fault:%X, PC:%X, Inst:[%X:%X:%X:%X]\n",
				nSrc, nPC, aInst[0], aInst[1], aInst[2], aInst[3]);
			while (1)
				;
			break;
		}
		case 1: // Inst Access fault.
		{
			uint8_t *aInst = (uint8_t *)nPC;
			printf("Inst Access Fault:%X, PC:%X, Inst:[%X:%X:%X:%X]\n",
				nSrc, nPC, aInst[0], aInst[1], aInst[2], aInst[3]);
			while (1)
				;
			break;
		}
		case 2: // Illegal inst.
		{
			uint8_t *aInst = (uint8_t *)nPC;
			printf("Inst Illegal:%X, PC:%X, Inst:[%X:%X:%X:%X]\n",
				nSrc, nPC, aInst[0], aInst[1], aInst[2], aInst[3]);
			while (1)
				;
			break;
		}
		case 3: // Break point.
		{
			uint8_t *aInst = (uint8_t *)nPC;
			printf("Inst Breakpoint:%X, PC:%X, Inst:[%X:%X:%X:%X]\n",
				nSrc, nPC, aInst[0], aInst[1], aInst[2], aInst[3]);
			while (1)
				;
			break;
		}
		case 4: // Load addr misaligned.
		case 5: // Load access fault.
		case 6: // Store/AMO addr misaligned.
		case 7: // Store/AMO access fault.
		default:
		{
			uint32_t nMem;
			asm("csrr %0, mbadaddr" : "=r"(nMem));
			printf("Data Fault:%X, PC:%X, Mem:%X\n", nSrc, nPC, nMem);
			while (1)
				;
			break;
		}
		case 8:	  // ecall from U mode.
		case 9:	  // ecall from S mode.
		case 0xA: // ecall from H mode.
		case 0xB: // ecall from M mode.
		{
			uint32_t nPC;
			asm volatile("csrr %0, mepc" : "=r"(nPC));
			//			UT_Printf("ECall :%X, P:%X, %X, %X, %X\n",
			//						nSrc, nParam0, nParam1, nParam2, nParam3);
			nPC += 4; // Inc PC because it is NOT fault.
			asm volatile("csrw mepc, %0" ::"r"(nPC));
			// WCH Fast interrupt때문에 parameter return은 안됨.
			uint32_t nRet = 0xFABC;
			asm volatile("add a0, %0, zero" ::"r"(nRet));
			break;
		}
	}
}