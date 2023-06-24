//Works
//It generates a precise 40 kHz signal on pins 3 and 5


const byte OUT1 = 3;  // Timer 2 "B" output: OC2B //Timer 2 controls pwm on pin 3 and 11
const byte OUT2 = 5;   // Timer 0 "B" output: OC0B //Timer 0 controls pwm on pin 5 and 6
const byte OUT3 = A0;   // To LOW
const byte OUT4 = A1;   // To LOW
const long frequency = 40000L;  // Hz

void setup() 
 {
  pinMode (OUT1, OUTPUT);
  pinMode (OUT2, OUTPUT);

  TCCR2A = _BV (WGM20) | _BV (WGM21) | _BV (COM2B1); // fast PWM, clear OC2B on compare
  TCCR2B = _BV (WGM22) | _BV (CS21);         // fast PWM, prescaler of 8
  OCR2A =  ((F_CPU / 8) / frequency) - 1;    // zero relative  
  OCR2B = ((OCR2A + 1) / 2) - 1;             // 50% duty cycle

  TCCR0A = _BV (WGM00) | _BV (WGM01) | _BV (COM0B1); // fast PWM, clear OC1B on compare
  TCCR0B = _BV (WGM02) | _BV (CS01);         // fast PWM, prescaler of 8
  OCR0A =  ((F_CPU / 8) / frequency) - 1;    // zero relative  
  OCR0B = ((OCR0A + 1) / 2) - 1;             // 50% duty cycle

  digitalWrite(OUT3,0);
  digitalWrite(OUT4,0);
  }  // end of setup

void loop() { }
