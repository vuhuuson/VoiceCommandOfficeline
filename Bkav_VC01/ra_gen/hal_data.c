/* generated HAL source file - do not edit */
#include "hal_data.h"

sci_uart_instance_ctrl_t cmd_uart_ctrl;

baud_setting_t cmd_uart_baud_setting =
{
/* Baud rate calculated with 0.160% error. */.abcse = 0,
  .abcs = 0, .bgdm = 1, .cks = 1, .brr = 194, .mddr = (uint8_t) 256, .brme = false };

/** UART extended configuration for UARTonSCI HAL driver */
const sci_uart_extended_cfg_t cmd_uart_cfg_extend =
{ .clock = SCI_UART_CLOCK_INT, .rx_edge_start = SCI_UART_START_BIT_FALLING_EDGE, .noise_cancel =
          SCI_UART_NOISE_CANCELLATION_DISABLE,
  .rx_fifo_trigger = SCI_UART_RX_FIFO_TRIGGER_MAX, .p_baud_setting = &cmd_uart_baud_setting, .uart_mode =
          UART_MODE_RS232,
  .ctsrts_en = SCI_UART_CTSRTS_RTS_OUTPUT,
#if 0
                .flow_control_pin       = BSP_IO_PORT_00_PIN_00,
#else
  .flow_control_pin = (bsp_io_port_pin_t) (0xFFFFU),
#endif
        };

/** UART interface configuration */
const uart_cfg_t cmd_uart_cfg =
{ .channel = 0, .data_bits = UART_DATA_BITS_8, .parity = UART_PARITY_OFF, .stop_bits = UART_STOP_BITS_1, .p_callback =
          user_uart_callback,
  .p_context = NULL, .p_extend = &cmd_uart_cfg_extend,
#define RA_NOT_DEFINED (1)
#if (RA_NOT_DEFINED == RA_NOT_DEFINED)
  .p_transfer_tx = NULL,
#else
                .p_transfer_tx       = &RA_NOT_DEFINED,
#endif
#if (RA_NOT_DEFINED == RA_NOT_DEFINED)
  .p_transfer_rx = NULL,
#else
                .p_transfer_rx       = &RA_NOT_DEFINED,
#endif
#undef RA_NOT_DEFINED
  .rxi_ipl = (12),
  .txi_ipl = (12), .tei_ipl = (12), .eri_ipl = (12),
#if defined(VECTOR_NUMBER_SCI0_RXI)
                .rxi_irq             = VECTOR_NUMBER_SCI0_RXI,
#else
  .rxi_irq = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_SCI0_TXI)
                .txi_irq             = VECTOR_NUMBER_SCI0_TXI,
#else
  .txi_irq = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_SCI0_TEI)
                .tei_irq             = VECTOR_NUMBER_SCI0_TEI,
#else
  .tei_irq = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_SCI0_ERI)
                .eri_irq             = VECTOR_NUMBER_SCI0_ERI,
#else
  .eri_irq = FSP_INVALID_VECTOR,
#endif
        };

/* Instance structure to use this module. */
const uart_instance_t cmd_uart =
{ .p_ctrl = &cmd_uart_ctrl, .p_cfg = &cmd_uart_cfg, .p_api = &g_uart_on_sci };
icu_instance_ctrl_t g_irq_sw1_ctrl;
const external_irq_cfg_t g_irq_sw1_cfg =
{ .channel = 4,
  .trigger = EXTERNAL_IRQ_TRIG_FALLING,
  .filter_enable = true,
  .pclk_div = EXTERNAL_IRQ_PCLK_DIV_BY_64,
  .p_callback = g_irq_sw1_cb,
  .p_context = NULL,
  .p_extend = NULL,
  .ipl = (12),
#if defined(VECTOR_NUMBER_ICU_IRQ4)
    .irq                 = VECTOR_NUMBER_ICU_IRQ4,
#else
  .irq = FSP_INVALID_VECTOR,
#endif
        };
/* Instance structure to use this module. */
const external_irq_instance_t g_irq_sw1 =
{ .p_ctrl = &g_irq_sw1_ctrl, .p_cfg = &g_irq_sw1_cfg, .p_api = &g_external_irq_on_icu };
gpt_instance_ctrl_t g_timer_playback_ctrl;
#if 0
const gpt_extended_pwm_cfg_t g_timer_playback_pwm_extend =
{
    .trough_ipl          = (BSP_IRQ_DISABLED),
#if defined(VECTOR_NUMBER_GPT0_COUNTER_UNDERFLOW)
    .trough_irq          = VECTOR_NUMBER_GPT0_COUNTER_UNDERFLOW,
#else
    .trough_irq          = FSP_INVALID_VECTOR,
#endif
    .poeg_link           = GPT_POEG_LINK_POEG0,
    .output_disable      =  GPT_OUTPUT_DISABLE_NONE,
    .adc_trigger         =  GPT_ADC_TRIGGER_NONE,
    .dead_time_count_up  = 0,
    .dead_time_count_down = 0,
    .adc_a_compare_match = 0,
    .adc_b_compare_match = 0,
    .interrupt_skip_source = GPT_INTERRUPT_SKIP_SOURCE_NONE,
    .interrupt_skip_count  = GPT_INTERRUPT_SKIP_COUNT_0,
    .interrupt_skip_adc    = GPT_INTERRUPT_SKIP_ADC_NONE,
    .gtioca_disable_setting = GPT_GTIOC_DISABLE_PROHIBITED,
    .gtiocb_disable_setting = GPT_GTIOC_DISABLE_PROHIBITED,
};
#endif
const gpt_extended_cfg_t g_timer_playback_extend =
        { .gtioca =
        { .output_enabled = false, .stop_level = GPT_PIN_LEVEL_LOW },
          .gtiocb =
          { .output_enabled = false, .stop_level = GPT_PIN_LEVEL_LOW },
          .start_source = (gpt_source_t) (GPT_SOURCE_NONE), .stop_source = (gpt_source_t) (GPT_SOURCE_NONE), .clear_source =
                  (gpt_source_t) (GPT_SOURCE_NONE),
          .count_up_source = (gpt_source_t) (GPT_SOURCE_NONE), .count_down_source = (gpt_source_t) (GPT_SOURCE_NONE), .capture_a_source =
                  (gpt_source_t) (GPT_SOURCE_NONE),
          .capture_b_source = (gpt_source_t) (GPT_SOURCE_NONE), .capture_a_ipl = (BSP_IRQ_DISABLED), .capture_b_ipl =
                  (BSP_IRQ_DISABLED),
#if defined(VECTOR_NUMBER_GPT0_CAPTURE_COMPARE_A)
    .capture_a_irq       = VECTOR_NUMBER_GPT0_CAPTURE_COMPARE_A,
#else
          .capture_a_irq = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_GPT0_CAPTURE_COMPARE_B)
    .capture_b_irq       = VECTOR_NUMBER_GPT0_CAPTURE_COMPARE_B,
#else
          .capture_b_irq = FSP_INVALID_VECTOR,
#endif
          .capture_filter_gtioca = GPT_CAPTURE_FILTER_NONE,
          .capture_filter_gtiocb = GPT_CAPTURE_FILTER_NONE,
#if 0
    .p_pwm_cfg                   = &g_timer_playback_pwm_extend,
#else
          .p_pwm_cfg = NULL,
#endif
        };
const timer_cfg_t g_timer_playback_cfg =
{ .mode = TIMER_MODE_PERIODIC,
/* Actual period: 0.0000625 seconds. Actual duty: 50%. */.period_counts = (uint32_t) 0x1d4c,
  .duty_cycle_counts = 0xea6, .source_div = (timer_source_div_t) 0, .channel = 0, .p_callback =
          g_timer_playback_callback,
  .p_context = NULL, .p_extend = &g_timer_playback_extend, .cycle_end_ipl = (12),
#if defined(VECTOR_NUMBER_GPT0_COUNTER_OVERFLOW)
    .cycle_end_irq       = VECTOR_NUMBER_GPT0_COUNTER_OVERFLOW,
#else
  .cycle_end_irq = FSP_INVALID_VECTOR,
#endif
        };
/* Instance structure to use this module. */
const timer_instance_t g_timer_playback =
{ .p_ctrl = &g_timer_playback_ctrl, .p_cfg = &g_timer_playback_cfg, .p_api = &g_timer_on_gpt };
dac_instance_ctrl_t g_dac_playback_ctrl;
const dac_extended_cfg_t g_dac_playback_ext_cfg =
{ .enable_charge_pump = 0, .data_format = DAC_DATA_FORMAT_FLUSH_RIGHT, .output_amplifier_enabled = 0, };
const dac_cfg_t g_dac_playback_cfg =
{ .channel = 0, .ad_da_synchronized = false, .p_extend = &g_dac_playback_ext_cfg };
/* Instance structure to use this module. */
const dac_instance_t g_dac_playback =
{ .p_ctrl = &g_dac_playback_ctrl, .p_cfg = &g_dac_playback_cfg, .p_api = &g_dac_on_dac };
dtc_instance_ctrl_t g_transfer1_ctrl;

transfer_info_t g_transfer1_info =
{ .dest_addr_mode = TRANSFER_ADDR_MODE_INCREMENTED,
  .repeat_area = TRANSFER_REPEAT_AREA_DESTINATION,
  .irq = TRANSFER_IRQ_END,
  .chain_mode = TRANSFER_CHAIN_MODE_DISABLED,
  .src_addr_mode = TRANSFER_ADDR_MODE_FIXED,
  .size = TRANSFER_SIZE_4_BYTE,
  .mode = TRANSFER_MODE_NORMAL,
  .p_dest = (void*) NULL,
  .p_src = (void const*) NULL,
  .num_blocks = 0,
  .length = 0, };
const dtc_extended_cfg_t g_transfer1_cfg_extend =
{ .activation_source = VECTOR_NUMBER_SPI1_RXI, };
const transfer_cfg_t g_transfer1_cfg =
{ .p_info = &g_transfer1_info, .p_extend = &g_transfer1_cfg_extend, };

/* Instance structure to use this module. */
const transfer_instance_t g_transfer1 =
{ .p_ctrl = &g_transfer1_ctrl, .p_cfg = &g_transfer1_cfg, .p_api = &g_transfer_on_dtc };
dtc_instance_ctrl_t g_transfer0_ctrl;

transfer_info_t g_transfer0_info =
{ .dest_addr_mode = TRANSFER_ADDR_MODE_FIXED,
  .repeat_area = TRANSFER_REPEAT_AREA_SOURCE,
  .irq = TRANSFER_IRQ_END,
  .chain_mode = TRANSFER_CHAIN_MODE_DISABLED,
  .src_addr_mode = TRANSFER_ADDR_MODE_INCREMENTED,
  .size = TRANSFER_SIZE_4_BYTE,
  .mode = TRANSFER_MODE_NORMAL,
  .p_dest = (void*) NULL,
  .p_src = (void const*) NULL,
  .num_blocks = 0,
  .length = 0, };
const dtc_extended_cfg_t g_transfer0_cfg_extend =
{ .activation_source = VECTOR_NUMBER_SPI1_TXI, };
const transfer_cfg_t g_transfer0_cfg =
{ .p_info = &g_transfer0_info, .p_extend = &g_transfer0_cfg_extend, };

/* Instance structure to use this module. */
const transfer_instance_t g_transfer0 =
{ .p_ctrl = &g_transfer0_ctrl, .p_cfg = &g_transfer0_cfg, .p_api = &g_transfer_on_dtc };
spi_instance_ctrl_t g_spi_i2s_ctrl;

/** SPI extended configuration for SPI HAL driver */
const spi_extended_cfg_t g_spi_i2s_ext_cfg =
{ .spi_clksyn = SPI_SSL_MODE_SPI,
  .spi_comm = SPI_COMMUNICATION_FULL_DUPLEX,
  .ssl_polarity = SPI_SSLP_LOW,
  .ssl_select = SPI_SSL_SELECT_SSL0,
  .mosi_idle = SPI_MOSI_IDLE_VALUE_FIXING_DISABLE,
  .parity = SPI_PARITY_MODE_DISABLE,
  .byte_swap = SPI_BYTE_SWAP_DISABLE,
  .spck_div =
  {
  /* Actual calculated bitrate: 1016949. */.spbr = 58,
    .brdv = 0 },
  .spck_delay = SPI_DELAY_COUNT_1,
  .ssl_negation_delay = SPI_DELAY_COUNT_1,
  .next_access_delay = SPI_DELAY_COUNT_1 };

/** SPI configuration for SPI HAL driver */
const spi_cfg_t g_spi_i2s_cfg =
{ .channel = 1,

#if defined(VECTOR_NUMBER_SPI1_RXI)
    .rxi_irq             = VECTOR_NUMBER_SPI1_RXI,
#else
  .rxi_irq = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_SPI1_TXI)
    .txi_irq             = VECTOR_NUMBER_SPI1_TXI,
#else
  .txi_irq = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_SPI1_TEI)
    .tei_irq             = VECTOR_NUMBER_SPI1_TEI,
#else
  .tei_irq = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_SPI1_ERI)
    .eri_irq             = VECTOR_NUMBER_SPI1_ERI,
#else
  .eri_irq = FSP_INVALID_VECTOR,
#endif

  .rxi_ipl = (12),
  .txi_ipl = (12), .tei_ipl = (12), .eri_ipl = (12),

  .operating_mode = SPI_MODE_SLAVE,

  .clk_phase = SPI_CLK_PHASE_EDGE_ODD,
  .clk_polarity = SPI_CLK_POLARITY_LOW,

  .mode_fault = SPI_MODE_FAULT_ERROR_DISABLE,
  .bit_order = SPI_BIT_ORDER_MSB_FIRST, .p_transfer_tx = g_spi_i2s_P_TRANSFER_TX, .p_transfer_rx =
          g_spi_i2s_P_TRANSFER_RX,
  .p_callback = g_spi_cb,

  .p_context = NULL,
  .p_extend = (void*) &g_spi_i2s_ext_cfg, };

/* Instance structure to use this module. */
const spi_instance_t g_spi_i2s =
{ .p_ctrl = &g_spi_i2s_ctrl, .p_cfg = &g_spi_i2s_cfg, .p_api = &g_spi_on_spi };
poeg_instance_ctrl_t g_poeg_ws_ctrl;
const poeg_cfg_t g_poeg_ws_cfg =
{ .trigger = (poeg_trigger_t) (POEG_TRIGGER_PIN | POEG_TRIGGER_SOFTWARE),
  .polarity = POEG_GTETRG_POLARITY_ACTIVE_HIGH,
  .noise_filter = POEG_GTETRG_NOISE_FILTER_DISABLED,
  .channel = 1,
  .ipl = (BSP_IRQ_DISABLED),
  .p_callback = NULL,
  .p_context = NULL,
#if defined(VECTOR_NUMBER_POEG1_EVENT)
    .irq       = VECTOR_NUMBER_POEG1_EVENT,
#else
  .irq = FSP_INVALID_VECTOR,
#endif
        };
/* Instance structure to use this module. */
const poeg_instance_t g_poeg_ws =
{ .p_ctrl = &g_poeg_ws_ctrl, .p_cfg = &g_poeg_ws_cfg, .p_api = &g_poeg_on_poeg };
gpt_instance_ctrl_t g_timer_SSL_ctrl;
#if 0
const gpt_extended_pwm_cfg_t g_timer_SSL_pwm_extend =
{
    .trough_ipl          = (BSP_IRQ_DISABLED),
#if defined(VECTOR_NUMBER_GPT1_COUNTER_UNDERFLOW)
    .trough_irq          = VECTOR_NUMBER_GPT1_COUNTER_UNDERFLOW,
#else
    .trough_irq          = FSP_INVALID_VECTOR,
#endif
    .poeg_link           = GPT_POEG_LINK_POEG0,
    .output_disable      =  GPT_OUTPUT_DISABLE_NONE,
    .adc_trigger         =  GPT_ADC_TRIGGER_NONE,
    .dead_time_count_up  = 0,
    .dead_time_count_down = 0,
    .adc_a_compare_match = 0,
    .adc_b_compare_match = 0,
    .interrupt_skip_source = GPT_INTERRUPT_SKIP_SOURCE_NONE,
    .interrupt_skip_count  = GPT_INTERRUPT_SKIP_COUNT_0,
    .interrupt_skip_adc    = GPT_INTERRUPT_SKIP_ADC_NONE,
    .gtioca_disable_setting = GPT_GTIOC_DISABLE_PROHIBITED,
    .gtiocb_disable_setting = GPT_GTIOC_DISABLE_PROHIBITED,
};
#endif
const gpt_extended_cfg_t g_timer_SSL_extend =
        { .gtioca =
        { .output_enabled = false, .stop_level = GPT_PIN_LEVEL_LOW },
          .gtiocb =
          { .output_enabled = true, .stop_level = GPT_PIN_LEVEL_LOW },
          .start_source = (gpt_source_t) (GPT_SOURCE_NONE), .stop_source = (gpt_source_t) (GPT_SOURCE_NONE), .clear_source =
                  (gpt_source_t) (GPT_SOURCE_NONE),
          .count_up_source = (gpt_source_t) (GPT_SOURCE_GTETRGB_FALLING | GPT_SOURCE_NONE), .count_down_source =
                  (gpt_source_t) (GPT_SOURCE_NONE),
          .capture_a_source = (gpt_source_t) (GPT_SOURCE_NONE), .capture_b_source = (gpt_source_t) (GPT_SOURCE_NONE), .capture_a_ipl =
                  (BSP_IRQ_DISABLED),
          .capture_b_ipl = (BSP_IRQ_DISABLED),
#if defined(VECTOR_NUMBER_GPT1_CAPTURE_COMPARE_A)
    .capture_a_irq       = VECTOR_NUMBER_GPT1_CAPTURE_COMPARE_A,
#else
          .capture_a_irq = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_GPT1_CAPTURE_COMPARE_B)
    .capture_b_irq       = VECTOR_NUMBER_GPT1_CAPTURE_COMPARE_B,
#else
          .capture_b_irq = FSP_INVALID_VECTOR,
#endif
          .capture_filter_gtioca = GPT_CAPTURE_FILTER_NONE,
          .capture_filter_gtiocb = GPT_CAPTURE_FILTER_NONE,
#if 0
    .p_pwm_cfg                   = &g_timer_SSL_pwm_extend,
#else
          .p_pwm_cfg = NULL,
#endif
        };
const timer_cfg_t g_timer_SSL_cfg =
{ .mode = TIMER_MODE_PWM,
  /* Actual period: 2.6666666666666667e-7 seconds. Actual duty: 50%. */.period_counts = (uint32_t) 0x20,
  .duty_cycle_counts = 0x10,
  .source_div = (timer_source_div_t) 0,
  .channel = 1,
  .p_callback = NULL,
  .p_context = NULL,
  .p_extend = &g_timer_SSL_extend,
  .cycle_end_ipl = (BSP_IRQ_DISABLED),
#if defined(VECTOR_NUMBER_GPT1_COUNTER_OVERFLOW)
    .cycle_end_irq       = VECTOR_NUMBER_GPT1_COUNTER_OVERFLOW,
#else
  .cycle_end_irq = FSP_INVALID_VECTOR,
#endif
        };
/* Instance structure to use this module. */
const timer_instance_t g_timer_SSL =
{ .p_ctrl = &g_timer_SSL_ctrl, .p_cfg = &g_timer_SSL_cfg, .p_api = &g_timer_on_gpt };
gpt_instance_ctrl_t g_timer_ws_ctrl;
#if 0
const gpt_extended_pwm_cfg_t g_timer_ws_pwm_extend =
{
    .trough_ipl          = (BSP_IRQ_DISABLED),
#if defined(VECTOR_NUMBER_GPT5_COUNTER_UNDERFLOW)
    .trough_irq          = VECTOR_NUMBER_GPT5_COUNTER_UNDERFLOW,
#else
    .trough_irq          = FSP_INVALID_VECTOR,
#endif
    .poeg_link           = GPT_POEG_LINK_POEG0,
    .output_disable      =  GPT_OUTPUT_DISABLE_NONE,
    .adc_trigger         =  GPT_ADC_TRIGGER_NONE,
    .dead_time_count_up  = 0,
    .dead_time_count_down = 0,
    .adc_a_compare_match = 0,
    .adc_b_compare_match = 0,
    .interrupt_skip_source = GPT_INTERRUPT_SKIP_SOURCE_NONE,
    .interrupt_skip_count  = GPT_INTERRUPT_SKIP_COUNT_0,
    .interrupt_skip_adc    = GPT_INTERRUPT_SKIP_ADC_NONE,
    .gtioca_disable_setting = GPT_GTIOC_DISABLE_PROHIBITED,
    .gtiocb_disable_setting = GPT_GTIOC_DISABLE_PROHIBITED,
};
#endif
const gpt_extended_cfg_t g_timer_ws_extend =
        { .gtioca =
        { .output_enabled = false, .stop_level = GPT_PIN_LEVEL_LOW },
          .gtiocb =
          { .output_enabled = true, .stop_level = GPT_PIN_LEVEL_LOW },
          .start_source = (gpt_source_t) (GPT_SOURCE_NONE), .stop_source = (gpt_source_t) (GPT_SOURCE_NONE), .clear_source =
                  (gpt_source_t) (GPT_SOURCE_NONE),
          .count_up_source = (gpt_source_t) (GPT_SOURCE_GTETRGB_FALLING | GPT_SOURCE_NONE), .count_down_source =
                  (gpt_source_t) (GPT_SOURCE_NONE),
          .capture_a_source = (gpt_source_t) (GPT_SOURCE_NONE), .capture_b_source = (gpt_source_t) (GPT_SOURCE_NONE), .capture_a_ipl =
                  (BSP_IRQ_DISABLED),
          .capture_b_ipl = (BSP_IRQ_DISABLED),
#if defined(VECTOR_NUMBER_GPT5_CAPTURE_COMPARE_A)
    .capture_a_irq       = VECTOR_NUMBER_GPT5_CAPTURE_COMPARE_A,
#else
          .capture_a_irq = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_GPT5_CAPTURE_COMPARE_B)
    .capture_b_irq       = VECTOR_NUMBER_GPT5_CAPTURE_COMPARE_B,
#else
          .capture_b_irq = FSP_INVALID_VECTOR,
#endif
          .capture_filter_gtioca = GPT_CAPTURE_FILTER_NONE,
          .capture_filter_gtiocb = GPT_CAPTURE_FILTER_NONE,
#if 0
    .p_pwm_cfg                   = &g_timer_ws_pwm_extend,
#else
          .p_pwm_cfg = NULL,
#endif
        };
const timer_cfg_t g_timer_ws_cfg =
{ .mode = TIMER_MODE_PWM,
  /* Actual period: 2.6666666666666667e-7 seconds. Actual duty: 50%. */.period_counts = (uint32_t) 0x20,
  .duty_cycle_counts = 0x10,
  .source_div = (timer_source_div_t) 0,
  .channel = 5,
  .p_callback = NULL,
  .p_context = NULL,
  .p_extend = &g_timer_ws_extend,
  .cycle_end_ipl = (BSP_IRQ_DISABLED),
#if defined(VECTOR_NUMBER_GPT5_COUNTER_OVERFLOW)
    .cycle_end_irq       = VECTOR_NUMBER_GPT5_COUNTER_OVERFLOW,
#else
  .cycle_end_irq = FSP_INVALID_VECTOR,
#endif
        };
/* Instance structure to use this module. */
const timer_instance_t g_timer_ws =
{ .p_ctrl = &g_timer_ws_ctrl, .p_cfg = &g_timer_ws_cfg, .p_api = &g_timer_on_gpt };
gpt_instance_ctrl_t g_timer_sck_ctrl;
#if 0
const gpt_extended_pwm_cfg_t g_timer_sck_pwm_extend =
{
    .trough_ipl          = (BSP_IRQ_DISABLED),
#if defined(VECTOR_NUMBER_GPT2_COUNTER_UNDERFLOW)
    .trough_irq          = VECTOR_NUMBER_GPT2_COUNTER_UNDERFLOW,
#else
    .trough_irq          = FSP_INVALID_VECTOR,
#endif
    .poeg_link           = GPT_POEG_LINK_POEG0,
    .output_disable      =  GPT_OUTPUT_DISABLE_NONE,
    .adc_trigger         =  GPT_ADC_TRIGGER_NONE,
    .dead_time_count_up  = 0,
    .dead_time_count_down = 0,
    .adc_a_compare_match = 0,
    .adc_b_compare_match = 0,
    .interrupt_skip_source = GPT_INTERRUPT_SKIP_SOURCE_NONE,
    .interrupt_skip_count  = GPT_INTERRUPT_SKIP_COUNT_0,
    .interrupt_skip_adc    = GPT_INTERRUPT_SKIP_ADC_NONE,
    .gtioca_disable_setting = GPT_GTIOC_DISABLE_PROHIBITED,
    .gtiocb_disable_setting = GPT_GTIOC_DISABLE_PROHIBITED,
};
#endif
const gpt_extended_cfg_t g_timer_sck_extend =
        { .gtioca =
        { .output_enabled = true, .stop_level = GPT_PIN_LEVEL_LOW },
          .gtiocb =
          { .output_enabled = false, .stop_level = GPT_PIN_LEVEL_LOW },
          .start_source = (gpt_source_t) (GPT_SOURCE_NONE), .stop_source = (gpt_source_t) (GPT_SOURCE_NONE), .clear_source =
                  (gpt_source_t) (GPT_SOURCE_NONE),
          .count_up_source = (gpt_source_t) (GPT_SOURCE_NONE), .count_down_source = (gpt_source_t) (GPT_SOURCE_NONE), .capture_a_source =
                  (gpt_source_t) (GPT_SOURCE_NONE),
          .capture_b_source = (gpt_source_t) (GPT_SOURCE_NONE), .capture_a_ipl = (BSP_IRQ_DISABLED), .capture_b_ipl =
                  (BSP_IRQ_DISABLED),
#if defined(VECTOR_NUMBER_GPT2_CAPTURE_COMPARE_A)
    .capture_a_irq       = VECTOR_NUMBER_GPT2_CAPTURE_COMPARE_A,
#else
          .capture_a_irq = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_GPT2_CAPTURE_COMPARE_B)
    .capture_b_irq       = VECTOR_NUMBER_GPT2_CAPTURE_COMPARE_B,
#else
          .capture_b_irq = FSP_INVALID_VECTOR,
#endif
          .capture_filter_gtioca = GPT_CAPTURE_FILTER_NONE,
          .capture_filter_gtiocb = GPT_CAPTURE_FILTER_NONE,
#if 0
    .p_pwm_cfg                   = &g_timer_sck_pwm_extend,
#else
          .p_pwm_cfg = NULL,
#endif
        };
const timer_cfg_t g_timer_sck_cfg =
{ .mode = TIMER_MODE_PERIODIC,
  /* Actual period: 9.75e-7 seconds. Actual duty: 49.572649572649574%. */.period_counts = (uint32_t) 0x75,
  .duty_cycle_counts = 0x3a,
  .source_div = (timer_source_div_t) 0,
  .channel = 2,
  .p_callback = NULL,
  .p_context = NULL,
  .p_extend = &g_timer_sck_extend,
  .cycle_end_ipl = (BSP_IRQ_DISABLED),
#if defined(VECTOR_NUMBER_GPT2_COUNTER_OVERFLOW)
    .cycle_end_irq       = VECTOR_NUMBER_GPT2_COUNTER_OVERFLOW,
#else
  .cycle_end_irq = FSP_INVALID_VECTOR,
#endif
        };
/* Instance structure to use this module. */
const timer_instance_t g_timer_sck =
{ .p_ctrl = &g_timer_sck_ctrl, .p_cfg = &g_timer_sck_cfg, .p_api = &g_timer_on_gpt };
flash_hp_instance_ctrl_t g_flash0_ctrl;
const flash_cfg_t g_flash0_cfg =
{ .data_flash_bgo = true, .p_callback = flash0_bgo_callback, .p_context = NULL,
#if defined(VECTOR_NUMBER_FCU_FRDYI)
    .irq                 = VECTOR_NUMBER_FCU_FRDYI,
#else
  .irq = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_FCU_FIFERR)
    .err_irq             = VECTOR_NUMBER_FCU_FIFERR,
#else
  .err_irq = FSP_INVALID_VECTOR,
#endif
  .err_ipl = (8),
  .ipl = (8), };
/* Instance structure to use this module. */
const flash_instance_t g_flash0 =
{ .p_ctrl = &g_flash0_ctrl, .p_cfg = &g_flash0_cfg, .p_api = &g_flash_on_flash_hp };
sci_uart_instance_ctrl_t g_uart_ds_ctrl;

baud_setting_t g_uart_ds_baud_setting =
{
/* Baud rate calculated with 1.725% error. */.abcse = 0,
  .abcs = 0, .bgdm = 1, .cks = 0, .brr = 15, .mddr = (uint8_t) 256, .brme = false };

/** UART extended configuration for UARTonSCI HAL driver */
const sci_uart_extended_cfg_t g_uart_ds_cfg_extend =
{ .clock = SCI_UART_CLOCK_INT, .rx_edge_start = SCI_UART_START_BIT_FALLING_EDGE, .noise_cancel =
          SCI_UART_NOISE_CANCELLATION_DISABLE,
  .rx_fifo_trigger = SCI_UART_RX_FIFO_TRIGGER_MAX, .p_baud_setting = &g_uart_ds_baud_setting, .uart_mode =
          UART_MODE_RS232,
  .ctsrts_en = SCI_UART_CTSRTS_RTS_OUTPUT,
#if 0
                .flow_control_pin       = BSP_IO_PORT_00_PIN_00,
#else
  .flow_control_pin = (bsp_io_port_pin_t) (0xFFFFU),
#endif
        };

/** UART interface configuration */
const uart_cfg_t g_uart_ds_cfg =
{ .channel = 3, .data_bits = UART_DATA_BITS_8, .parity = UART_PARITY_OFF, .stop_bits = UART_STOP_BITS_1, .p_callback =
          user_uart_callback,
  .p_context = NULL, .p_extend = &g_uart_ds_cfg_extend,
#define RA_NOT_DEFINED (1)
#if (RA_NOT_DEFINED == RA_NOT_DEFINED)
  .p_transfer_tx = NULL,
#else
                .p_transfer_tx       = &RA_NOT_DEFINED,
#endif
#if (RA_NOT_DEFINED == RA_NOT_DEFINED)
  .p_transfer_rx = NULL,
#else
                .p_transfer_rx       = &RA_NOT_DEFINED,
#endif
#undef RA_NOT_DEFINED
  .rxi_ipl = (12),
  .txi_ipl = (12), .tei_ipl = (12), .eri_ipl = (12),
#if defined(VECTOR_NUMBER_SCI3_RXI)
                .rxi_irq             = VECTOR_NUMBER_SCI3_RXI,
#else
  .rxi_irq = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_SCI3_TXI)
                .txi_irq             = VECTOR_NUMBER_SCI3_TXI,
#else
  .txi_irq = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_SCI3_TEI)
                .tei_irq             = VECTOR_NUMBER_SCI3_TEI,
#else
  .tei_irq = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_SCI3_ERI)
                .eri_irq             = VECTOR_NUMBER_SCI3_ERI,
#else
  .eri_irq = FSP_INVALID_VECTOR,
#endif
        };

/* Instance structure to use this module. */
const uart_instance_t g_uart_ds =
{ .p_ctrl = &g_uart_ds_ctrl, .p_cfg = &g_uart_ds_cfg, .p_api = &g_uart_on_sci };
void g_hal_init(void)
{
    g_common_init ();
}
