/**
 * ============================================================================
 *  systick.h — SysTick 1ms 时基 + 延时
 * ============================================================================
 */
#ifndef SYSTICK_H
#define SYSTICK_H

#include <stdint.h>

void systick_config(void);      /* SysTick 配置为 1000Hz 中断 */
void delay_decrement(void);     /* SysTick 中断里调用(it.c), 递减延时计数 */
void delay_1ms(uint32_t count); /* 阻塞延时 count 毫秒 */

#endif /* SYSTICK_H */
