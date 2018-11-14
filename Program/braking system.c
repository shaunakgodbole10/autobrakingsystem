// /*
//  * braking_system.c
//  *
//  * Created: 19/03/2016 12:10:32 AM
//  *  Author: Prasad
//  */


#include<avr/io.h>
#include<avr/delay.h>
#include<compat/deprecated.h>
#include<math.h>

//#define led PORTC
#define rs0 cbi(PORTA,0)
#define rs1 sbi(PORTA,0)
#define rw0 cbi(PORTA,1)
//#define rw1 sbi(PORTA,1)
#define en0 cbi(PORTA,2)
#define en1 sbi(PORTA,2)
uint16_t result;

void lcd_cmd(unsigned char cmd)
{
	rs0;
	_delay_ms(10);
	rw0;
	_delay_ms(10);
	PORTD=cmd;
	_delay_ms(10);
	en1;
	_delay_ms(10);
	en0;
	_delay_ms(10);
}

void lcd_data(unsigned char data)
{
	rs1;
	_delay_ms(10);
	rw0;
	_delay_ms(10);
	PORTD=data;
	_delay_ms(10);
	en1;
	_delay_ms(10);
	en0;
	_delay_ms(10);
}

void lcd_init()
{
	lcd_cmd(0x01);
	_delay_ms(10);	// Initialize LCD 2 Rows
	lcd_cmd(0x38);
	_delay_ms(10);	// Clear LCD
	lcd_cmd(0x80);
	_delay_ms(10);	// 1st Row 1st Column
	lcd_cmd(0x0C);
	_delay_ms(10);	// Display on, Cursor Blinking
	lcd_cmd(0x06);
	_delay_ms(10);	// Entry mode
}

void lcd_str(unsigned char *str)
{
	while(*str)
	{
		lcd_data(*str);
		//_delay_ms(1000);
		str++;
	}
}


 int read_pulse(unsigned char ch)
 {
 	DDRB = 0b11111110;
 	int wh = 1, alw = 0, r = 0;
 
 	TCCR1A = 0x00;
 	TCCR1B = (1 << CS11);
 	TCNT1 = 0x00;
 
 	for(int i = 0; i < 30000; i++)
 	{
 		int x = PINB & 0b00000001;
 
 		if(x == 1 && r == 0)
 		{
 			TCNT1 = 0;
 			r = 1;
 		}
 		else if(x == 1 && r == 1)
 		{
 			alw = 1;
 		}
 		else if(x == 0)
 		{
 			if(alw == 1)
 			{
 				result = TCNT1;
 				r = 0;
 				alw = 0;
 			}
 		}
 	}
	 return(result);
 	TCCR1B = 0x00;
 }

void main()
{
	int rv,distance,distance1,count,msb2,msb,msb1,lsb;
	//unsigned char led=0x00;
	MCUCSR|= (1<<JTD);
	DDRD=0xFF;
	DDRA=0xFF;
	DDRB=0x02;
	DDRC=0x0F;
	uint16_t d1,d2;
	_delay_ms(10);
	lcd_init();
	d2=0;

	//led=0x00;
	while(1)
	{
		//count=0;
		lcd_cmd(0x80);
		lcd_str("Distance=");
		lcd_cmd(0x8C);
		lcd_str("cm");
// 		sbi(PORTB,0);
// 		_delay_us(10);
// 		cbi(PORTB,0);
// 		while(PINB1==0);
// 			
// 		while(PINB1)
// 		{
// 			count=count++;
// 			_delay_us(5.8);
// 		}
// 			lcd_cmd(0x89);
// 			_delay_ms(10);
// 			msb=count & 0xF000;
// 			msb=msb>>12;
// 			lcd_data(msb);
// 			msb1=count&0xF00;
// 			msb1=msb1>>8;
// 			msb2=count&0xF0;
// 			msb2=msb2>>4;
// 			lsb=count&0x0F;
				//led=0x00;
 		for(int i = 0; i < 3; i++)
 		{
 			// Set ultrasonic port as output
 			DDRB = 0x02;
 			
 
 			// Give Trigger as 10us High pulse
 			sbi(PORTB,1);
 			_delay_us(10);
 
 			// Stop Trigger pulse
 			cbi(PORTB, 1);
 			//_delay_us(160);
 
 			// measure the width of pulse
 			result=read_pulse(1);
 
 			d1=(result/58);
			rv=((d2-d1)/(result+10));
 			lcd_cmd(0x89);
 			lcd_data((d1 / 100) + 48);
 			lcd_data(((d1 % 100) / 10) + 48);
 			lcd_data((d1 % 10) + 48);
			
			lcd_cmd(0xc0);
			lcd_str("velocity=");
			lcd_cmd(0xC9);
			lcd_data((rv / 100) + 48);
			lcd_data(((rv % 100) / 10) + 48);
			lcd_data((rv % 10) + 48);
 		}
		 d2=d1;
// 		if(d1 > 10)
// 		{
// 			led=0x01;
// 		} 
// 		else if(d1>=20)
// 		{
// 			led=0x03;
// 			
// 		} 
// 		else if(d1>=30)
// 		{
// 			led=0x07;			
// 		}
// 		else if(d1>=40)
// 		{
// 			led=0x0F;			
// 		}
// 		else if(d1>=50)
// 		{
// 			led=0x1F;
// 		}
// 		else if(d1>=60)
// 		{
// 			led=0x3F;
// 		}
// 		else if(d1>=70)
// 		{
// 			led=0x7F;			
// 		}
// 		else if(d1>=80)
// 		{
// 			led=0xFF;			
// 		}
// 		else
// 		{
// 			led=0x00;
// 		}
		
		PORTC=0x01;
// 		while((PINC & 1<<4)!=0)
// 		{
// 			lcd_cmd(0x01);
// 			d1=0;
// 			//PORTC=0x02;
// 		}
	}	
}



