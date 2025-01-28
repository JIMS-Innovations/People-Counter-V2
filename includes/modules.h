/**
 * @file modules.h
 * @author Jesutofunmmi Kupoluyi (jimsufficiency@gmail.com)
 * @brief This is a header file containing the struct definitions
 * for the hardware modules used.
 * @version 0.1
 * @date 2023-10-04
 * 
 * @copyright Copyright (c) 2023
 * 
 */
/* Including required libraries */
#include "main.h"

/* Indicator struct */
typedef struct _indicator
{
    uint8_t pin;
    volatile uint8_t* port;
    volatile uint8_t* ddr;
} indicator;

/* Pump struct */
typedef struct _pump
{
    uint8_t pin;
    volatile uint8_t* port;
    volatile uint8_t* ddr;
} pump;

/* ADC struct */
typedef struct _adc
{
    uint8_t pin;
    volatile uint8_t* port;
    volatile uint8_t* ddr;
    int data;
} adc;

typedef struct _lcd
{
    uint8_t d4;
    uint8_t d5;
    uint8_t d6;
    uint8_t d7;
    uint8_t rs;
    uint8_t e;
    volatile uint8_t* port;
    volatile uint8_t* ddr;
} lcd;

/* Indicator Function prototypes */
void indicator_init(indicator *);
void indicator_on(indicator *);
void indicator_off(indicator *);
void indicator_blink(indicator *, int);

/* LCD Function prototypes */
void LCD_Init(lcd *);
void LCD_Write_Command(lcd *, uint8_t);
void LCD_Write_Char(lcd *, uint8_t);
void LCD_Write_String(lcd *, char *);
void LCD_Write_String_XY(lcd *, uint8_t, uint8_t, char *);
void LCD_Clear(lcd *);

/* Pump function prototypes */
void pump_init(pump *);
void pump_on(pump *);
void pump_off(pump *);

/* ADC function prototypes */
void ADC_Init(adc *);
int ADC_Read(adc *);