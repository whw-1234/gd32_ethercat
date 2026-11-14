/**
 * ============================================================================
 *  systick.c — SysTick 配置与延时（官方模板同款模式）
 * ----------------------------------------------------------------------------
 *  要点: SysTick_Config 装 1ms 中断; 中断里 delay_decrement() 让计数递减;
 *        delay_1ms 阻塞等计数归零——学习工程够用, 之后可换成非阻塞。
 * ============================================================================
 */
#include "gd32f4xx.h"
#include "systick.h"

static volatile uint32_t delay;

void systick_config(void)
{
    /* SysTick 配置为 1000Hz 中断 */
    if (SysTick_Config(SystemCoreClock / 1000u)) {
        while (1) {
        }
    }
    NVIC_SetPriority(SysTick_IRQn, 0x00u);
}

void delay_decrement(void)
{
    if (delay != 0u) {
        delay--;
    }
}

void delay_1ms(uint32_t count)
{
    delay = count;
    while (delay != 0u) {
    }
}
