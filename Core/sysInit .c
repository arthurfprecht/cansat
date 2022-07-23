#include "sysInit.h"

#ifndef BOARD_RCC_PLLMUL
#define BOARD_RCC_PLLMUL RCC_PLLMUL_9
#endif

#define VECT_TAB_ADDR 0x8000000

#define VECT_TAB_OFFSET 0


static void setup_nvic(void);
static void setup_clocks(void);
static void setup_flash(void) ;

void systemInit(void)
{
  	setup_flash();
	setup_clocks();
    setup_nvic();
}

static void setup_flash(void) {
    // Turn on as many Flash "go faster" features as
    // possible. flash_enable_features() just ignores any flags it
    // can't support.
    flash_enable_features(FLASH_PREFETCH | FLASH_ICACHE | FLASH_DCACHE);
    // Configure the wait states, assuming we're operating at "close
    // enough" to 3.3V.
    flash_set_latency(FLASH_SAFE_WAIT_STATES);
}

static stm32f1_rcc_pll_data pll_data = {BOARD_RCC_PLLMUL};
rcc_pll_cfg w_board_pll_cfg = {RCC_PLLSRC_HSE, &pll_data};

void board_reset_pll(void) 
{
            // TODO
        }

void board_setup_clock_prescalers(void) 
{
            rcc_set_prescaler(RCC_PRESCALER_AHB, RCC_AHB_SYSCLK_DIV_1);
            rcc_set_prescaler(RCC_PRESCALER_APB1, RCC_APB1_HCLK_DIV_2);
            rcc_set_prescaler(RCC_PRESCALER_APB2, RCC_APB2_HCLK_DIV_1);
			rcc_clk_disable(RCC_USB);
			#if F_CPU == 72000000
			rcc_set_prescaler(RCC_PRESCALER_USB, RCC_USB_SYSCLK_DIV_1_5);
			#elif F_CPU == 48000000
			rcc_set_prescaler(RCC_PRESCALER_USB, RCC_USB_SYSCLK_DIV_1_5);			
			#endif			
        }

static void setup_clocks(void) {
    // Turn on HSI. We'll switch to and run off of this while we're
    // setting up the main PLL.
    rcc_turn_on_clk(RCC_CLK_HSI);

    // Turn off and reset the clock subsystems we'll be using, as well
    // as the clock security subsystem (CSS). Note that resetting CFGR
    // to its default value of 0 implies a switch to HSI for SYSCLK.
    RCC_BASE->CFGR = 0x00000000;
    rcc_disable_css();
    rcc_turn_off_clk(RCC_CLK_PLL);
    rcc_turn_off_clk(RCC_CLK_HSE);
    board_reset_pll();
    // Clear clock readiness interrupt flags and turn off clock
    // readiness interrupts.
    RCC_BASE->CIR = 0x00000000;

    // Enable HSE, and wait until it's ready.
    rcc_turn_on_clk(RCC_CLK_HSE);
    while (!rcc_is_clk_ready(RCC_CLK_HSE))
        ;

    // Configure AHBx, APBx, etc. prescalers and the main PLL.
    board_setup_clock_prescalers();
    rcc_configure_pll(&w_board_pll_cfg);

    // Enable the PLL, and wait until it's ready.
    rcc_turn_on_clk(RCC_CLK_PLL);
    while(!rcc_is_clk_ready(RCC_CLK_PLL))
        ;

    // Finally, switch to the now-ready PLL as the main clock source.
    rcc_switch_sysclk(RCC_CLKSRC_PLL);
}


static void setup_nvic(void) {

nvic_init((uint32)VECT_TAB_ADDR, (uint32)VECT_TAB_OFFSET);

/* Roger Clark. We now control nvic vector table in boards.txt using the build.vect paramater
#ifdef VECT_TAB_FLASH
    nvic_init(USER_ADDR_ROM, 0);
#elif defined VECT_TAB_RAM
    nvic_init(USER_ADDR_RAM, 0);
#elif defined VECT_TAB_BASE
    nvic_init((uint32)0x08000000, 0);
#elif defined VECT_TAB_ADDR
    // A numerically supplied value
    nvic_init((uint32)VECT_TAB_ADDR, 0);
#else
    // Use the __text_start__ value from the linker script; this
    // should be the start of the vector table.
    nvic_init((uint32)&__text_start__, 0);
#endif

*/
}