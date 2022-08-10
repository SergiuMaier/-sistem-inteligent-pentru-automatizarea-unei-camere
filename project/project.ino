#include <LiquidCrystal.h>
#define ADC_VREF_TYPE ((0 << REFS1) | (0 << REFS0) | (0 << ADLAR))

LiquidCrystal lcd(4, 5, 3, 12, 10, 11); // RS, EN, D4, D5, D6, D7

long duration; 
volatile float distance_cm; 
int angle=-180;
long contor=0;
long timp=0;//contor ultrasonic
int mili=0;//0.1ms
int milisecunde=0;
int secunde=0;
int contorLed=0,contorUsa=0;
long t1=0;
long t2=0;
int afisat1data=0;
long flag1=0;
long flag2=0;
long flag3=0;
volatile float LastPulseTime=0;
unsigned int valoareSenzor;
float temperatura;
float stepADC = 0.004882814;  // 5/1024

void setup() {

  lcd.begin(16, 2); 

  PORTD = B00000000;
  DDRB |=B00110010;
  DDRD &=B01111011;

  //TIMER0
  TCCR0A = (1 << WGM01) | (0 << WGM00);//selectam valoarea de TOP in OCR0A simodul de operare CTC
  TCNT0 = 0;//registru ptr valoare acumulata
  OCR0A = 20;//valoare de top; intrerupere la 10 microsecunde
  TCCR0B = (0 << WGM02);//pereche cu TCCR0A
  TCCR0B = (0 << CS02) | (1 << CS01) | (0 << CS00);//selectare prescalar
  TIFR0 |= B00000010;//registru de flaguri(folosim OCFA)
  TIMSK0 |= B00000010;//(1 << OCIE0A); permite intrerupere

  //TIMER1 pentru servomotor, pulsuri PWM
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1 = 0;
  TCCR1A = 0b10100010;
  TCCR1B = 0b00010010;
  ICR1 = 20000;
  OCR1A = 580;

  //INTRERUPERE EXTERNA pentru intrarea de la senzorul ultrasonic
  EICRA |= (0 << ISC11 ) | (1<< ISC10 ) | (0 << ISC01 ) | (1 << ISC00 );  
  EIMSK |= (0 << INT1 ) | (1 << INT0 ) ;
  EIFR |= (0 << INTF1 ) | (0 << INTF0 ) ;
  
  initializareADC();
  SREG |= (1 << SREG_I);//activare intr. globale
}

ISR(TIMER0_COMPA_vect)
{
  SREG &= ~(1 << SREG_I);//dezactivare intr. globale
  ++contor;//10us sau 0.01ms
  ++timp;
  ++flag1;//resetabil
  ++flag2;//resetabli
  if(contor>=10)//0.1ms
  {
    ++mili;
    contor=0;
    if(mili>=200)//20ms
    {
      ++milisecunde;
      mili=0;
      if(milisecunde>=50)//1s
      {
        ++secunde;
        milisecunde=0;
      }
    }
  }
  SREG |= (1 << SREG_I);//act. intr. globale
}

ISR ( INT0_vect )  //intr. externa ptr masurarea duratei pulsului la ultrasonic
{
  SREG &= ~(1 << SREG_I ) ; 
  static unsigned long startTime;
  
  if ( PIND & B00000100 )
  { 
    startTime = timp; //primul front al semnalului 
  } 
  else //al doilea front
  {  
    LastPulseTime = timp - startTime; //calculare timp intre cele doua fronturi 
    timp=0;//resetare contor ultrasonic
  } 
  
  valoareSenzor = citesteADC(0);//citirea val analogice ptr temperatura
  temperatura = (valoareSenzor/1024.0)*4.9*35.75-50;  //calculare temperatura in grade Celsius
  
  SREG |= (1 << SREG_I ) ; 
}


unsigned int citesteADC(unsigned char adc_input)
{
  ADMUX = adc_input | ADC_VREF_TYPE;
  delayMicroseconds(10);//delay ptr stabilizare adc
  
  ADCSRA |= (1 << ADSC);//start conversie
  
  while ((ADCSRA & (1<<ADIF)) == 0){}//asteptare finalizare conversie
  
  ADCSRA |= (1<<ADIF);
  
  return ADCW;//returnare rezultat
}

void initializareADC()
{
  DIDR0 = (0<<ADC5D) | (0 << ADC4D) | (0 << ADC3D) | (0 << ADC2D) | (0 << ADC1D) | (0 << ADC0D); //dezactivare buffer de intrare digitala
  ADMUX = ADC_VREF_TYPE;//configurare multiplexor adc
  ADCSRA = (1 << ADEN) | (0 << ADSC) | (1 << ADATE) | (0 << ADIF) | (0 << ADIE) | (1 << ADPS2) | (0 << ADPS1) | (0 << ADPS0);//registru de control si stare
}

void loop() { 
  
  lcd.setCursor(0, 0);
  lcd.print("T=");
  lcd.setCursor(9, 1);
  lcd.print("Sec=");
  lcd.setCursor(13, 1);
  lcd.print(secunde);
  
  if(afisat1data==0)
  {
    lcd.setCursor(2,0);
    lcd.print(temperatura);
    ++afisat1data;
  }

  if (PINB & B00000001) {//verificare semnal de la PIR(pin 8)
    PORTD |= B01000000;//activare LED (pin 6)
    contorLed = secunde;//masurare durata led aprins
    lcd.setCursor(13,0);
    lcd.print("ON ");
  }
  else{}
   
  if  ((secunde - contorLed) >= 5) {//perioada led aprins in sec
    PORTD &= B10111111;//stingere led (pin 6)
    lcd.setCursor(13,0);
    lcd.print("OFF");
  }
 
  if(flag1>=10)//initializare pin trig pentru ultrasonic
  {
    PORTB &= B11011111;//semnal 0 pe pinul trig
  }
  
  if(flag1>=100)//delay trig
  {
    PORTB |= B00100000;//valoare 1 pe pinul trig
  }
  
  if(flag1>=200)//delay
  {
    PORTB &= B11011111;//dezactivare pin trig
    flag1=0;//resetare delay ultrasonic
  }

  distance_cm = LastPulseTime*10;//compensez ptr intreruperea de 10 microsecunde
  distance_cm=distance_cm*0.0345;//viteza sunetului~=340m/s
  distance_cm=distance_cm*0.5;// masurat timp ptr dus-intors
  lcd.setCursor(0, 1);
  lcd.print("d=");
    
  if(distance_cm<10)
  {
    OCR1A=1550;//deschidere usa prin setare valoare de top timer1 
    contorUsa=secunde;//start contor pentru timpul in care usa e deschisa
  }
  
  if((temperatura<25)&&(secunde-contorUsa>=5))//conditie de temperatura& contor ptr usa deschisa
  {
    OCR1A=580;//inchidere usa
  }
  
  if(flag2>100000)//afisare o data pe secunda
  {
    if(distance_cm<=400)
    {
      lcd.setCursor(2,1);
      lcd.print(distance_cm);
    }
    lcd.setCursor(9,0);
    lcd.print("LED:");
         
    ++flag3;
    flag2=0;
    
    if(flag3>=3)//afisare o data la 3 sec
    {
       lcd.setCursor(2,0);
       lcd.print(temperatura);
       flag3=0;
       }    
     }
  //lcd.clear();
}
