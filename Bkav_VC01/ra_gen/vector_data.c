/* generated vector source file - do not edit */
#include "bsp_api.h"
/* Do not build these data structures if no interrupts are currently allocated because IAR will have build errors. */
#if VECTOR_DATA_IRQ_COUNT > 0
        BSP_DONT_REMOVE const fsp_vector_t g_vector_table[BSP_ICU_VECTOR_MAX_ENTRIES] BSP_PLACE_IN_SECTION(BSP_SECTION_APPLICATION_VECTORS) =
        {
                        [0] = sci_uart_rxi_isr, /* SCI3 RXI (Received data full) */
            [1] = sci_uart_txi_isr, /* SCI3 TXI (Transmit data empty) */
            [2] = sci_uart_tei_isr, /* SCI3 TEI (Transmit end) */
            [3] = sci_uart_eri_isr, /* SCI3 ERI (Receive error) */
            [4] = fcu_frdyi_isr, /* FCU FRDYI (Flash ready interrupt) */
            [5] = fcu_fiferr_isr, /* FCU FIFERR (Flash access error interrupt) */
            [6] = spi_rxi_isr, /* SPI1 RXI (Receive buffer full) */
            [7] = spi_txi_isr, /* SPI1 TXI (Transmit buffer empty) */
            [8] = spi_tei_isr, /* SPI1 TEI (Transmission complete event) */
            [9] = spi_eri_isr, /* SPI1 ERI (Error) */
            [10] = gpt_counter_overflow_isr, /* GPT0 COUNTER OVERFLOW (Overflow) */
            [11] = r_icu_isr, /* ICU IRQ4 (External pin interrupt 4) */
            [12] = sci_uart_rxi_isr, /* SCI0 RXI (Receive data full) */
            [13] = sci_uart_txi_isr, /* SCI0 TXI (Transmit data empty) */
            [14] = sci_uart_tei_isr, /* SCI0 TEI (Transmit end) */
            [15] = sci_uart_eri_isr, /* SCI0 ERI (Receive error) */
        };
        const bsp_interrupt_event_t g_interrupt_event_link_select[BSP_ICU_VECTOR_MAX_ENTRIES] =
        {
            [0] = BSP_PRV_IELS_ENUM(EVENT_SCI3_RXI), /* SCI3 RXI (Received data full) */
            [1] = BSP_PRV_IELS_ENUM(EVENT_SCI3_TXI), /* SCI3 TXI (Transmit data empty) */
            [2] = BSP_PRV_IELS_ENUM(EVENT_SCI3_TEI), /* SCI3 TEI (Transmit end) */
            [3] = BSP_PRV_IELS_ENUM(EVENT_SCI3_ERI), /* SCI3 ERI (Receive error) */
            [4] = BSP_PRV_IELS_ENUM(EVENT_FCU_FRDYI), /* FCU FRDYI (Flash ready interrupt) */
            [5] = BSP_PRV_IELS_ENUM(EVENT_FCU_FIFERR), /* FCU FIFERR (Flash access error interrupt) */
            [6] = BSP_PRV_IELS_ENUM(EVENT_SPI1_RXI), /* SPI1 RXI (Receive buffer full) */
            [7] = BSP_PRV_IELS_ENUM(EVENT_SPI1_TXI), /* SPI1 TXI (Transmit buffer empty) */
            [8] = BSP_PRV_IELS_ENUM(EVENT_SPI1_TEI), /* SPI1 TEI (Transmission complete event) */
            [9] = BSP_PRV_IELS_ENUM(EVENT_SPI1_ERI), /* SPI1 ERI (Error) */
            [10] = BSP_PRV_IELS_ENUM(EVENT_GPT0_COUNTER_OVERFLOW), /* GPT0 COUNTER OVERFLOW (Overflow) */
            [11] = BSP_PRV_IELS_ENUM(EVENT_ICU_IRQ4), /* ICU IRQ4 (External pin interrupt 4) */
            [12] = BSP_PRV_IELS_ENUM(EVENT_SCI0_RXI), /* SCI0 RXI (Receive data full) */
            [13] = BSP_PRV_IELS_ENUM(EVENT_SCI0_TXI), /* SCI0 TXI (Transmit data empty) */
            [14] = BSP_PRV_IELS_ENUM(EVENT_SCI0_TEI), /* SCI0 TEI (Transmit end) */
            [15] = BSP_PRV_IELS_ENUM(EVENT_SCI0_ERI), /* SCI0 ERI (Receive error) */
        };
        #endif
