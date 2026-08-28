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
#define RX_LEN_STREAM       112
#define TX_LEN_MEM          128
#define RX_LEN_STREAM_P2    RX_LEN_STREAM+2

int f;
// MCS1 112, 128
uint8_t data_user[TX_LEN_MEM] = {15, 14, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 170, 100, 0, 132, 236, 3, 0, 0, 4, 32, 0, 46, 0, 96, 8, 205, 55, 166, 0, 32, 214, 1, 60, 241, 0, 96, 8, 173, 59, 175, 0, 0, 74, 111, 121, 44, 32, 98, 114, 105, 103, 104, 116, 32, 115, 112, 97, 114, 107, 32, 111, 102, 32, 100, 105, 118, 105, 110, 105, 116, 121, 44, 10, 68, 97, 117, 103, 104, 116, 101, 114, 32, 111, 102, 32, 69, 108, 121, 115, 105, 117, 109, 44, 10, 70, 105, 114, 101, 45, 105, 110, 115, 105, 114, 101, 100, 32, 119, 101, 32, 116, 114, 101, 97, 0, 0, 0, 0, 0, 0, 0, 0};
uint8_t data_rx[TX_LEN_MEM];

int main()
{
    printf("=== ITB WiFi P2P debug tapeout ===\r\n");

    printf("Setting register variables\r\n");
    volatile uint32_t* txmm_p = 0x1A400000;
    volatile uint32_t* rxmm_p = 0x1A401000;
    volatile uint32_t* txintf_p = 0x1A402000;
    volatile uint32_t* rxintf_p = 0x1A403000;
    volatile uint32_t* tx_p = 0x1A404000;
    volatile uint32_t* rx_p = 0x1A405000;

#ifdef RF
    printf("Writing to register for loopback RF\r\n");
    // Use RF
    *(rxintf_p+3) = 0; // Use RF
    *(rxintf_p+11) = 0x4; // With shift
#else
    printf("Writing to register for loopback chip digital\r\n");
    // Loopback FPGA
    *(rxintf_p+3) = 0x100; // Loopback
    *(rxintf_p+11) = 0; // No shift
#endif

    //while (1) {
    printf("Inserting data to TX buffer\r\n");
    // Clear RX
    for (int i = 0; i <= RX_LEN_STREAM_P2; i++)
        data_rx[i] = 0;
    // Print TX
    printf("--- TX:\r\n");
    for (int i = 0; i <= TX_LEN_MEM-1-4; i++)
        if ((i < 3) || (i > 15))
            printf("%d,", data_user[i]);
    printf("\r\n");
    // Write TX
    for (int i = 0; i <= TX_LEN_MEM-1; i++)
    {
        *(txmm_p+0) = i;
        *(txmm_p+1) = data_user[i];
        *(txmm_p+2) = 1;
    }
    *(txmm_p+3) = 1;
	
    // Wait RX
    //pos_delay_busy_us(1000);
    while (*(rxmm_p+2) == 0);

    // Read RX
    printf("--- RX:\r\n");
    for (int i = 0; i <= RX_LEN_STREAM_P2-4; i++) {
        *(rxmm_p+0) = i;
        data_rx[i] = *(rxmm_p+1);
        printf("%d,", data_rx[i]);
    }
    printf("\r\n");

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
    printf("bit_error=%d\r\n", bit_err);
    
    printf("=== Testing end ===\r\n\r\n");
    pos_delay_busy_us(200*1000);
    //}

    return 0;
}

void pe_start(void)
{
}
