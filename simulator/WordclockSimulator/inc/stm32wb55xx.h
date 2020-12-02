#pragma once

#include <cstdint>

#define FLASH_BASE             (0x08000000UL)/*!< FLASH(up to 1 MB) base address */
#define SRAM_BASE              (0x20000000UL)/*!< SRAM(up to 256 KB) base address */
#define PERIPH_BASE            (0x40000000UL)/*!< Peripheral base address */

#define APB1PERIPH_BASE       PERIPH_BASE
#define APB2PERIPH_BASE       (PERIPH_BASE + 0x00010000UL)
#define AHB1PERIPH_BASE       (PERIPH_BASE + 0x00020000UL)
#define AHB2PERIPH_BASE       (PERIPH_BASE + 0x08000000UL)
#define AHB4PERIPH_BASE       (PERIPH_BASE + 0x18000000UL)
#define APB3PERIPH_BASE       (PERIPH_BASE + 0x20000000UL)
#define AHB3PERIPH_BASE       (PERIPH_BASE + 0x50000000UL)

#define IOPORT_BASE         (AHB2PERIPH_BASE + 0x00000000UL)
#define GPIOA_BASE          (IOPORT_BASE + 0x00000000UL)
#define GPIOB_BASE          (IOPORT_BASE + 0x00000400UL)
#define GPIOC_BASE          (IOPORT_BASE + 0x00000800UL)
#define GPIOD_BASE          (IOPORT_BASE + 0x00000C00UL)
#define GPIOE_BASE          (IOPORT_BASE + 0x00001000UL)
#define GPIOH_BASE          (IOPORT_BASE + 0x00001C00UL)

#define SPI1_BASE             (APB2PERIPH_BASE + 0x00003000UL)
#define SPI1                ((SPI_TypeDef *) SPI1_BASE)

#define USART1_BASE           (APB2PERIPH_BASE + 0x00003800UL)
#define USART1              ((USART_TypeDef *) USART1_BASE)

typedef struct
    {
      }GPIO_TypeDef;

typedef struct
    {
      }SPI_TypeDef;

typedef struct
    {
      }USART_TypeDef;

#define GPIOA               ((GPIO_TypeDef *) GPIOA_BASE)
#define GPIOB               ((GPIO_TypeDef *) GPIOB_BASE)
#define GPIOC               ((GPIO_TypeDef *) GPIOC_BASE)
#define GPIOD               ((GPIO_TypeDef *) GPIOD_BASE)
#define GPIOE               ((GPIO_TypeDef *) GPIOE_BASE)
#define GPIOH               ((GPIO_TypeDef *) GPIOH_BASE)
