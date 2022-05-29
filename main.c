#include<stdlib.h>
#include<stdint.h>

/* Endereço base do AHB1 */
#define STM32_RCC_BASE_ADRESS      0x40023800U /* Os endereços de 0x40023800 até 0x40023BFF são de Reset and Clock Control (RCC) */

/* Endereço base do AHB2 */
#define STM32_GPIOC_BASE_ADRESS        0x48000800U /* Os endereços de 0x48000800 até 0x48000bff são dos GPIOs porta C */


/* Offset de registros */
#define STM32_RCC_AHB1ENABLE_REGISTER_OFFSET      0x0030 /* Registro de ativação do clock periférico AHB1 */


#define STM32_GPIO_MODE_REGISTER_OFFSET   0x0000 /* Registrador de modo de porta do GPIO */
#define STM32_GPIO_OTYPE_REGISTER_OFFSET  0x0004 /* Registrador do tipo de saída da porta do GPIO */
#define STM32_GPIO_PUPD_REGISTER_OFFSET   0x000c /* Registrador pull-up/pull-down da porta do GPIO */

#define STM32_GPIO_BSR_REGISTER_OFFSET    0x0018/* Registrador de set/reset do bit da porta do GPIO */

/* Endereço de registro */
#define STM32_RCC_AHB1_ENABLE_REGISTER   (STM32_RCC_BASE_ADRESS + STM32_RCC_AHB1ENABLE_REGISTER_OFFSET) /* Como visto, o endereço do registro é o endereço base mais o offset */

#define STM32_GPIOC_MODE_REGISTER       (STM32_GPIOC_BASE_ADRESS + STM32_GPIO_MODE_REGISTER_OFFSET)
#define STM32_GPIOC_OTYPE_REGISTER      (STM32_GPIOC_BASE_ADRESS + STM32_GPIO_OTYPE_REGISTER_OFFSET)
#define STM32_GPIOC_PUPD_REGISTER       (STM32_GPIOC_BASE_ADRESS + STM32_GPIO_PUPD_REGISTER_OFFSET)
#define STM32_GPIOC_BSR_RREGISTER       (STM32_GPIOC_BASE_ADRESS + STM32_GPIO_BSR_REGISTER_OFFSET)

/* Registro de ativação do clock periférico AHB1 */
#define RCC_AHB1ENR_GPIOC_ENABLE     (1 << 2) /* Bit 2: ativador de clock IO porta C */


/* Registrador do modo de porta do GPIO */

#define GPIO_MODE_REGISTER_INPUT        (0)    /* Entrada */
#define GPIO_MODE_REGISTER_OUTPUT       (1)    /* Modo de saída de propósito genérico */
#define GPIO_MODE_REGISTER_ALT          (2)    /* Modo Alternado */
#define GPIO_MODE_REGISTER_ANALOG       (3)    /* Modo Analog */

#define GPIO_MODE_REGISTER_PIN_13_SHIFT      (26)
#define GPIO_MODE_REGISTER_PIN_13_MASK       (3 << GPIO_MODE_REGISTER_PIN_13_SHIFT)

/* Registro de tipo de saída da porta do GPIO */
#define GPIO_OTYPE_REGISTER_PUSHPULL          (0)   /* 0 = Saída push-pull */
#define GPIO_OTYPE_REGISTER_OPENDRAIN          (1)   /* 1 =  Saída open-drain */

#define GPIO_OTYPE_PIN_13_SHIFT         (13)
#define GPIO_OTYPE_PIN_13_MASK          (1 << GPIO_OTYPE_PIN_13_SHIFT)

/* Registro de  pull-up/pull-down da porta do GPIO */
#define GPIO_PUPD_REGISTER_NONE         (0)    /* Sem pull-up e sem pull-down */
#define GPIO_PUPD_REGISTER_PULLUP       (1)   /* Pull-up */
#define GPIO_PUPD_REGISTER_PULLDOWN     (2)  /* Pull-down */

/* Gerando o shift no pino 13 */
#define GPIO_PUPD_REGISTER_PIN_13_SHIFT      (26)
#define GPIO_PUPD_REGISTER_PIN_13_MASK       (3 << GPIO_PUPD_REGISTER_PIN_13_SHIFT)


/* Registro de bit set/reset da porta do GPIO */
#define GPIO_BSR_REGISTER_SET(n)(1 << (n))
#define GPIO_BSR_REGISTER_RESET(n)(1 << (n + 16))

/* Definindo uma variável global que registra o estado do nosso LED */
static uint32_t led_status;
/* Definindo um array que salva informações sobre a versão do programa */
static const char fw_version[] = {'V','1','.','0'};

/* Definindo o delay do LED */
#define LED_DELAY 20000

/* Definindo GPIO_BSRR13_SET e GPIO_BSRR13_RESET*/
#define GPIO_BSR_REGISTER_PIN_13_SET       GPIO_BSR_REGISTER_SET(13)
#define GPIO_BSR_REGISTER_PIN_13_RESET     GPIO_BSR_REGISTER_RESET(13)

int main(int argc, char *argv[])
{   
    uint32_t reg;

    /* Ponteiros para registradores */
    uint32_t *pRCC_AHB1_ENABLE_REGISTER = (uint32_t *)STM32_RCC_AHB1_ENABLE_REGISTER;
    uint32_t *pGPIOC_MODE_REGISTER  = (uint32_t *)STM32_GPIOC_MODE_REGISTER;
    uint32_t *pGPIOC_OTYPE_REGISTER = (uint32_t *)STM32_GPIOC_OTYPE_REGISTER;
    uint32_t *pGPIOC_PUPD_REGISTER  = (uint32_t *)STM32_GPIOC_PUPD_REGISTER;
    uint32_t *pGPIOC_BSR_REGISTER = (uint32_t *)STM32_GPIOC_BSR_RREGISTER;

    /* Habilitando o clock do GPIOC */
    reg = *pRCC_AHB1_ENABLE_REGISTER;
    reg |= RCC_AHB1ENR_GPIOC_ENABLE;
    *pRCC_AHB1_ENABLE_REGISTER = reg;


    /* Loop para piscar o led */
    while(1)
    {
        *pGPIOC_BSR_REGISTER = GPIO_BSR_REGISTER_PIN_13_SET;
        led_status = 0;
        for(uint32_t i =0; i < LED_DELAY; i++);
        *pGPIOC_BSR_REGISTER = GPIO_BSR_REGISTER_PIN_13_RESET;
        led_status = 1;
        for(uint32_t i =0; i < LED_DELAY; i++);
    };
    
    /* Não deveria chegar até aqui */

    return EXIT_SUCCESS;
}