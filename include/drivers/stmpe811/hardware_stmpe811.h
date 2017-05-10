/**
 * PDCL_OS
 * ----------------------------------------
 *
 * MIT License
 *
 * Copyright (c) 2017 robin
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

/**
 * @file	hardware_stmpe811.h
 * @author	robin
 * @date	Jan 31, 2017
 * @brief	[DESCRIPTION]
 */

#define STMPE811_REG_CHIP_ID        0x00
#define STMPE811_REG_ID_VER         0x02
#define STMPE811_REG_SYS_CTRL1      0x03
#define STMPE811_REG_SYS_CTRL2      0x04
#define STMPE811_REG_SPI_CFG        0x08
#define STMPE811_REG_INT_CTRL       0x09
#define STMPE811_REG_INT_EN         0x0A
#define STMPE811_REG_INT_STA        0x0B
#define STMPE811_REG_GPIO_EN        0x0C
#define STMPE811_REG_GPIO_INT_STA   0x0D
#define STMPE811_REG_ADC_INT_EN     0x0E
#define STMPE811_REG_ADC_INT_STA    0x0F
#define STMPE811_REG_GPIO_SET_PIN   0x10
#define STMPE811_REG_GPIO_CLR_PIN   0x11
#define STMPE811_REG_GPIO_MP_STA    0x12
#define STMPE811_REG_GPIO_DIR       0x13
#define STMPE811_REG_GPIO_ED        0x14
#define STMPE811_REG_GPIO_RE        0x15
#define STMPE811_REG_GPIO_FE        0x16
#define STMPE811_REG_GPIO_AF        0x17
#define STMPE811_REG_ADC_CTRL1      0x20
#define STMPE811_REG_ADC_CTRL2      0x21
#define STMPE811_REG_ADC_CAPT       0x22
#define STMPE811_REG_ADC_DATA_CH0   0x30
#define STMPE811_REG_ADC_DATA_CH1   0x32
#define STMPE811_REG_ADC_DATA_CH2   0x34
#define STMPE811_REG_ADC_DATA_CH3   0x36
#define STMPE811_REG_ADC_DATA_CH4   0x38
#define STMPE811_REG_ADC_DATA_CH5   0x3A
#define STMPE811_REG_ADC_DATA_CH6   0x3C
#define STMPE811_REG_ADC_DATA_CH7   0x3E
#define STMPE811_REG_TSC_CTRL       0x40
#define STMPE811_REG_TSC_CFG        0x41
#define STMPE811_REG_WDW_TR_X       0x42
#define STMPE811_REG_WDW_TR_Y       0x44
#define STMPE811_REG_WDW_BL_X       0x46
#define STMPE811_REG_WDW_BL_Y       0x48
#define STMPE811_REG_FIFO_TH        0x4A
#define STMPE811_REG_FIFO_STA       0x4B
#define STMPE811_REG_FIFO_SIZE      0x4C
#define STMPE811_REG_TSC_DATA_X     0x4D
#define STMPE811_REG_TSC_DATA_Y     0x4F
#define STMPE811_REG_TSC_DATA_Z     0x51
#define STMPE811_REG_TSC_DATA_XYZ   0x52
#define STMPE811_REG_TSC_FRACTION_Z 0x56
#define STMPE811_REG_TSC_DATA       0x57
#define STMPE811_REG_TSC_I_DRIVE    0x58
#define STMPE811_REG_TSC_SHIELD     0x59
#define STMPE811_REG_TEMP_CTRL      0x60
#define STMPE811_REG_TEMP_DATA      0x61
#define STMPE811_REG_TEMP_TH        0x62
