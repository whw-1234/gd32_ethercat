/**
 * ============================================================================
 *  gd32f4xx_it.c — 中断服务程序
 * ----------------------------------------------------------------------------
 *  要点: SysTick_Handler 每 1ms 进一次, 调 delay_decrement() 维持 delay_1ms;
 *        以后加新中断(如 EXTI), 处理函数写在这个文件里。
 * ============================================================================
 */
#include "gd32f4xx_it.h"
#include "systick.h"

void NMI_Handler(void)
{
    while (1) {
    }
}

void HardFault_Handler(void)
{
    while (1) {
    }
}

void MemManage_Handler(void)
{
    while (1) {
    }
}

void BusFault_Handler(void)
{
    while (1) {
    }
}

void UsageFault_Handler(void)
{
    while (1) {
    }
}

void SVC_Handler(void)
{
}

void DebugMon_Handler(void)
{
}

void PendSV_Handler(void)
{
}

void SysTick_Handler(void)
{
    delay_decrement();
}
