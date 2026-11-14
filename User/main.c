/**
 * ============================================================================
 *  main.c — GD32F407VET6 学习工程入口
 * ----------------------------------------------------------------------------
 *  芯片级配置（相当于 CubeMX 帮你生成的部分，这里已配置好，不用动）:
 *   1. 时钟: HXTAL 25MHz -> PLL -> SYSCLK 168MHz (APB1=42M, APB2=84M)
 *   2. SysTick: 1ms 中断 -> delay_1ms() 可用
 *   3. 调试串口: USART1(PD5/PD6, 板上 RS485 口) 115200-8-N-1
 *      -> log_printf() 可直接打印调试信息（串口终端 115200 接板子 485 端子）
 *
 *  你的代码写在 main() 里 "TODO" 标记处;
 *  要加新外设, 照抄 uart_init() 的三段式: 开时钟 -> 配 GPIO -> 配外设。
 * ============================================================================
 */
#include "gd32f4xx.h"
#include "systick.h"
#include <stdio.h>
#include <stdarg.h>

/* ---------------- 调试串口: 引脚/外设定义 ---------------- */
#define LOG_USART        USART1
#define LOG_USART_CLK    RCU_USART1
#define LOG_GPIO_CLK     RCU_GPIOD
#define LOG_GPIO_PORT    GPIOD
#define LOG_TX_PIN       GPIO_PIN_5
#define LOG_RX_PIN       GPIO_PIN_6
#define LOG_AF           GPIO_AF_7

/* ---------------- 时钟: 25M HXTAL -> PLL -> 168MHz ---------------- */
static void clock_config(void)
{
    rcu_osci_on(RCU_HXTAL);
    if (rcu_osci_stab_wait(RCU_HXTAL) != SUCCESS) {
        while (1) {
        }
    }

    /* 25M /25 = 1M, x336 = 336M, /2 = 168M(SYSCLK), /7 = 48M(USB 备用) */
    rcu_pll_config(RCU_PLLSRC_HXTAL, 25u, 336u, 2u, 7u);
    rcu_osci_on(RCU_PLL_CK);
    if (rcu_osci_stab_wait(RCU_PLL_CK) != SUCCESS) {
        while (1) {
        }
    }

    rcu_ahb_clock_config(RCU_AHB_CKSYS_DIV1);
    rcu_apb1_clock_config(RCU_APB1_CKAHB_DIV4);
    rcu_apb2_clock_config(RCU_APB2_CKAHB_DIV2);
    rcu_system_clock_source_config(RCU_CKSYSSRC_PLLP);

    /* GD32 无需配置 Flash 等待周期(硬件加速缓存自动处理)——与 STM32 不同 */
}

/* ---------------- 调试串口初始化 ---------------- */
static void uart_init(void)
{
    rcu_periph_clock_enable(LOG_GPIO_CLK);
    rcu_periph_clock_enable(LOG_USART_CLK);

    gpio_mode_set(LOG_GPIO_PORT, GPIO_MODE_AF, GPIO_PUPD_PULLUP, LOG_TX_PIN);
    gpio_mode_set(LOG_GPIO_PORT, GPIO_MODE_AF, GPIO_PUPD_PULLUP, LOG_RX_PIN);
    gpio_output_options_set(LOG_GPIO_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, LOG_TX_PIN);
    gpio_af_set(LOG_GPIO_PORT, LOG_AF, LOG_TX_PIN);
    gpio_af_set(LOG_GPIO_PORT, LOG_AF, LOG_RX_PIN);

    usart_deinit(LOG_USART);
    usart_baudrate_set(LOG_USART, 115200u);
    usart_word_length_set(LOG_USART, USART_WL_8BIT);
    usart_stop_bit_set(LOG_USART, USART_STB_1BIT);
    usart_parity_config(LOG_USART, USART_PM_NONE);
    usart_transmit_config(LOG_USART, USART_TRANSMIT_ENABLE);
    usart_receive_config(LOG_USART, USART_RECEIVE_ENABLE);
    usart_enable(LOG_USART);
}

/* ---------------- 调试打印: log_printf("x=%d", v) ---------------- */
void log_printf(const char *fmt, ...)
{
    char buf[128];
    va_list ap;
    int n;

    va_start(ap, fmt);
    n = vsnprintf(buf, sizeof(buf), fmt, ap);
    va_end(ap);
    if (n <= 0) {
        return;
    }

    for (int i = 0; i < n; i++) {
        while (usart_flag_get(LOG_USART, USART_FLAG_TBE) == RESET) {
            /* 等发送寄存器空 */
        }
        usart_data_transmit(LOG_USART, (uint8_t)buf[i]);
    }
    while (usart_flag_get(LOG_USART, USART_FLAG_TC) == RESET) {
        /* 等移位完成 */
    }
}

int main(void)
{
    /* ---- 芯片级配置（已就绪） ---- */
    clock_config();
    systick_config();
    uart_init();

    log_printf("\r\n=====================================\r\n");
    log_printf(" GD32F407VET6 学习工程 @168MHz\r\n");
    log_printf(" 运行中, 每秒打印一次心跳\r\n");
    log_printf("=====================================\r\n");

    /* ---- 你的代码从这里开始 ---- */
    uint32_t count = 0u;

    while (1) {
        count++;
        log_printf("heartbeat %u\r\n", count);
        delay_1ms(1000u);

        /* TODO: 在这里写你的应用代码
         * 例如点亮一个 GPIO:
         *   rcu_periph_clock_enable(RCU_GPIOA);
         *   gpio_mode_set(GPIOA, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO_PIN_0);
         *   gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_2MHZ, GPIO_PIN_0);
         *   gpio_bit_toggle(GPIOA, GPIO_PIN_0);
         */
    }
}
