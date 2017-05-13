/*
 * hardware_i2c.c
 *
 *  Created on: May 9, 2017
 *      Author: robin
 */

#include "diag/Trace.h"
#include "error.h"
#include "hardware_i2c.h"
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_gpio.h"
#include "stm32f4xx_hal_i2c.h"

I2C_HandleTypeDef i2c3;

void HW_I2C_GPIO_Init()
{
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();

	GPIO_InitTypeDef GPIO_InitStruct;

	//
	// I2C3 SCL (PA8) and SDA (PC9)
	//
	GPIO_InitStruct.Pin = GPIO_PIN_9;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	GPIO_InitStruct.Alternate = GPIO_AF4_I2C3;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_8;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	GPIO_InitStruct.Alternate = GPIO_AF4_I2C3;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    //
    // Touch Interrupt
    //
    GPIO_InitStruct.Pin = GPIO_PIN_15;
    GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}

void HW_I2C_Init()
{
	HW_I2C_GPIO_Init();

	__HAL_RCC_I2C3_CLK_ENABLE();

	;
	i2c3.Init.ClockSpeed = 100000;
	i2c3.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
	i2c3.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
	i2c3.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
	i2c3.Init.NoStretchMode = I2C_NOSTRETCH_DISABLED;
	i2c3.Init.DutyCycle = I2C_DUTYCYCLE_2;
	i2c3.Init.OwnAddress1 = 0x0;
	i2c3.Init.OwnAddress2 = 0x0;
	i2c3.Mode = HAL_I2C_MODE_MASTER;
	i2c3.Instance = I2C3;

	if (HAL_OK != HAL_I2C_Init(&i2c3)) {
		Error_Handler();
	}
}

void HW_I2C_WriteReg8(uint16_t device_addr, uint16_t reg_addr, uint8_t pdata)
{
	//
	// HAL_I2C_Mem_Write(
	//	I2C_HandleTypeDef *hi2c,
	//	uint16_t DevAddress,
	//	uint16_t MemAddress,
	//	uint16_t MemAddSize,
	//	uint8_t *pData,
	//	uint16_t Size,
	//	uint32_t Timeout
	// );
	//

	HAL_I2C_Mem_Write(
			&i2c3,
			device_addr,
			reg_addr,
			I2C_MEMADD_SIZE_8BIT,
			&pdata,
			1, HAL_MAX_DELAY);
}

uint8_t HW_I2C_ReadReg8(uint16_t device_addr, uint16_t reg_addr)
{
	//
	// HAL_StatusTypeDef HAL_I2C_Mem_Read(
	//	I2C_HandleTypeDef *hi2c,
	//	uint16_t DevAddress,
	//	uint16_t MemAddress,
	//	uint16_t MemAddSize,
	//	uint8_t *pData,
	//	uint16_t Size,
	//	uint32_t Timeout
	// );
	//

	uint8_t rdata;

	HAL_I2C_Mem_Read(
			&i2c3,
			device_addr,
			reg_addr,
			I2C_MEMADD_SIZE_8BIT,
			&rdata,
			1, HAL_MAX_DELAY);

	return rdata;
}

uint8_t HW_I2C_ReadReg(uint16_t device_addr, uint16_t reg_addr, uint8_t *pdata, uint8_t size)
{
	HAL_I2C_Mem_Read(
			&i2c3,
			device_addr,
			reg_addr,
			I2C_MEMADD_SIZE_8BIT,
			pdata,
			size,
			HAL_MAX_DELAY);

	return 0;
}
