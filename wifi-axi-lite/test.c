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
#define AXI_XPU_BASE  		0x1A400000
#define AXI_TXINTF_BASE  	0x1A401000
#define AXI_RXINTF_BASE  	0x1A402000
#define AXI_TX_BASE  		0x1A403000
#define AXI_RX_BASE  		0x1A404000
#define AXI_TXMM_BASE  		0x1A405000
#define AXI_RXMM_BASE  		0x1A406000

/* Driver Macros */
#define WR_REG(base, off, val) (*(volatile uint32_t*)(base + (off)) = (val))
#define RD_REG(base, off)      (*(volatile uint32_t*)(base + (off)))

int main()
{
  printf("Hello WiFi AXI Lite Dummy Test!\r\n");

  WR_REG(AXI_XPU_BASE, 0x04, 0x88);
  //WR_REG(AXI_TXINTF_BASE, 0x04, 0x88);
  //WR_REG(AXI_RXINTF_BASE, 0x04, 0x88);
  //WR_REG(AXI_TX_BASE, 0x04, 0x88);
  //WR_REG(AXI_RX_BASE, 0x04, 0x88);
  //WR_REG(AXI_TXMM_BASE, 0x04, 0x88);
  //WR_REG(AXI_RXMM_BASE, 0x08, 0x88);
  
  printf("Done!\r\n");
  
  //pos_delay_busy_us(10);
  
  return 0;
}

void pe_start(void)
{
}

