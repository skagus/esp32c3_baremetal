
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "esp32c3.h"
#define SECTION_CHECK		(0)

#define printf(...)			ets_printf(__VA_ARGS__)

extern int main();
extern int uart_tx_one_char(char nCh);
extern int usb_uart_tx_one_char(char nCh);
extern void disable_default_watchdog();
extern void uart_tx_flush(uint8_t uart_no);
extern void ets_install_uart_printf();
extern int ets_printf(const char *fmt, ...);

extern unsigned char sbss;
extern unsigned char ebss;

#if (SECTION_CHECK == 1)
extern unsigned char sdata;
extern unsigned char edata;

extern int gnSData;
extern int gnSBss;
extern unsigned int gnConst;

int gnData = 5;
int gnZero;
#endif

void __attribute__((naked)) startup()
{
	soc_init();
	wdt_disable();

#if (SECTION_CHECK == 1)
	int nDataBefore;
	int nBssBefore;

	nDataBefore = gnData;
	nBssBefore = gnZero;
#endif
	// bootloader load non-zero (.data) area from flash.
//	memcpy(&sdata, &_data_lma, &edata - &sdata);
	memset(&sbss, 0, &ebss - &sbss);

	ets_install_uart_printf();
	delay_ms(1000);

	gpio_output(LED0);
	gpio_output(LED1);
	gpio_toggle(LED0);

#if (SECTION_CHECK == 1)
	printf("\n\nBuilt Time : %s %s\n", __DATE__, __TIME__);
	printf("data %p ~ %p, bss: %p ~ %p\n", &sdata, &edata, &sbss, &ebss);

	printf("DAT5 Addr: %p, Value: %X -> %X\n", &gnData, nDataBefore, gnData);
	printf("BSS Addr: %p, Value: %X -> %X\n", &gnZero, nBssBefore, gnZero);

	printf("Data: %X @ %X\n", gnSData, &gnSData);
	printf("BSS : %X @ %X\n", gnSBss, &gnSBss);
	printf("Const: %X @ %X\n", gnConst, &gnConst);
#endif

	int nCnt = 0;
	while (nCnt < 4)
	{
		gpio_toggle(LED0);
		gpio_toggle(LED1);
		delay_ms(1000);
		printf("\t\tTest Loop with LED: %d\n", nCnt++);
	}

	main();
}


__attribute__((interrupt("machine"))) void EXC_Handler()
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
		default:
		{
			printf("Exception Src:%X, PC:%X\n", nSrc, nPC);
			while (1)
				;
			break;
		}
	}
}