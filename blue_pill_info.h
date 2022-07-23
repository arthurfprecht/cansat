#define MCU_STM32F103C8
#define F_CPU 72000000L

#define clockCyclesPerMicrosecond() ( F_CPU / 1000000L )
#define clockCyclesToMicroseconds(a) ( ((a) * 1000L) / (F_CPU / 1000L) )
#define microsecondsToClockCycles(a) ( (a) * (F_CPU / 1000000L) )

#define BOARD_NR_SPI 2

#define	BOARD_SPI1_NSS_PIN	PA4
#define BOARD_SPI1_SCK_PIN	PA5
#define BOARD_SPI1_MISO_PIN	PA6
#define BOARD_SPI1_MOSI_PIN	PA7
	 
#define	BOARD_SPI2_NSS_PIN	PB12
#define BOARD_SPI2_SCK_PIN	PB13
#define BOARD_SPI2_MISO_PIN	PB14
#define BOARD_SPI2_MOSI_PIN	PB15