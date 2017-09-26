#define F_CPU 12000000
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

unsigned char ADC_Conversion(unsigned char);
unsigned char ADC_Value;

unsigned char a,b,c,d,e,value;

void init_devices (void) //use this function to initialize all devices
{
	cli(); //disable all interrupts
	adc_init();
	sei(); //re-enable interrupts
}

void adc_init()
{
	ADCSRA = 0x00;
	ADMUX = 0x20;		//Vref=5V external --- ADLAR=1 --- MUX4:0 = 0000
	ACSR = 0x80;
	ADCSRA = 0x86;		//ADEN=1 --- ADIE=1 --- ADPS2:0 = 1 1 0
}

//This Function accepts the Channel Number and returns the corresponding Analog Value
unsigned char ADC_Conversion(unsigned char Ch)
{
	unsigned char a;
	Ch = Ch & 0x07;
	ADMUX= 0x20| Ch;
	ADCSRA = ADCSRA | 0x40;		//Set start conversion bit
	while((ADCSRA&0x10)==0);	//Wait for ADC conversion to complete
	a=ADCH;
	ADCSRA = ADCSRA|0x10; //clear ADIF (ADC Interrupt Flag) by writing 1 to it
	return a;
}
void forward (void) //both wheels forward
{
	PORTD=0b01100000;
}

void back (void) //both wheels backward
{
	PORTD=0b10010000;
}

void left (void) //Left wheel backward, Right wheel forward
{
	PORTD=0b01010000;
}

void right (void) //Left wheel forward, Right wheel backward
{
	PORTD=0b10100000;
}

void soft_left (void) //Left wheel stationary, Right wheel forward
{
	PORTD=0b01000000;
}

void soft_right (void) //Left wheel forward, Right wheel is stationary
{
	PORTD=0b00100000;
}

void stop (void) //hard stop
{
	PORTD=0b00000000;
}

void inch(void)
{
	forward();
	_delay_ms(150);
}

//Main Function
int main()
{
	char h,l;
	h=80;l=20;
	
	DDRB=0b00001111;		//LED
	PORTB=0b00000000;
	DDRD=0b11110000;		//MOTOR
	PORTD=0b00000000;
	
	init_devices();
	
	int p[100],i,j,flag=1;
	for (i=0;i<100;i++)
	{
		p[i]=-1;
	}
	
	while(flag==1)
	{
		a=ADC_Conversion(0);	//PC0
		b=ADC_Conversion(1);	//PC1
		c=ADC_Conversion(2);	//PC2
		d=ADC_Conversion(3);	//PC3
		e=ADC_Conversion(4);	//PC4
		
		if (a<l && b<l && c<l && d<l && e<l)
		{
			right();
			_delay_ms(300);
			p[i]=4;
			i=i+1;
		}
		
		else if ((a>h && b>h && c>h )||(a>h && b>h && c>h && d>h))
		{
			inch();
			if(a<l && b<l && c<l && d<l && e<l)
			{
				right();
				_delay_ms(300);
				p[i]=3;
				i=i+1;
			}
			else
			{
				forward();
				p[i]=2;
				i=i+1;
			}
		}
		else if (a>h && b>h && c>h && d>h && e>h)
		{
			inch();
			if (a>h && b>h && c>h && d>h && e>h)
			{
				//end of maze
				stop();
				flag=0;
				int *sort(int *p,int j);
				int *remove(int *p,int j);
			}
			else
			{
				left();
				_delay_ms(300);
				p[i]=1;
				i=i+1;
			}
		}
		else if ((e>h && d>h && c>h && b<l && a<l)||(e>h && d>h && c>h && b>h && a<l))
		{
			
			left();
			_delay_ms(300);
			p[i]=1;
			i=i+1;
		}
		else if(e>h && d>h)
		{
			soft_left();
		}
		else if(a>h && b>h)
		{
			soft_right();
		}
		else if((c>h && b>h) || ( b>h && d>h))
		{
			forward();
		}
		
	}
	
	int *sort(int *p,int j)
	{
		int i=3;
		while(i<j)
		{
			if(p[i-3]==1 && p[i-2]==4 && p[i-1]==1)
			{
				p[i-3]=2;
				p[i-2]=0;
				p[i-1]=0;
				
			}
			else if(p[i-3]==1 && p[i-2]==4 && p[i-1]==2)
			{
				p[i-3]=3;
				p[i-2]=0;
				p[i-1]=0;
			}
			else if(p[i-3]==2 && p[i-2]==4 && p[i-1]==1)
			{
				p[i-3]=3;
				p[i-2]=0;
				p[i-1]=0;
			}
		} i++;
	}
	return p;
	int *remove(int *p,int j)
	{
		int i=0,a[100];
		while(i<j)
		{
			if(p[i]!=0)
			{
				a[j]=p[i];
				j++;
			}
			i++;
		}
		return a;
	}
}