/*
 * Copyright (c) 2021, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "ti_msp_dl_config.h"
#include <stdio.h>
#include <stdint.h>

#define FACTORY_BASE_ADDR 0x41C40000

int main(void)
{
    SYSCFG_DL_init();
    //factory region type-A
    const char *factory_registers[32]={
        "TRACEID",
        "DEVICEID",
        "USERID",
        "BSLPIN_UART",
        "BSLPIN_I2C",
        "BSLPIN_INVOKE",
        "SRAMFLASH",
        "PLLSTARTUP0_4_8MHZ",
        "PLLSTARTUP1_4_8MHZ",
        "PLLSTARTUP0_8_16MHZ",
        "PLLSTARTUP1_8_16MHZ",
        "PLLSTARTUP0_16_32MHZ",
        "PLLSTARTUP1_16_32MHZ",
        "PLLSTARTUP0_32_48MHZ",
        "PLLSTARTUP1_32_48MHZ",
        "TEMP_SENSE0",
        "RESERVED0",
        "RESERVED1",
        "RESERVED2",
        "RESERVED3",
        "RESERVED4",
        "RESERVED5",
        "RESERVED6",
        "RESERVED7",
        "RESERVED8",
        "RESERVED9",
        "RESERVED10",
        "RESERVED11",
        "RESERVED12",
        "RESERVED13",
        "RESERVED14",
        "BOOTCRC"
    };
    volatile uint32_t *factory_region_ptr = (volatile uint32_t *)FACTORY_BASE_ADDR; //pointer to factory region as 32 bit word pointer
    int i = 0;
    while (i<32) {
        uint32_t addr = FACTORY_BASE_ADDR+(i*4);
        uint32_t value = factory_region_ptr[i];
        printf("%-18s 0x%08X 0x%08X\n",factory_registers[i], addr, value); //formats the output so that the acronym is left justified with an 18 character space, and the address and value both get displayed as 8 digit hex numbers
        i++;
    }
}