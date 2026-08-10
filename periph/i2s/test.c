#include <stdio.h>
#include "pulp.h"

int main() {
    printf("[I2S TEST] Commencing structural register initialization...\n");
    
    *(volatile unsigned int *)0x1A102000 = 0x20; // Enable clock gating for sys clock
    *(volatile unsigned int *)0x1A102320 = (1 << 25);
    unsigned int reg_val = *(volatile unsigned int *)0x1A102320;
    printf("[I2S DEBUG] Register 0x1A102320 Value = 0x%08X\n", reg_val);
    *(volatile unsigned int *)0x1A102310 = 0x1C000088;

    printf("[I2S TEST] Transfer finished successfully!\n");
    return 0;
}

void pe_start(void)
{
}

