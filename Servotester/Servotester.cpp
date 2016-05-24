#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define OCR_MIN 112
#define OCR_MAX 250
#define ICR_MAX F_CPU/64/55.7 //ICR1(TOP) = fclk/(N*f) ; N-��������; f-�������; 

//������������� ���:
void adc_init(void){
	ADCSRA |= (1<<ADEN)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0); // �������� ������� = 128

//	ADMUX |= (1<<MUX0);   // �������� ���� ADC1
}

//������ ������ ���:
uint16_t adc_read(){
	ADCSRA &= ~_BV(ADIF);           // ���������� ��� ��������� ���������
	ADCSRA |= _BV(ADSC);            // �������� ���������
	
	while(ADCSRA & _BV(ADSC));      // ������� ��������� ���������. ���������� ���� ������ �������� �� ����������.
	
	return ADC;             // ���������� ADC (10 ���);
}

//������������� ������
void Port_init (void)
{
	//���� �� ����������
	DDRB |= (1<<PB1);
}

//�������������� �������
void timer_init (void)
{
	TCCR1A |= (1<<COM1A1)|(0<<COM1A0)|(0<<WGM10)|(1<<WGM11); // ������������� ����� ��� Fast PWM
	ICR1=ICR_MAX;
	OCR1A = OCR_MIN; // ���������� ��������� �� �������
	TCCR1B |=(1<<CS10)|(1<<CS11)|(0<<CS12)|(1<<WGM12)|(1<<WGM13); //������ �������
}

//������������� ����������
void init_interp (void)
{
//	MCUCR |=(1<<ISC01)|(1<<ISC11); //������� ��������� ���������� �� ������� INT0 � INT1
//	GICR |= (1<<INT0); //���������� ���������� �� INT0
}

//���������� ����������
ISR (INT0_vect) //������� �� ��������� ����� ��� ������ ������
{

}

//������������ ������� map
long map(long x, long in_min, long in_max, long out_min, long out_max)
{
	return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

int main (void)
{
	uint16_t adc_result;

	Port_init(); //������������� ������
//	init_interp(); //������������� ������� ����������
	adc_init(); //������������� ADC
	timer_init(); //������������� �������
//	sei(); //���������� ���������� ����������	

	while(1){
		adc_result=adc_read(); //�������� ��������� �������������
		OCR1A = map(adc_result,0,1024,OCR_MIN,OCR_MAX); //������������ ��������� ������������� � �������� OCR		
//		OCR1A=200;

	}
}