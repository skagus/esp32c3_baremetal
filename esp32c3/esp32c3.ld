MEMORY
{
    flash (r): org = 0x0000000, len = 4M
    irom (x): org = 0x42000000, len = 4M
    drom (r): org = 0x3C000000, len = 4M
    ram (rw): org = 0x3FC80000, len = 320K
    rtc_ram (rx): org = 0x50000000, len = 8K
}
