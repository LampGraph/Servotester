#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define OCR_MIN 900/8 //Минимальная длина импульса
#define OCR_MAX 2000/8 //Максимальная длина импульса
#define ICR_MAX F_CPU/64/55.7 //ICR1(TOP) = fclk/(N*f) ; N-Делитель; f-Частота; 

//Инициализация АЦП:
void adc_init(void){
	ADCSRA |= (1<<ADEN)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0); // делитель частоты = 128
}

//Чтение канала АЦП:
uint16_t adc_read(){
	ADCSRA &= ~_BV(ADIF);           // сбрасываем бит окончания конверсии
	ADCSRA |= _BV(ADSC);            // начинаем конверсию
	
	while(ADCSRA & _BV(ADSC));      // ожидаем окончания конверсии. Правильнее этот момент повешать на прерывания, но не в этом случае
					//Здесь лучше оставить как есть, потому что прерывания могут мешать правильной генерации ШИМ.
	return ADC;             // возвращаем ADC (10 бит);
}

//Инициализация портов
void Port_init (void)
{
	DDRB |= (1<<PB1); //Порт на выход
}

//Инициализируем таймеры
void timer_init (void)
{
	TCCR1A |= (1<<COM1A1)|(0<<COM1A0)|(0<<WGM10)|(1<<WGM11); // устанавливаем режим шим Fast PWM и поведение при совпадении
	ICR1=ICR_MAX; //Устанавливаем частоту ~55,7Гц
	OCR1A = OCR_MIN; // выставляем минимум
	TCCR1B |=(1<<CS10)|(1<<CS11)|(0<<CS12)|(1<<WGM12)|(1<<WGM13); //запуск таймера
}

//Ардуиновская функция map, мне она очень нравится.
long map(long x, long in_min, long in_max, long out_min, long out_max)
{
	return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

int main (void)
{
	uint16_t adc_result;

	Port_init(); //инициализация портов
	adc_init(); //Инициализация ADC
	timer_init(); //инициализация таймера

	while(1){
		adc_result=adc_read(); //Получаем положение потенциометра
		OCR1A = map(adc_result,0,1024,OCR_MIN,OCR_MAX); //Конвертируем положение потенциометра в значение OCR		
	}
}
