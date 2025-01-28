/**
 * @file main.h
 * @author Jesutofunmi Kupoluyi (jimsufficiency@gmail.com)
 * @brief Firmware header for Bidirectional visitor counter
 * @version 0.2
 * @date 2023-10-24
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef __MAIN_H__
#define __MAIN_H__

/* CPU Defines */
#define __AVR_ATmega328__
#define F_CPU 8000000UL /* CPU frequency: 8MHz */

/* Including required libraries */
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>
#include <string.h>


/* LCD Defines */
#define LCD_RS      PB2
#define LCD_E       PB3
#define LCD_D4      PB4
#define LCD_D5      PB5
#define LCD_D6      PB6
#define LCD_D7      PB7
#define LCD_DDR     DDRB
#define LCD_PORT    PORTB

/* LED defines */
#define LED_1_pin   PD4
#define LED_1_DDR   DDRD
#define LED_1_PORT  PORTD

#define LED_2_pin   PD5
#define LED_2_DDR   DDRD
#define LED_2_PORT  PORTD

/* Relay defines */
#define RELAY_PIN   PD6
#define RELAY_DDR   DDRD
#define RELAY_PORT  PORTD

/* IR Sensor defines */
#define IR_SENSE_1  PD2
#define IR_SENSE_2  PD3
#define IR_PORT     PORTD
#define IR_PIN      PIND




#endif/*__MAIN_H__*/