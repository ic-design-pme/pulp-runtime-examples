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
#include <stdint.h>
#include <unistd.h>

#define RF
#define MCS       0

#define RX_LEN_STREAM       1536
#define TX_LEN_MEM          1552
#define RX_LEN_STREAM_P2    RX_LEN_STREAM+2

#include "wifi_payload.h"

uint8_t data_rx[TX_LEN_MEM];

volatile uint32_t* txmm_p = (volatile uint32_t*)0x1A400000;
volatile uint32_t* rxmm_p = (volatile uint32_t*)0x1A401000;
volatile uint32_t* txintf_p = (volatile uint32_t*)0x1A402000;
volatile uint32_t* rxintf_p = (volatile uint32_t*)0x1A403000;
volatile uint32_t* tx_p = (volatile uint32_t*)0x1A404000;
volatile uint32_t* rx_p = (volatile uint32_t*)0x1A405000;

void reset()
{ 
  /*
  for (int i=0;i<8;i++)
    *(txintf_p+0) = 0;
  for (int i=0;i<32;i++)
    *(txintf_p+0) = 0xFFFFFFFF;
  for (int i=0;i<8;i++)
    *(txintf_p+0) = 0;

  for (int i=0;i<8;i++)
    *(rxintf_p+0) = 0;
  for (int i=0;i<32;i++)
    *(rxintf_p+0) = 0xFFFFFFFF;
  for (int i=0;i<8;i++)
    *(rxintf_p+0) = 0;

  for (int i=0;i<8;i++)
    *(tx_p+0) = 0;
  for (int i=0;i<32;i++)
    *(tx_p+0) = 0xFFFFFFFF;
  for (int i=0;i<8;i++)
    *(tx_p+0) = 0;
  */
    
  for (int i=0;i<8;i++)
    *(rx_p+0) = 0;
  for (int i=0;i<32;i++)
    *(rx_p+0) = 0xFFFFFFFF;
  for (int i=0;i<8;i++)
    *(rx_p+0) = 0;

  pos_delay_busy_us(1);
}

int main()
{
  printf("=== ITB WiFi P2P loop ===\r\n");

#ifdef RF
  // Use RF
  printf("Writing to register for loopback RF\r\n");
  *(rxintf_p+3) = 0; // Use RF
  *(rxintf_p+11) = 0x4; // With shift
#else
  // Loopback FPGA
  printf("Writing to register for loopback chip digital\r\n");
  *(rxintf_p+3) = 0x100; // Loopback
  *(rxintf_p+11) = 0; // No shift
#endif

  int n = 0;
  while (1) {
    reset();
    
    // Clear RX
    for (int i = 0; i <= RX_LEN_STREAM_P2; i++)
      data_rx[i] = 0;

    // Write TX
    for (int i = 0; i <= TX_LEN_MEM-1; i++)
    {
      *(txmm_p+0) = i;
      *(txmm_p+1) = data_user[i];
      *(txmm_p+2) = 1;
    }
    *(txmm_p+3) = 1;

    pos_delay_busy_us(10*1000);
    while (*(rxmm_p+2) == 0);

    // Read RX
    for (int i = 0; i <= RX_LEN_STREAM_P2-4; i++)
    {
      *(rxmm_p+0) = i;
      data_rx[i] = *(rxmm_p+1);
    }

    // Compare
    int bit_err = 0;
    for (int i = 0; i <= 2; i++) // Header
      for (int j = 0; j < 8; j++)
        if ( (data_rx[i] & (1 << j)) != (data_user[i] & (1 << j)) )
          bit_err++;
    for (int i = 3; i <= (RX_LEN_STREAM_P2-4); i++) // Data
      for (int j = 0; j < 8; j++)
        if ( (data_rx[i] & (1 << j)) != (data_user[i+13] & (1 << j)) )
          bit_err++;
    printf("Frame: %06d, MCS: %d, LEN: %d bytes, bit_error: %d bits\r\n", n++, mcs, RX_LEN_STREAM, bit_err);
    
    pos_delay_busy_us(200*1000);
  }

  return 0;
}


void pe_start(void)
{
}

