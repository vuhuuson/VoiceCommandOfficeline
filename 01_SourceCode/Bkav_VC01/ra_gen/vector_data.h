/* generated vector header file - do not edit */
#ifndef VECTOR_DATA_H
#define VECTOR_DATA_H
/* Number of interrupts allocated */
#ifndef VECTOR_DATA_IRQ_COUNT
#define VECTOR_DATA_IRQ_COUNT    (12)
#endif
/* ISR prototypes */
void sci_uart_rxi_isr(void);
void sci_uart_txi_isr(void);
void sci_uart_tei_isr(void);
void sci_uart_eri_isr(void);
void fcu_frdyi_isr(void);
void fcu_fiferr_isr(void);
void spi_rxi_isr(void);
void spi_txi_isr(void);
void spi_tei_isr(void);
void spi_eri_isr(void);
void gpt_counter_overflow_isr(void);
void r_icu_isr(void);

/* Vector table allocations */
#define VECTOR_NUMBER_SCI3_RXI ((IRQn_Type) 0) /* SCI3 RXI (Received data full) */
#define VECTOR_NUMBER_SCI3_TXI ((IRQn_Type) 1) /* SCI3 TXI (Transmit data empty) */
#define VECTOR_NUMBER_SCI3_TEI ((IRQn_Type) 2) /* SCI3 TEI (Transmit end) */
#define VECTOR_NUMBER_SCI3_ERI ((IRQn_Type) 3) /* SCI3 ERI (Receive error) */
#define VECTOR_NUMBER_FCU_FRDYI ((IRQn_Type) 4) /* FCU FRDYI (Flash ready interrupt) */
#define VECTOR_NUMBER_FCU_FIFERR ((IRQn_Type) 5) /* FCU FIFERR (Flash access error interrupt) */
#define VECTOR_NUMBER_SPI1_RXI ((IRQn_Type) 6) /* SPI1 RXI (Receive buffer full) */
#define VECTOR_NUMBER_SPI1_TXI ((IRQn_Type) 7) /* SPI1 TXI (Transmit buffer empty) */
#define VECTOR_NUMBER_SPI1_TEI ((IRQn_Type) 8) /* SPI1 TEI (Transmission complete event) */
#define VECTOR_NUMBER_SPI1_ERI ((IRQn_Type) 9) /* SPI1 ERI (Error) */
#define VECTOR_NUMBER_GPT0_COUNTER_OVERFLOW ((IRQn_Type) 10) /* GPT0 COUNTER OVERFLOW (Overflow) */
#define VECTOR_NUMBER_ICU_IRQ4 ((IRQn_Type) 11) /* ICU IRQ4 (External pin interrupt 4) */
typedef enum IRQn
{
    Reset_IRQn = -15,
    NonMaskableInt_IRQn = -14,
    HardFault_IRQn = -13,
    MemoryManagement_IRQn = -12,
    BusFault_IRQn = -11,
    UsageFault_IRQn = -10,
    SecureFault_IRQn = -9,
    SVCall_IRQn = -5,
    DebugMonitor_IRQn = -4,
    PendSV_IRQn = -2,
    SysTick_IRQn = -1,
    SCI3_RXI_IRQn = 0, /* SCI3 RXI (Received data full) */
    SCI3_TXI_IRQn = 1, /* SCI3 TXI (Transmit data empty) */
    SCI3_TEI_IRQn = 2, /* SCI3 TEI (Transmit end) */
    SCI3_ERI_IRQn = 3, /* SCI3 ERI (Receive error) */
    FCU_FRDYI_IRQn = 4, /* FCU FRDYI (Flash ready interrupt) */
    FCU_FIFERR_IRQn = 5, /* FCU FIFERR (Flash access error interrupt) */
    SPI1_RXI_IRQn = 6, /* SPI1 RXI (Receive buffer full) */
    SPI1_TXI_IRQn = 7, /* SPI1 TXI (Transmit buffer empty) */
    SPI1_TEI_IRQn = 8, /* SPI1 TEI (Transmission complete event) */
    SPI1_ERI_IRQn = 9, /* SPI1 ERI (Error) */
    GPT0_COUNTER_OVERFLOW_IRQn = 10, /* GPT0 COUNTER OVERFLOW (Overflow) */
    ICU_IRQ4_IRQn = 11, /* ICU IRQ4 (External pin interrupt 4) */
} IRQn_Type;
#endif /* VECTOR_DATA_H */
