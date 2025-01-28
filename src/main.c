/**
 * @file main.c
 * @author Jesutofunmi Kupoluyi (jimsufficiency@gmail.com)
 * @brief Firmware source for Bidirectional visitor counter
 * @version 0.2
 * @date 2023-10-24
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include "main.h"
#include "modules.h"

/* Create an LCD instance */
lcd screen = {
	.d4 	= LCD_D4,
	.d5 	= LCD_D5,
	.d6 	= LCD_D6,
	.d7 	= LCD_D7, 
	.rs 	= LCD_RS,
	.e  	= LCD_E, 
	.port 	= &LCD_PORT,
	.ddr 	= &LCD_DDR
};

/* Create indicator instances */
indicator led_1 = {
	.pin 	= LED_1_pin,
	.port 	= &LED_1_PORT,
	.ddr 	= &LED_1_DDR
};

indicator led_2 = {
	.pin 	= LED_2_pin,
	.port 	= &LED_2_PORT,
	.ddr 	= &LED_2_DDR
};

indicator relay = {
	.pin 	= RELAY_PIN,
	.port 	= &RELAY_PORT,
	.ddr 	= &RELAY_DDR
};

/* Variables */
uint8_t int_flag_1 = 0;
uint8_t int_flag_2 = 0;
uint8_t int_last_state_1 = 0;
uint8_t int_last_state_2 = 0;
volatile int count = 0;

char lcd_buffer[16];


ISR (PCINT2_vect)
{
	/* Debounce pin */
  	for (uint32_t i = 0; i < 1000000000UL; i++)
		for (uint32_t j = 0; j < 10000000UL; j++)
			;

	/* entry sensor */
	if (!(IR_PIN & _BV(IR_SENSE_1))) 
	{
		if (int_last_state_1 == 0) 
		{
      		int_last_state_1 = 1;
    	}
	}
	else if(int_last_state_1 == 1)
	{
		int_last_state_1 = 0;
		if (int_flag_1 == 0 && int_flag_2 == 0) 
		{
			int_flag_1 = 1;
		}
		else if (int_flag_1 == 0 && int_flag_2 == 1) 
		{
			count--;
			int_flag_2 = 0;
		}
	}
	

  	/* exit sensor */
  	if (!(IR_PIN & _BV(IR_SENSE_2))) 
  	{ 
		if (int_last_state_2 == 0) 
		{
			int_last_state_2 = 1;
		}
  	}		
	else if(int_last_state_2 == 1)
	{
		int_last_state_2 = 0;
		if (int_flag_1 == 0 && int_flag_2 == 0) 
		{
			int_flag_2 = 1;
		}
		else if (int_flag_1 == 1 && int_flag_2 == 0) 
		{
			count++;
			int_flag_1 = 0;
		}
	}
  

}

/* Prototype main function as a void function */
/* void main(void) __attribute__((noreturn)); */

int main()
{
	/* Setting up pin change interrupts */
	PCICR |= (1 << PCIE2);
	PCMSK2 = (1 << PCINT18) | (1 << PCINT19);

	/* Enable global interrupts i.e. set the I bit in the SREG register */
	sei();

    /* Initialize modules */
    indicator_init(&led_1);
    indicator_init(&led_2);
    indicator_init(&relay);

    LCD_Init(&screen);

    _delay_ms(250);

    LCD_Write_String_XY(&screen, 0, 0, "Advanced People");
    LCD_Write_String_XY(&screen, 1, 0, "Counter Project");

	_delay_ms(250);

	LCD_Clear(&screen);
	

    while(1)
    {
		/* Check for valid count */
		count < 0 ? count = 0 : count;

		/* Display people count */
		sprintf(lcd_buffer, "People Cnt: %d", count);

		LCD_Write_String_XY(&screen, 0, 0, lcd_buffer);

        /* Control logic */
		if (count > 0)
		{
			indicator_on(&led_1);
			indicator_off(&led_2);
			indicator_off(&relay);
    	}
		else
		{
			indicator_off(&led_1);
			indicator_on(&led_2);
			indicator_on(&relay);
		}
		
	}

	return 0;
}