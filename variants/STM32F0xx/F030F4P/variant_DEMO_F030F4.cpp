/*
 *******************************************************************************
 * Copyright (c) 2021, STMicroelectronics
 * All rights reserved.
 *
 * This software component is licensed by ST under BSD 3-Clause license,
 * the "License"; You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                        opensource.org/licenses/BSD-3-Clause
 *
 *******************************************************************************
 */
#if defined(ARDUINO_DEMO_F030F4) || defined(ARDUINO_DEMO_F030F4_16M) || defined(ARDUINO_DEMO_F030F4_HSI)
#include "pins_arduino.h"

#ifdef __cplusplus
extern "C" {
#endif

// Pin number
// This array allows to wrap Arduino pin number(Dx or x)
// to STM32 PinName (PX_n)
const PinName digitalPin[] = {
  // USB connector on the top, MCU side
  // Left Side
  PA_0,  //D0
  PA_1,  //D1
  PA_2,  //D2  - TX
  PA_3,  //D3  - RX
  PA_4,  //D4  - LED
  // Right side
  PA_5,  //D5  - SCK
  PA_6,  //D6  - MISO
  PA_7,  //D7  - MOSI
  PB_1,  //D8  - SS
  PA_9,  //D9  - SCL (TX UART Header)
  PA_10, //D10 - SDA (RX UART Header)
  PA_13, //D11 - SWDIO
  PA_14, //D12 - SWCLK
  // These two are only available on boards without a crystal:
  PF_0,
  PF_1,
};

// Analog (Ax) pin number array
const uint32_t analogInputPin[] = {
  0, //A0
  1, //A1
  2, //A2
  3, //A3
  4, //A4
  5, //A5
  6, //A6
  7, //A7
  8  //A8
};

#ifdef __cplusplus
}
#endif

// ----------------------------------------------------------------------------

#ifdef __cplusplus
extern "C" {
#endif

/**
  * @brief  System Clock Configuration
  * @param  None
  * @retval None
  */
WEAK void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {};

  /*
   * Initializes the RCC Oscillators according to the specified parameters
   * in the RCC_OscInitTypeDef structure.
   */
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
#ifdef ARDUINO_DEMO_F030F4_HSI
  /* Internal HSI, 48MHz system clock */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL12;
  RCC_OscInitStruct.PLL.PREDIV = RCC_PREDIV_DIV1;
#else
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL6;
#ifdef ARDUINO_DEMO_F030F4_16M
  RCC_OscInitStruct.PLL.PREDIV = RCC_PREDIV_DIV2;
#else
  RCC_OscInitStruct.PLL.PREDIV = RCC_PREDIV_DIV1;
#endif
#endif
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
    Error_Handler();
  }

  /* Initializes the CPU, AHB and APB busses clocks */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
                                | RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK) {
    Error_Handler();
  }
}

#ifdef __cplusplus
}
#endif
#endif /* ARDUINO_* */
