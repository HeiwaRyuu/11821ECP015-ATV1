#include<stdlib.h>
#include<stdint.h>

/* Endereço base do AHB1 */
#define STM32_RCC_BASE      0x40023800U /* Os endereços de 0x40023800 até 0x40023BFF são de Reset and Clock Control (RCC) */

/* Endereço base do AHB2 */
#define STM32_GPIOC_BASE        0x48000800U /* Os endereços de 0x48000800 até 0x48000bff são dos GPIOs porta C */


/* Offset de registros */
#define STM32_RCC_AHB1ENR_OFFSET       0x0030 /* Registro de ativação do clock periférico AHB1 */


#define STM32_GPIO_MODER_OFFSET   0x0000 /* Registrador de modo de porta do GPIO */
#define STM32_GPIO_OTYPER_OFFSET  0x0004 /* Registrador do tipo de saída da porta do GPIO */
#define STM32_GPIO_PUPDR_OFFSET   0x000c /* Registrador pull-up/pull-down da porta do GPIO */

#define STM32_GPIO_BSRR_OFFSET    0x0018/* Registrador de set/reset do bit da porta do GPIO */

/* Endereço de registro */
#define STM32_RCC_AHB1ENR   (STM32_RCC_BASE + STM32_RCC_AHB1ENR_OFFSET) /* Como visto, o endereço do registro é o endereço base mais o offset */

#define STM32_GPIOC_MODER       (STM32_GPIOC_BASE + STM32_GPIO_MODER_OFFSET)
#define STM32_GPIOC_OTYPER      (STM32_GPIOC_BASE + STM32_GPIO_OTYPER_OFFSET)
#define STM32_GPIOC_PUPDR       (STM32_GPIOC_BASE + STM32_GPIO_PUPDR_OFFSET)
#define STM32_GPIOC_BSRR        (STM32_GPIOC_BASE + STM32_GPIO_BSRR_OFFSET)

/* Registro de ativação do clock periférico AHB1 */
#define RCC_AHB1ENR_GPIOCEN     (1 << 2) /* Bit 2: ativador de clock IO porta C */


/* Registrador do modo de porta do GPIO */

#define GPIO_MODER_INPUT        (0)     /* Input */
#define GPIO_MODER_OUTPUT       (1)    /* General purpose output mode */
#define GPIO_MODER_ALT          (2)       /* Alternate mode */
#define GPIO_MODER_ANALOG       (3)    /* Analog mode */

#define GPIO_MODER13_SHIFT      (26)
#define GPIO_MODER13_MASK       (3 << GPIO_MODER13_SHIFT)

/* GPIO port output type register */
#define GPIO_OTYPER_PP          (0)   /* 0=Output push-pull */
#define GPIO_OTYPER_OD          (1)   /* 1=Output open-drain */

#define GPIO_OT13_SHIFT         (13)
#define GPIO_OT13_MASK          (1 << GPIO_OT13_SHIFT)

/* GPIO port pull-up/pull-down register */
#define GPIO_PUPDR_NONE         (0)      /* No pull-up, pull-down */
#define GPIO_PUPDR_PULLUP       (1)    /* Pull-up */
#define GPIO_PUPDR_PULLDOWN     (2)  /* Pull-down */

#define GPIO_PUPDR13_SHIFT      (26)
#define GPIO_PUPDR13_MASK       (3 << GPIO_PUPDR13_SHIFT)


/* GPIO port bit set/reset register */
#define GPIO_BSRR_SET(n)(1 << (n))
#define GPIO_BSRR_RST(n)(1 << (n + 16))

/* Definindo uma variável global que registra o estado do nosso LED */
static uint32_t led_status;
/* Definindo um array que salva informações sobre a versão do programa */
static const char fw_version[] = {'V','1','.','0'};

/* Definindo o delay do LED */
#define LED_DELAY 2000
/* Definindo GPIO_BSRR13_SET e GPIO_BSRR13_RESET*/
#define GPIO_BSRR13_SET       GPIO_BSRR_SET(13)
#define GPIO_BSRR13_RESET     GPIO_BSRR_RST(13)

int main(int argc, char *argv[])
{   
    uint32_t reg;

    /* Ponteiros para registradores */
    uint32_t *pRCC_AHB1ENR = (uint32_t *)STM32_RCC_AHB1ENR;
    //uint32_t *pGPIOC_MODER  = (uint32_t *)STM32_GPIOC_MODER;
    //uint32_t *pGPIOC_OTYPER = (uint32_t *)STM32_GPIOC_OTYPER;
    //uint32_t *pGPIOC_PUPDR  = (uint32_t *)STM32_GPIOC_PUPDR;
    uint32_t *pGPIOC_BSRR = (uint32_t *)STM32_GPIOC_BSRR;

    /* Habilitando o clock do GPIOC */
    reg = *pRCC_AHB1ENR;
    reg |= RCC_AHB1ENR_GPIOCEN;
    *pRCC_AHB1ENR = reg;


    while(1)
    {
        *pGPIOC_BSRR = GPIO_BSRR13_SET;
        led_status = 0;
        for(uint32_t i =0; i < LED_DELAY; i++);
        *pGPIOC_BSRR = GPIO_BSRR13_RESET;
        led_status = 1;
        for(uint32_t i =0; i < LED_DELAY; i++);
    };
    
    /* Não deveria chegar até aqui */

    return EXIT_SUCCESS;
}