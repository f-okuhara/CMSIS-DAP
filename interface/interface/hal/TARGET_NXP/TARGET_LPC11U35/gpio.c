/* CMSIS-DAP Interface Firmware
 * Copyright (c) 2009-2013 ARM Limited
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
#include <LPC11Uxx.h>
#include <RTL.h>

#include "gpio.h"

static uint16_t isr_flags;
static OS_TID isr_notify;

#ifdef DBG_LPC812
#define SW_RESET_BUTTON    0
#else
#define SW_RESET_BUTTON    1
#endif

#ifdef SW_RESET_BUTTON
#define RESET_PORT        (1)
#define RESET_PIN         (19)
#define RESET_INT_CH      (0)
#define RESET_INT_MASK    (1 << RESET_INT_CH)
#endif

#if defined(CEREVO_TZ1_SB)
#define PIN_DAP_LED       (1<<16)
#define PIN_MSD_LED       (1<<16)
#define PIN_CDC_LED       (1<<16)
#else
#define PIN_DAP_LED       (1<<21)
#define PIN_MSD_LED       (1<<20)
#define PIN_CDC_LED       (1<<11)
#endif

void gpio_init(void) {
    // enable clock for GPIO port 0
    LPC_SYSCON->SYSAHBCLKCTRL |= (1UL << 6);

#if defined(BOARD_CEREVO_TZ1) && defined(CEREVO_TZ1_SB)
    // POWER HOLD 
    LPC_IOCON->TDO_PIO0_13 |= 0x01;     //PIO0_13 / 
    LPC_GPIO->DIR[0] |= (1 << 13);
    LPC_GPIO->CLR[0] |= (1 << 13);    
    
    // configure GPIO-LED as output
    // DAP/MSD/CDC led (green)
    LPC_GPIO->DIR[0]  |= (1<<16);
    LPC_GPIO->CLR[0]  |= (1<<16);

    // POWER led (red)
    LPC_IOCON->SWDIO_PIO0_15 = 0x00000491; //PIO0_15/PULLUP/DIGITAL/OPENDRAIN
    LPC_GPIO->DIR[0]  |= (1<<15);
    LPC_GPIO->CLR[0]  = (1<<15);
#else
    // configure GPIO-LED as output
    // DAP led (green)
    LPC_GPIO->DIR[0]  |= (PIN_DAP_LED);
    LPC_GPIO->CLR[0]  |= (PIN_DAP_LED);

    // MSD led (red)
    LPC_GPIO->DIR[0]  |= (PIN_MSD_LED);
    LPC_GPIO->CLR[0]  |= (PIN_MSD_LED);

    // Serial LED (blue)
    LPC_IOCON->TDI_PIO0_11 |= 0x01;
    LPC_GPIO->DIR[0]  |= (PIN_CDC_LED);
    LPC_GPIO->CLR[0]  |= (PIN_CDC_LED);
#endif

    // configure Button as input
#if SW_RESET_BUTTON
    LPC_GPIO->DIR[RESET_PORT]  &= ~(1 << RESET_PIN);
#endif

    /* Enable AHB clock to the FlexInt, GroupedInt domain. */
    LPC_SYSCON->SYSAHBCLKCTRL |= ((1<<19) | (1<<23) | (1<<24));
}

void gpio_set_dap_led(uint8_t state) {
    if (state) {
        LPC_GPIO->SET[0] |= (PIN_DAP_LED);
    } else {
        LPC_GPIO->CLR[0] |= (PIN_DAP_LED);
    }
}

void gpio_set_cdc_led(uint8_t state) {
    if (state) {
      LPC_GPIO->SET[0] |= (PIN_CDC_LED);
    } else {
      LPC_GPIO->CLR[0] |= (PIN_CDC_LED);
    }
}

void gpio_set_msd_led(uint8_t state) {
    if (state) {
        LPC_GPIO->SET[0] |= (PIN_MSD_LED);
    } else {
        LPC_GPIO->CLR[0] |= (PIN_MSD_LED);
    }
}

void gpio_enable_button_flag(OS_TID task, uint16_t flags) {
    /* When the "reset" button is pressed the ISR will set the */
    /* event flags "flags" for task "task" */

    /* Keep a local copy of task & flags */
    isr_notify=task;
    isr_flags=flags;

#if SW_RESET_BUTTON
    LPC_SYSCON->STARTERP0 |= RESET_INT_MASK;
    LPC_SYSCON->PINTSEL[RESET_INT_CH] = (RESET_PORT) ? (RESET_PIN + 24) : (RESET_PIN);

    if (!(LPC_GPIO_PIN_INT->ISEL & RESET_INT_MASK))
        LPC_GPIO_PIN_INT->IST = RESET_INT_MASK;

    LPC_GPIO_PIN_INT->IENF |= RESET_INT_MASK;

    NVIC_EnableIRQ(FLEX_INT0_IRQn);
#endif
}

#if defined(BOARD_CEREVO_TZ1) && defined(CEREVO_TZ1_SB)
void gpio_hold_powersw(void)
{
    LPC_GPIO->SET[0] |= (1<<13);
}

void gpio_release_powersw(void)
{
    LPC_GPIO->CLR[0] |= (1<<13);
}
#endif

void FLEX_INT0_IRQHandler() {
    isr_evt_set(isr_flags, isr_notify);
    NVIC_DisableIRQ(FLEX_INT0_IRQn);

    // ack interrupt
    LPC_GPIO_PIN_INT->IST = 0x01;
}
