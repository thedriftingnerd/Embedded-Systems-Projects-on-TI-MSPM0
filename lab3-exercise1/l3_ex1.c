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
#define GPIOB_BASE_ADDR ((uint32_t)GPIOB)
#define DIN2_OFFSET (0x1380+2)
#define DOUT2_OFFSET (0x1280+2)
#define S2_BIT (1u<<5)
#define BLUE_BIT (1u<<6) //still going to make the blue LED light up as a visual indicator for S2 being pressed
#define BUSCLK_DEFAULT (32000000UL) //in HZ
#define DEBOUNCE (20U)

static inline uint32_t tick_conversion(uint32_t ms){
    return (BUSCLK_DEFAULT/1000UL)*ms;
}

static void delay(uint32_t ms){
    uint32_t start = DL_Timer_getTimerCount(TIMG12);
    uint32_t target = tick_conversion(ms);
    while((uint32_t)(DL_Timer_getTimerCount(TIMG12)-start) < target){__NOP();}
}

int main(void)
{
    SYSCFG_DL_init();
    DL_GPIO_enablePower(GPIOB);
    DL_GPIO_initDigitalOutput((IOMUX_PINCM50));
    DL_GPIO_enableOutput(GPIOB, DL_GPIO_PIN_22);
    DL_GPIO_clearPins(GPIOB, DL_GPIO_PIN_22);
    DL_GPIO_initPeripheralInputFunctionFeatures(IOMUX_PINCM49, IOMUX_PINCM49_PF_GPIOB_DIO21, DL_GPIO_INVERSION_DISABLE, DL_GPIO_RESISTOR_PULL_UP, DL_GPIO_HYSTERESIS_DISABLE, DL_GPIO_WAKEUP_DISABLE);
    volatile uint8_t*const DIN2 = (volatile uint8_t*)(GPIOB_BASE_ADDR+DIN2_OFFSET);
    volatile uint8_t*const DOUT2 = (volatile uint8_t*)(GPIOB_BASE_ADDR+DOUT2_OFFSET);
    uint8_t one = ((*DIN2 & S2_BIT) ? 1u:0u);
    uint32_t status_pb = DL_GPIO_readPins(GPIOB, DL_GPIO_PIN_21);
    DL_Timer_enablePower(TIMG12);
    DL_Timer_ClockConfig config;
    config.clockSel = DL_TIMER_CLOCK_BUSCLK;
    config.divideRatio = DL_TIMER_CLOCK_DIVIDE_1;
    config.prescale = 0;
    DL_Timer_setClockConfig(TIMG12, &config);
    DL_Timer_TimerConfig timerConfig;
    timerConfig.timerMode = DL_TIMER_TIMER_MODE_PERIODIC_UP;
    timerConfig.period = -1;
    timerConfig.startTimer = DL_TIMER_START;
    timerConfig.genIntermInt = DL_TIMER_INTERM_INT_DISABLED;
    timerConfig.counterVal = 0;
    DL_Timer_initTimerMode(TIMG12, &timerConfig);
    uint32_t counter_value = DL_Timer_getTimerCount(TIMG12);

    printf("Press the button");

    uint8_t previous = ((*DIN2 & S2_BIT) ? 1u:0u);
    uint32_t start = 0;

    while (1) {
        uint8_t current = ((*DIN2 & S2_BIT) ? 1u:0u);
        if(current!=previous){
            delay(DEBOUNCE);
            uint8_t confirmation = ((*DIN2 & S2_BIT) ? 1u : 0u);
            if(confirmation == current){
                if(previous == 1u && current == 0u){
                    start = DL_Timer_getTimerCount(TIMG12);
                    *DOUT2 |= BLUE_BIT;
                }
                else if(previous == 0u && current == 1u){
                    uint32_t end = DL_Timer_getTimerCount(TIMG12);
                    uint32_t delta = (uint32_t)(end-start);
                    double time = ((double)delta*1000)/(double)(BUSCLK_DEFAULT);
                    printf("S2 pressed for %.3f ms (ticks = %lu)\n", time, (unsigned long)delta);
                    *DOUT2 &= (uint8_t)~BLUE_BIT;
                }
                previous = current;
            }
        }
    }
}