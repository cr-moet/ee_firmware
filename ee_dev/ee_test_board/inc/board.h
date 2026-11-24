#ifndef _BOARD_H
#define _BOARD_H

#include <hal/gpio/stm32h7.h>

/* Nucleo-H743ZI (MB1137, UM1974) */
#define LED_GREEN	STM32_GPIO('B', 0)
#define LED_BLUE	STM32_GPIO('B', 7)
#define LED_RED		STM32_GPIO('B', 14)

#define BUTTON_USER	STM32_GPIO('C', 13)	/* B1. The value is 1 when you push the button. */

#endif /* _BOARD_H */