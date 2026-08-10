/*
 * Copyright (C) 2018 ETH Zurich and University of Bologna
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <pulp.h>
#include <stdio.h>
#include <implem/delay.h>

/* Hardware Base Address */
#define AXI_GCD_BASE  0x1A400000

/* Register Offsets */
#define REG_CTRL    0x00
#define REG_A       0x04
#define REG_B       0x08
#define REG_R       0x0C

/* Driver Macros */
#define WR_REG(off, val) (*(volatile uint32_t*)(AXI_GCD_BASE + (off)) = (val))
#define RD_REG(off)      (*(volatile uint32_t*)(AXI_GCD_BASE + (off)))

int main()
{
  printf("Hello SS58!\r\n");
  pos_delay_busy_us(10);
  printf("Hello after delay!\r\n");

  WR_REG(REG_A, 35);
  WR_REG(REG_B, 25);
  WR_REG(REG_CTRL, 1);
  WR_REG(REG_CTRL, 0);
  while ( !(RD_REG(REG_CTRL) & (1 << 1)) );
  printf("Result GCD(35,25): %d\r\n", RD_REG(REG_R));

  WR_REG(REG_A, 128);
  WR_REG(REG_B, 8);
  WR_REG(REG_CTRL, 1);
  WR_REG(REG_CTRL, 0);
  while ( !(RD_REG(REG_CTRL) & (1 << 1)) );
  printf("Result GCD(128,8): %d\r\n", RD_REG(REG_R));

  WR_REG(REG_A, 341);
  WR_REG(REG_B, 257);
  WR_REG(REG_CTRL, 1);
  WR_REG(REG_CTRL, 0);
  while ( !(RD_REG(REG_CTRL) & (1 << 1)) );
  printf("Result GCD(341,257): %d\r\n", RD_REG(REG_R));
  
  pos_delay_busy_us(10);
  
  return 0;
}

void pe_start(void)
{
}

