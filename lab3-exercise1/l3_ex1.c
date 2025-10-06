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
#define TIMER_FREQUENCY (32000000UL)
#define CLICK_DELAY (20U)

static void delay(uint32_t ms){
    uint32_t target_ticks = (TIMER_FREQUENCY/1000UL)*ms;
    uint32_t begin = DL_Timer_getTimerCount(TIMG12);
    while((uint32_t)(DL_Timer_getTimerCount(TIMG12)-begin) < target_ticks){__NOP();} 
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
    //above code duplicated from Lab 2 Exercise 3
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
    uint8_t previous_state = ((*DIN2 & S2_BIT) ? 1u:0u);
    uint32_t start = 0;

    while (1) {
        uint8_t current_state = ((*DIN2 & S2_BIT) ? 1u:0u);
        if(current_state != previous_state){
            delay(CLICK_DELAY);
            uint8_t confirm_state = ((*DIN2 & S2_BIT) ? 1u:0u);
            if(confirm_state == current_state){
                if(previous_state == 1u && current_state == 0u){
                    start = DL_Timer_getTimerCount(TIMG12);
                    *DOUT2 |= BLUE_BIT;
                }
                else if(previous_state == 0u && current_state == 1u){
                    uint32_t end = DL_Timer_getTimerCount(TIMG12);
                    uint32_t delta = (uint32_t)(end-start);
                    double time = ((double)delta*1000)/(double)(TIMER_FREQUENCY);
                    printf("S2 pressed for %.3f ms\n", time);
                    *DOUT2 &= (uint8_t)~BLUE_BIT;
                }
                previous_state = current_state;
            }
        }
    }
}