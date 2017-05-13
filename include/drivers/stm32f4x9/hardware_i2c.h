/*
 * hardware_i2c.h
 *
 *  Created on: May 9, 2017
 *      Author: robin
 */

#ifndef INCLUDE_DRIVERS_STM32F4X9_HARDWARE_I2C_H_
#define INCLUDE_DRIVERS_STM32F4X9_HARDWARE_I2C_H_

#ifdef __cplusplus
extern "C" {
#endif

void HW_I2C_Init();
void HW_I2C_DeInit();
void HW_I2C_WriteReg8(uint16_t device_addr, uint16_t reg_addr, uint8_t pdata);
uint8_t HW_I2C_ReadReg8(uint16_t device_addr, uint16_t reg_addr);
uint8_t HW_I2C_ReadReg(uint16_t device_addr, uint16_t reg_addr, uint8_t *pdata, uint8_t size);

#ifdef __cplusplus
}
#endif

#endif /* INCLUDE_DRIVERS_STM32F4X9_HARDWARE_I2C_H_ */
