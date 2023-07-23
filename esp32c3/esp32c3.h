// Copyright (c) 2022 Cesanta
// All rights reserved

#pragma once

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#define BIT(x) ((uint32_t)1U << (x))
#define REG(x) (*(volatile uint32_t *)(x))

/****************************************************************************/
// Register base address, Each groups are 4KB sized(except CACHE:32KB)
#define C3_UART 				0x60000000
#define C3_SPI1 				0x60002000
#define C3_SPI0 				0x60003000
#define C3_GPIO 				0x60004000
//#define C3_FE2 					0x60005000
//#define C3_FE 					0x60006000
#define C3_TIMER 				0x60007000
#define C3_LOWPOWER				0x60008000
//#define C3_EFUSE 				0x60008800
#define C3_IO_MUX 				0x60009000
//#define C3_RTC_I2C 				0x6000e000
#define C3_UART1 				0x60010000
#define C3_I2C_EXT 				0x60013000
#define C3_UHCI0 				0x60014000
#define C3_REMOT				0x60016000
#define C3_LEDC 				0x60019000
#define C3_EFUSEC 				0x6001A000
//#define C3_NRX 					0x6001CC00
//#define C3_BB 					0x6001D000
#define C3_TIMERGRP0 			0x6001F000
#define C3_TIMERGRP1 			0x60020000
#define C3_SYSTIMER 			0x60023000
#define C3_SPI2 				0x60024000
//#define C3_SYSCON 				0x60026000
#define C3_APB_CTRL 			0x60026000
#define C3_TWAI 				0x6002B000
#define C3_I2S0 				0x6002D000

#define C3_AES 					0x6003A000
#define C3_SHA 					0x6003B000
#define C3_RSA 					0x6003C000
#define C3_DIGITAL_SIGNATURE 	0x6003D000
#define C3_HMAC 				0x6003E000
#define C3_GDMA 				0x6003F000

#define C3_ADC		 			0x60040000
#define C3_USB_JTAG	 			0x60043000

#define C3_SYSTEM 				0x600C0000
#define C3_SENSITIVE			0x600C1000
#define C3_INTERRUPT			0x600C2000
#define C3_CACHE				0x600C4000		// SIZE: 32KB
//#define C3_MMU_TABLE			0x600C5000
#define C3_EXT_MEM_AES			0x600CC000
#define C3_ASSIST_DEBUG 		0x600CE000
#define C3_WORLD_CTRL			0x600D0000


#define C3_WORLD_CNTL 			0x600D0000
#define C3_DPORT_END 			0x600D3FFC

//#define GPIO_OUT_EN		(0x20)
//#define GPIO_OUT_FUNC	(0x554)
//#define GPIO_IN_FUNC	(0x154)

// Perform `count` "NOP" operations
static inline void spin(volatile unsigned long count)
{
	while (count--)
	{
		asm volatile("nop");
	}
}

static inline uint64_t systick(void)
{
	REG(C3_SYSTIMER + 0x4) = BIT(30); // read out system timer value.
	spin(1);
	return ((uint64_t)REG(C3_SYSTIMER + 0x40) << 32)  |
			(uint64_t)REG(C3_SYSTIMER + 0x44);
}

static inline uint64_t uptime_us(void)
{
	return systick() >> 4;
}

static inline void delay_us(unsigned long us)
{
	uint64_t until = uptime_us() + us;
	while (uptime_us() < until)
	{
		spin(1);
	}
}

static inline void delay_ms(unsigned long ms)
{
	delay_us(ms * 1000);
}

static inline void wdt_disable(void)
{
	REG(C3_LOWPOWER + 0xA8) = 0x50D83AA1;	// Disable Write Protection.
	// REG(C3_RTCCNTL)[36] &= BIT(31);    // Disable RTC WDT
	REG(C3_LOWPOWER + 0x90) = 0; // Disable RTC WDT
	REG(C3_LOWPOWER + 0x8C) = 0;
//	REG(C3_LOWPOWER + 0xA8) = 0xABCDEF00;	// Enable Write Protection.

	// bootloader_super_wdt_auto_feed()
	REG(C3_LOWPOWER + 0xB0) = 0x8F1D312A;	// Disable Write Protection.
	REG(C3_LOWPOWER + 0xAC) |= BIT(31); // Auto feed
//	REG(C3_LOWPOWER + 0xB4) = 0; // RTC_CNTL_SW_CPU_STALL_REG
//	REG(C3_LOWPOWER + 0xB0) = 0xABCDEF00;	// Enable Write Protection.

	REG(C3_TIMERGRP0 + 0x64) = 0x50D83AA1;	// Disable Write Protection.
	REG(C3_TIMERGRP0 + 0xFC) &= ~BIT(29);
	REG(C3_TIMERGRP0 + 0x48) = 0; // Disable TG0 WDT
//	REG(C3_TIMERGRP0 + 0x64) = 0xABCDEF00;	// Enable Write Protection.

	REG(C3_TIMERGRP1 + 0x64) = 0x50D83AA1;	// Disable Write Protection.
	REG(C3_TIMERGRP1 + 0xFC) &= ~BIT(29);
	REG(C3_TIMERGRP1 + 0x48) = 0; // Disable TG1 WDT
//	REG(C3_TIMERGRP1 + 0x64) = 0xABCDEF00;	// Enable Write Protection.
}

#if 0
static inline void wifi_get_mac_addr(uint8_t mac[6])
{
	uint32_t a = REG(C3_EFUSE)[17], b = REG(C3_EFUSE)[18];
	mac[0] = (b >> 8) & 255, mac[1] = b & 255, mac[2] = (uint8_t)(a >> 24) & 255;
	mac[3] = (a >> 16) & 255, mac[4] = (a >> 8) & 255, mac[5] = a & 255;
}
#endif

static inline void soc_enable_int(int int_no)
{
#define INTERRUPT_CORE0_CPU_INT_ENABLE_REG			(0x104)

	REG(C3_INTERRUPT + INTERRUPT_CORE0_CPU_INT_ENABLE_REG) |= BIT(int_no);
}

static inline void test_ecall(void)
{
	asm("ecall" ::);
}

static inline void soc_int(bool b_en)
{
	uint32_t nTmp;
	if(b_en)
	{
		asm("csrr %0, mstatus" : "=r"(nTmp));
		nTmp |= BIT(3);
		asm("csrw mstatus, %0"::"r"(nTmp));
	}
	else
	{
		asm("csrr %0, mstatus" : "=r"(nTmp));
		nTmp &= ~BIT(3);
		asm("csrw mstatus, %0"::"r"(nTmp));
	}
}

static inline void soc_init(void)
{
	// Init clock. TRM 6.2.4.1
	REG(C3_SYSTEM + 0x8) &= ~3U;
	REG(C3_SYSTEM + 0x8) |= BIT(0) | BIT(2);
	REG(C3_SYSTEM + 0x58) = BIT(19) | (40U << 12) | BIT(10);
	// REG(C3_RTCCNTL)[47] = 0; // RTC_APB_FREQ_REG -> freq >> 12
#if 0
  // Configure system clock timer, TRM 8.3.1, 8.9
  REG(C3_TIMERGROUP0)[1] = REG(C3_TIMERGROUP0)[2] = 0UL;  // Reset LO and HI
  REG(C3_TIMERGROUP0)[8] = 0;                             // Trigger reload
  REG(C3_TIMERGROUP0)[0] = (83U << 13) | BIT(12) | BIT(29) | BIT(30) | BIT(31);
#endif
}

// API GPIO

static inline void gpio_int_en(int pin)
{
	REG(C3_GPIO + 0x74 + (4 * pin)) |= BIT(7) | BIT(13);// rising edge.
#define INTERRUPT_CORE0_GPIO_INTERRUPT_PRO_MAP_REG	(0x40)
	REG(C3_INTERRUPT + INTERRUPT_CORE0_GPIO_INTERRUPT_PRO_MAP_REG) = 9;
}

static inline void gpio_output_enable(int pin, bool enable)
{
	REG(C3_GPIO + 0x20) &= ~BIT(pin);
	REG(C3_GPIO + 0x20) |= (enable ? 1U : 0U) << pin;
}

static inline void gpio_output(int pin)
{
	REG(C3_GPIO + 0x554 + (4 * pin)) = BIT(9) | BIT(7); // Simple out, TRM 5.5.3
	gpio_output_enable(pin, 1);
}

static inline void gpio_write(int pin, bool value)
{
	REG(C3_GPIO + 0x4) &= ~BIT(pin); // Clear first
	REG(C3_GPIO + 0x4) |= (value ? 1U : 0U) << pin; // Then set
}

static inline void gpio_toggle(int pin)
{
	REG(C3_GPIO + 0x4) ^= BIT(pin);
}

static inline void gpio_input(int pin)
{
	gpio_output_enable(pin, 0); // Disable output
	REG(C3_IO_MUX + 4*(1 + pin)) = BIT(9) | BIT(8); // Enable pull-up
}

static inline bool gpio_read(int pin)
{
	return REG(C3_GPIO + (15 * 4)) & BIT(pin) ? 1 : 0;
}

// API SPI

struct spi
{
	int miso, mosi, clk, cs; // Pins
	int spin;				 // Number of NOP spins for bitbanging
};

static inline void spi_begin(struct spi *spi)
{
	gpio_write(spi->cs, 0);
}

static inline void spi_end(struct spi *spi)
{
	gpio_write(spi->cs, 1);
}

static inline bool spi_init(struct spi *spi)
{
	if (spi->miso < 0 || spi->mosi < 0 || spi->clk < 0)
		return false;
	gpio_input(spi->miso);
	gpio_output(spi->mosi);
	gpio_output(spi->clk);
	if (spi->cs >= 0)
	{
		gpio_output(spi->cs);
		gpio_write(spi->cs, 1);
	}
	return true;
}

// Send a byte, and return a received byte
static inline unsigned char spi_txn(struct spi *spi, unsigned char tx)
{
	unsigned count = spi->spin <= 0 ? 9 : (unsigned)spi->spin;
	unsigned char rx = 0;
	for (int i = 0; i < 8; i++)
	{
		gpio_write(spi->mosi, tx & 0x80); // Set mosi
		spin(count);					  // Wait half cycle
		gpio_write(spi->clk, 1);		  // Clock high
		rx = (unsigned char)(rx << 1);	  // "rx <<= 1" gives warning??
		if (gpio_read(spi->miso))
			rx |= 1;				   // Read miso
		spin(count);				   // Wait half cycle
		gpio_write(spi->clk, 0);	   // Clock low
		tx = (unsigned char)(tx << 1); // Again, avoid warning
	}
	return rx; // Return the received byte
}

// API WS2812
static inline void ws2812_show(int pin, const uint8_t *buf, size_t len)
{
	unsigned long delays[2] = {2, 6};
	for (size_t i = 0; i < len; i++)
	{
		for (uint8_t mask = 0x80; mask; mask >>= 1)
		{
			int i1 = buf[i] & mask ? 1 : 0, i2 = i1 ^ 1; // This takes some cycles
			gpio_write(pin, 1);
			spin(delays[i1]);
			gpio_write(pin, 0);
			spin(delays[i2]);
		}
	}
}

// Default settings for board peripherals

#ifndef LED1
//#define LED1 2 // Default LED pin
#define LED0 12 // Default LED pin
#define LED1 13 // Default LED pin
#endif



#ifndef BTN1
#define BTN1 9 // Default user button pin
#endif
