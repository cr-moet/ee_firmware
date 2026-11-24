#ifndef _HAL_GPIO_STM32H7_H
#define _HAL_GPIO_STM32H7_H

/* STM32_GPIO('B', 0) gives the global GPIO number of pin PB0. */
#define STM32_GPIO(port, pin)	((((port) - 'A') * 16) + (pin))

#endif /* _HAL_GPIO_STM32H7_H */