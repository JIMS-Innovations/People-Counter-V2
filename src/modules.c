/**
 * @file modules.c
 * @author Jesutofunmi Kupoluyi (jimsufficency@gmail.com)
 * @brief This is the source file for the modules used in the 
 * automatic irrigation device in C.
 * @version 0.1
 * @date 2023-10-09
 * 
 * @copyright Copyright (c) 2023
 * 
 */

/* Include header */
#include "modules.h"


/* Indicator low level function */
void indicator_init(indicator *ind)
{
    (*(ind->ddr)) |= _BV(ind->pin);
}

void indicator_on(indicator *ind)
{ 
    (*(ind->port)) |= (1 << (ind->pin));
}

void indicator_off(indicator *ind)
{ 
    (*(ind->port)) &= ~(1 << (ind->pin));
}

/*Indicator high level functions */
void indicator_blink(indicator *ind, int delay_ms)
{
    (*(ind->port)) ^= (1 << ind->pin);
    _delay_ms(delay_ms);
}

/* LCD low level functions */
void LCD_Write_Command(lcd *display, uint8_t command) 
{
  /** 
     * Sending the upper nibble of command:
     * Clear lower 4 pins of the port
     * Send upper nibble of the command
     */
  (*(display->port)) = ((*(display->port)) & 0x0F) | (command & 0xF0);

  (*(display->port)) &= ~(1 << display->rs); /* RS = 0; Command register */

  /* Enable pulse; HIGH --> LOW */
  (*(display->port)) |= (1 << display->e);
  _delay_us(1);
  (*(display->port)) &= ~(1 << display->e);
  _delay_ms(200);

  /** 
     * Sending the lower nibble of command:
     * Clear lower 4 pins of the port
     * Send lower nibble of the command
     */
  (*(display->port)) = ((*(display->port)) & 0x0F) | (command << 4);

  /* Enable pulse; HIGH --> LOW */
  (*(display->port)) |= (1 << display->e);
  _delay_us(1);
  (*(display->port)) &= ~(1 << display->e);
  _delay_ms(2);
}

/**
 * @brief LCD initialization function
 *
 */
void LCD_Init(lcd *display) 
{
  (*(display->ddr)) = (1 << display->d4) | (1 << display->d5) | (1 << display->d6) | (1 << display->d7) | (1 << display->rs) | (1 << display->e); /* PC0 and PC1 are unused */
  _delay_ms(20);          /* Power on delay > 15ms */

  /* Attempting manual initialization */
  LCD_Write_Command(display, 0x03);
  _delay_ms(5);
  LCD_Write_Command(display, 0x03);
  _delay_ms(1);
  LCD_Write_Command(display, 0x03);
  LCD_Write_Command(display, 0x02);
  _delay_ms(20);

  LCD_Write_Command(display, 0x02); /* 4 bit initialization of LCD  */
  _delay_ms(5);
  LCD_Write_Command(display, 0x28); /* 2 line, 5 * 7 matrix in 4 bit mode */
  _delay_ms(5);
  LCD_Write_Command(display, 0x0C); /* Display on, cursor off */
  _delay_ms(5);
  LCD_Write_Command(display, 0x06); /* Increment cursor: shift cursor to the right */
  _delay_ms(5);
  LCD_Write_Command(display, 0x01); /* Clear display screen */

  _delay_ms(10); /* delay */
}

/* LCD high level funcions */

/**
 * @brief Function for writing character to LCD
 *
 * @param data
 */
void LCD_Write_Char(lcd *display, uint8_t data) 
{
  /**
     * the upper nibble of data:
     * Clear lower 4 pins of the port
     * Send upper nibble of the data
     */
  (*(display->port)) = ((*(display->port)) & 0x0F) | (data & 0xF0);

  (*(display->port)) |= (1 << display->rs); /* RS = 1; data register */

  /* Enable pulse; HIGH --> LOW */
  (*(display->port)) |= (1 << display->e);
  _delay_us(1);
  (*(display->port)) &= ~(1 << display->e);
  _delay_ms(2);

  /** 
     * Sending the lower nibble of data:
     * Clear lower 4 pins of the port
     * Send lower nibble of the data
     */
  (*(display->port)) = ((*(display->port)) & 0x0F) | (data << 4);

  /* Enable pulse; HIGH --> LOW */
  (*(display->port)) |= (1 << display->e);
  _delay_us(1);
  (*(display->port)) &= ~(1 << display->e);
  _delay_ms(2);
}

/**
 * @brief Function to print string to LCD
 *
 * @param data
 */
void LCD_Write_String(lcd *display, char *data) 
{
  /* Iterate and print every character in string */
  for (int i = 0; *(data + i) != '\0'; i++)
    LCD_Write_Char(display, *(data + i));
}

/**
 * @brief Function for printing string to LCD with a specific position.
 *
 * @param row
 * @param pos
 * @param data
 */
void LCD_Write_String_XY(lcd *display, uint8_t row, uint8_t pos, char *data) 
{
  if (row == 0 && pos < 16)
    LCD_Write_Command(display, (pos & 0x0F) | 0x80);
  else if (row == 1 && pos < 16)
    LCD_Write_Command(display, (pos & 0x0F) | 0xC0);
  LCD_Write_String(display, data);
}

/**
 * @brief Function for clearing LCD screen
 *
 */
void LCD_Clear(lcd *display) 
{
  LCD_Write_Command(display, 0x01);
  _delay_ms(2);
  LCD_Write_Command(display, 0x02);
}

/* Pump low level function */
void pump_init(pump *p)
{
    (*(p->ddr)) |= _BV(p->pin);
}

void pump_on(pump *p)
{ 
    (*(p->port)) |= (1 << (p->pin));
}

void pump_off(pump *p)
{ 
    (*(p->port)) &= ~(1 << (p->pin));
}

/* ADC Function definitions */

/**
 * @brief ADC initialization function
 * @param sensor
 * 
 */
void ADC_Init(adc *sensor) {
  (*(sensor->ddr)) = 0x00; /* Make ADC port input */
  ADCSRA = 0x87;  /* Enable ADC, freq / 128 */
  ADMUX = 0x40;   /* Vref = AVcc, ADC channel: 0 */
}

/**
 * @brief ADC reading function
 * 
 * @param sensor 
 * @return int 
 */
int ADC_Read(adc *sensor) {

  ADMUX |= (sensor->pin & 0x07); /* Set input channel to read */

  ADCSRA |= (1 << ADSC); /* Start conversion */
  while ((ADCSRA & (1 << ADSC)))
    ; /* Monitor end of conversion interrupt */

  ADCSRA |= (1 << ADIF);
  sensor->data = ADC;

  return ADC;
}