#include <avr/interrupt.h>
#include <avr/sleep.h>
#include "pitches.h"

/**************************************************************************************************/
/*                                    MEMORIA ROM                                                 */
/**************************************************************************************************/
#define PIN_ACTIVAR 2    // (pin 7)
#define PIN_PIEZO 1      // (pin 6)
#define NUM_NOTAS 89

const int notas[NUM_NOTAS] =
{
    NOTE_B0, NOTE_C1, NOTE_CS1, NOTE_D1, NOTE_DS1, NOTE_E1, NOTE_F1, NOTE_FS1, NOTE_G1, NOTE_GS1, 
    NOTE_A1, NOTE_AS1, NOTE_B1, NOTE_C2, NOTE_CS2, NOTE_D2, NOTE_DS2, NOTE_E2, NOTE_F2, NOTE_FS2,
    NOTE_G2, NOTE_GS2, NOTE_A2, NOTE_AS2, NOTE_B2, NOTE_C3, NOTE_CS3, NOTE_D3, NOTE_DS3, NOTE_E3,
    NOTE_F3, NOTE_FS3, NOTE_G3, NOTE_GS3, NOTE_A3, NOTE_AS3, NOTE_B3, NOTE_C4, NOTE_CS4, NOTE_D4,
    NOTE_DS4, NOTE_E4, NOTE_F4, NOTE_FS4, NOTE_G4, NOTE_GS4, NOTE_A4, NOTE_AS4, NOTE_B4, NOTE_C5,
    NOTE_CS5, NOTE_D5, NOTE_DS5, NOTE_E5, NOTE_F5, NOTE_FS5, NOTE_G5, NOTE_GS5, NOTE_A5, NOTE_AS5,
    NOTE_B5, NOTE_C6, NOTE_CS6, NOTE_D6, NOTE_DS6, NOTE_E6, NOTE_F6, NOTE_FS6, NOTE_G6, NOTE_GS6,
    NOTE_A6, NOTE_AS6, NOTE_B6, NOTE_C7, NOTE_CS7, NOTE_D7, NOTE_DS7, NOTE_E7, NOTE_F7, NOTE_FS7,
    NOTE_G7, NOTE_GS7, NOTE_A7, NOTE_AS7, NOTE_B7, NOTE_C8, NOTE_CS8, NOTE_D8, NOTE_DS8
};

/**************************************************************************************************/
/*                                    MEMORIA RAM                                                 */
/**************************************************************************************************/
volatile int activar;
int nota, tiempo, t_max, tiempo_nota, tiempo_entre_nota;

/**************************************************************************************************/
/*                                     PROGRAMA                                                   */
/**************************************************************************************************/
void Int0ISR()
{    
    if(activar == 0)
        activar = 1;
    else
        activar = 0;
}

void setup(void)
{
    pinMode(PIN_ACTIVAR, INPUT_PULLUP);
    pinMode(PIN_PIEZO, OUTPUT);
    
    digitalWrite(PIN_PIEZO, LOW);
    set_sleep_mode(SLEEP_MODE_PWR_DOWN);
    randomSeed(micros());
    
    activar = 0;
    
    attachInterrupt(0, Int0ISR, LOW);
    aDormir();
}

void loop()
{
    if(activar == 1)
    {
        nota = random(0, NUM_NOTAS);
        tiempo = random(5, 20);
  
        reproducir(nota, tiempo);
    }
    else
    {
        delay(100);
        digitalWrite(PIN_PIEZO, LOW);
        aDormir();
    }
}

void aDormir()
{ 
    sleep_enable();  
    sleep_cpu();
    sleep_disable();
    
    delay(100);
}

void reproducir(int n, int t)
{
    tiempo_nota = 1000/t;
    tiempo_entre_nota = tiempo_nota * 1.3;
    
    buzz(PIN_PIEZO, notas[n], tiempo_nota);
    delay(tiempo_entre_nota);
}

void buzz(int targetPin, long frequency, long length) 
{
    long delayValue = (1000000/frequency/2); // calculate the delay value between transitions
    //// 1 second's worth of microseconds, divided by the frequency, then split in half since
    //// there are two phases to each cycle
    
    long numCycles = frequency * length/ 1000; // calculate the number of cycles for proper timing
    //// multiply frequency, which is really cycles per second, by the number of seconds to 
    //// get the total number of cycles to produce
    
    for (long i=0; i < numCycles; i++) // for the calculated length of time...
    {   
        digitalWrite(targetPin,HIGH); // write the buzzer pin high to push out the diaphram
        delayMicroseconds(delayValue); // wait for the calculated delay value
        digitalWrite(targetPin,LOW); // write the buzzer pin low to pull back the diaphram
        delayMicroseconds(delayValue); // wait again or the calculated delay value
    }
}
