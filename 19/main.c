/* * GccApplication1.c * * Created: 2019/02/26 23:27:36 * Author : fiord */ 
#define F_CPU 8000000
#include <avr/io.h>
#include <util/delay.h>
#undef DEBUG

const unsigned long long border = 0b0111011101LL;

void setB() {
#ifdef DEBUG
  DDRB &= 0x11111110;
  PORTB = 0;
#else
  DDRB |= 0b00000001;
  PORTB = 1;
#endif
}

void changeB() {
  DDRB ^= 1;
  PORTB ^= 1;
}

void setD() {
  DDRD &= 0b11111110;
  PORTD = 0;
}

void changeD() {
  DDRD ^= 1;
  PORTD ^= 1;
}

void init() {
  setB();
  setD();
}

unsigned long long get() {
  // start reading
  ADCSRA |= 1<<4;
  ADCSRA |= 1<<6;

  // wait for ADC finishing
  while(1) {
    if(ADCSRA&(1<<4)) {
      break;
    }
  }

  unsigned long long ret = ADC;
  return ret;
}

int main(void) {
  init();

  unsigned long long volt = 0;
  
  // http://ziqoo.com/wiki/index.php?AVR%20A%2FD%CA%D1%B4%B9#se27336e …
  ADMUX = 0b01000000;
  ADCSRA = 0b10000111;
  DIDR0 = 0b00000001;

  unsigned long long nothing = 0;
  unsigned long long democnt = 0;
  int mode = 1;
  
  while(1) {
    // 拍手検出について。
    // 拍手音を録音・解析してみると、音量的には0.01sの間にかなり小さくなることが分かる。
    // ADCにかかる時間はμsオーダーなので、__delay_ms(10)を挟んで再度計測、音量が小さくなっているかどうかで判定が可能。
    volt = get();
#ifdef DEBUG
    _delay_ms(100);
    if(volt < border) {
      PORTB |= 1;
      PORTD |= 1;
      DDRB |= 1;
      DDRD |= 1;
    }
    else {
      PORTB &= 0xEF;
      PORTD &= 0xEF;
      DDRB &= 0xEF;
      DDRD &= 0xEF;
    }
#else
    if(volt < border) {
      _delay_ms(10);
      volt = get();
      if(border <= volt) {
        // swap
        changeB();
        changeD();
        _delay_ms(10);
        nothing = 0;
      }
      else {
        nothing++;
        if(nothing >= 10000) {
          nothing--;
          democnt += mode;
          if(democnt == 1000) {
            changeB();  changeD();
            mode=-1;
          }
          if(democnt == 0) {
            changeB();  changeD();
            mode=1;
          }
        }
      }
    }
    else {
      nothing++;
      if(nothing>=10000){
        nothing--;
        democnt += mode;
        if(democnt == 1000) {
          changeB();  changeD();
          mode=-1;
        }
        if(democnt == 0) {
          changeB();  changeD();
          mode=1;
        }
      }
    }
#endif
  }
  return 0;
}
