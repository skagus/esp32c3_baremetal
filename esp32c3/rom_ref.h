#pragma once
#include <stdint.h>

int uart_tx_one_char(char nCh);
int usb_uart_tx_one_char(char nCh);
void disable_default_watchdog(void);
void uart_tx_flush(uint8_t uart_no);
void ets_install_uart_printf(void);
int ets_printf(const char *fmt, ...);
int esp_pp_rom_version_get(void);
void ets_update_cpu_frequency(uint32_t ticks_per_us);
