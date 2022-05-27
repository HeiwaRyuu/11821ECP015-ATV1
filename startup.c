#define SRAM_START 0x200000000U                         /* Inicio da SRAM CORTEX-M */
#define SRAM_SIZE  (128U * 1024U)                       /* Tamanho SRAM STM32F411 128K */
#define SRAM_END   ((SRAM_START) + (SRAM_SIZE))         /* Final da SRAM STM32F411 */

#define STACK_START SRAM_END                            /* Início da Stack */