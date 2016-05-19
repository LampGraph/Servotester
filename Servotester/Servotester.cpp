#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

void Port_init (void)
{
	DDRB =0xFF; //2-� ������ ����� �� ����� ��� ��� ��� ��������� ����������
	PORTB=0x00; // 4�� ����� ���� �� �����(����� OC0)
}

void timer_init (void)
{
	TCCR1A|=(0<<WGM10)|(1<<WGM11); // ������������� ����� ��� Fast PWM
	TCCR1A|=(1<<COM1A1); // ���������� �������� ��� ��������� �����
	ICR1=4999; 
	OCR1A = 225; // ���������� ����� ���������
	TCCR1B |=(1<<CS10)|(1<<CS11)|(0<<CS12)|(1<<WGM12)|(1<<WGM13); //������ �������
}

void init_interp (void)
{
	MCUCR |=(1<<ISC01)|(1<<ISC11); //������� ��������� ���������� �� ������� INT0 � INT1
//	GICR |= (1<<INT0); //���������� ���������� �� INT0
}

char i=7; //������� �� ��������� ����� ��� ������� ������
ISR (INT0_vect) //������� �� ��������� ����� ��� ������ ������
{
	if((!(PIND&1))&&(i>7))i-=1;

	if((PIND&1)&&(i<37))i+=1;
	PORTB=i;
}

int main (void)
{
	Port_init(); //������������� ������
	//	init_interp(); //������������� ������� ����������
	timer_init(); //������������� �������
	//	sei(); //���������� ���������� ����������

	while(1){
/*		for (int i=225;i<=500;i++)
		{
		OCR1A = i;
		_delay_ms(100);		
		}
		for (int i=500;i>=225;i--)
		{
			OCR1A = i;
			_delay_ms(100);
		}
		*/
		OCR1A=500;
//		PORTA=i;
	}
}