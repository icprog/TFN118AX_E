/* Copyright (c) 2012 Nordic Semiconductor. All Rights Reserved.
 *
 * The information contained herein is property of Nordic Semiconductor ASA.
 * Terms and conditions of usage are described in detail in NORDIC
 * SEMICONDUCTOR STANDARD SOFTWARE LICENSE AGREEMENT.
 *
 * Licensees are granted free, non-transferable use of the information. NO
 * WARRANTY of ANY KIND is provided. This heading must NOT be removed from
 * the file.
 *
 */
#ifndef PCA10001_H
#define PCA10001_H

#define LED_START      18
#define LED0           18
#define LED_STOP       19
#define LED1           19
#define LED_PORT       NRF_GPIO_PORT_SELECT_PORT2
#define LED_OFFSET     2

#define BUTTON_START   16
#define BUTTON0        16
#define BUTTON_STOP    17
#define BUTTON1        17

#define RX_PIN_NUMBER  11
#define TX_PIN_NUMBER  9
#define CTS_PIN_NUMBER 10
#define RTS_PIN_NUMBER 8
#define HWFC           false

#define V1_PIN_NUMBER 0
#define V2_PIN_NUMBER 1
#define V3_PIN_NUMBER 2
#define V4_PIN_NUMBER 3

#define LNA_EN_PIN_NUMBER		4
#define LNA_SPDT_PIN_NUMBER	5	//1-Tx 0-Rx

#define BLINKY_STATE_MASK   0x01

#endif
