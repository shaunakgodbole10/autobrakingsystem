#include <avr/io.h>
#include <compat/deprecated.h>
#include <util/delay.h>

#define TEST(x)	0b00000001<<(x)
#define MARGIN 0x01FF		//half of 3FF that is 1111111111 ; 10bit precision
#define rs_1 sbi(PORTD,2)
#define rs_0 cbi(PORTD,2)
#define rw_1 sbi(PORTD,3)
#define rw_0 cbi(PORTD,3)
#define en_1 sbi(PORTD,4)
#define en_0 cbi(PORTD,4)

uint16_t adc_val;
int ch=0,temp;
int digital_at[8];
unsigned char i;

int main(void)
{
	DDRB=0xFF;
	DDRC=0xFF;
	DDRD=0xFF;
	DDRA=0x00;
	PORTA=0xFF;	//pulling up
	ADCSRA=0x83;
	ADMUX=0x40;
	PORTC=0x00;
	lcd_init();
	while(1)
	{
		// 		ADMUX=0x00;
		// 		for (ch=0;ch<8;ch++)
		// 		{
			// 			ADMUX=0x00+ch;
			start_adc();
			while(!complete());
			PORTC=ADCL;
			PORTD=ADCH;
			adc_val=ADC;
			lcd_cmd(0x80);
			lcd_data_string("DISTANCE:");
			lcd_cmd(0x88);
			hex_ascii(ADC);
			if (ADC>MARGIN)
			{
				sbi(PORTD,5);
			}
			else
			{
				sbi(PORTD,6);
			}
			
			// 			ch=0;
			// 			if(ADC>MARGIN)
			// 			{
				// 				/*digital_at[ch]=1;*/
				// 				sbi(PORTB,0);
				// 				cbi(PORTB,1);
			// 			}
			// 			else
			// 			{
				// 				/*digital_at[ch]=0;*/
				// 				cbi(PORTB,0);
				// 				sbi(PORTB,1);
			// 			}
			/*_delay_ms(1000);*/
		}
		// 		ch=0;
		
	}



	void start_adc()
	{
		sbi(ADCSRA,6);
	}

	int complete()
	{
		if(ADCSRA&TEST(4))
		{
			return(1);
		}
		else
		return(0);
	}




//Command sending function.
void lcd_cmd(unsigned char item)
{
	PORTB=item;
	rs_0;
	rw_0;
	en_1;
	_delay_ms(10);
	en_0;
	return;
}

// DATASENDING FUNCTION
void lcd_data(unsigned char item)
{
	PORTB=item;
	rs_1;
	rw_0;
	en_1;
	_delay_ms(10);
	en_0;
	return;
}


void lcd_data_string(unsigned char *str)
{
	i=0;
	while(str[i]!='\0')
	{
		lcd_data(str[i]);
		i++;
	}
	return;
}


void lcd_init() // Function to initialize LCD
{
	lcd_cmd(0x38);
	_delay_ms(50);
	
	lcd_cmd(0x0E);
	_delay_ms(50);
	
	lcd_cmd(0x01);
	_delay_ms(50);
	
	lcd_cmd(0x06);
	_delay_ms(50);
	
	lcd_cmd(0x80);
	_delay_ms(50);
}

void hex_ascii(unsigned int temp)
{
	unsigned msb,lsb,middle;
	lsb=temp&0x00F;
	msb=temp&0x0F00;
	middle=temp&0x0F0;
	msb=msb>>(8);
	middle=middle>>(4);
	
	if(msb>0x09)
	{
		msb=msb+0x37;
		lcd_data(msb);
		msb=msb+16*16;
	}
	else
	{
		msb=msb+0x30;
		lcd_data(msb);
	}
	if(middle>0x09)
	{
		middle=middle+0x37;
		lcd_data(middle);
	}
	else
	{
		middle=middle+0x30;
		lcd_data(middle);
	}				
	
	if(lsb>0x09)
	{
		lsb=lsb+0x37;
		lcd_data(lsb);
	}
	else
	{
		lsb=lsb+0x30;
		lcd_data(lsb);
	}
}