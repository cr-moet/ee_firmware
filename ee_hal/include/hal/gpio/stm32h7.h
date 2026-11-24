#ifndef _HAL_GPIO_STM32H7_H
#define _HAL_GPIO_STM32H7_H

/* STM32_GPIO('B', 0) -> globale GPIO-Nummer von PB0 */
#define STM32_GPIO(port, pin)	((((port) - 'A') * 16) + (pin))

int stm32h7_gpio_init(void);

#endif /* _HAL_GPIO_STM32H7_H */