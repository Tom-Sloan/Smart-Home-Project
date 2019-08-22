

#include <Dusk2Dawn.h>
#include <Stepper.h> // Library for stepper 

long milInDay = 60000;
long current = 0;
int laSunrise  = 0;
int laSunset   = 0;

int Pin1 = 10;
int Pin2 = 11;
int Pin3 = 12;
int Pin4 = 13;
int _step = 0;
int count = 0;
boolean dir = false;//false = Counterclock

Dusk2Dawn toronto(43.888577, -79.739735, -4);

void times() {
  int laSunrise  = toronto.sunrise(2018, 5, 22, false);
  int laSunset   = toronto.sunset(2017, 5, 22, false);
}


void setup() {
  Serial.begin (9600);
  laSunrise  = toronto.sunrise(2018, 5, 22, false);
  laSunset   = toronto.sunset(2017, 5, 22, false);
  Serial.println(laSunrise);  // 418
  Serial.println(laSunset);   // 1004

  pinMode(Pin1, OUTPUT);
  pinMode(Pin2, OUTPUT);
  pinMode(Pin3, OUTPUT);
  pinMode(Pin4, OUTPUT);

}

void loop() {
  current = millis() % milInDay;
  if (laSunrise == current) {
    while (count < 4096) {
      switch (_step) {
        case 0:
          digitalWrite(Pin1, LOW);
          digitalWrite(Pin2, LOW);
          digitalWrite(Pin3, LOW);
          digitalWrite(Pin4, HIGH);
          break;
        case 1:
          digitalWrite(Pin1, LOW);
          digitalWrite(Pin2, LOW);
          digitalWrite(Pin3, HIGH);
          digitalWrite(Pin4, HIGH);
          break;
        case 2:
          digitalWrite(Pin1, LOW);
          digitalWrite(Pin2, LOW);
          digitalWrite(Pin3, HIGH);
          digitalWrite(Pin4, LOW);
          break;
        case 3:
          digitalWrite(Pin1, LOW);
          digitalWrite(Pin2, HIGH);
          digitalWrite(Pin3, HIGH);
          digitalWrite(Pin4, LOW);
          break;
        case 4:
          digitalWrite(Pin1, LOW);
          digitalWrite(Pin2, HIGH);
          digitalWrite(Pin3, LOW);
          digitalWrite(Pin4, LOW);
          break;
        case 5:
          digitalWrite(Pin1, HIGH);
          digitalWrite(Pin2, HIGH);
          digitalWrite(Pin3, LOW);
          digitalWrite(Pin4, LOW);
          break;
        case 6:
          digitalWrite(Pin1, HIGH);
          digitalWrite(Pin2, LOW);
          digitalWrite(Pin3, LOW);
          digitalWrite(Pin4, LOW);
          break;
        case 7:
          digitalWrite(Pin1, HIGH);
          digitalWrite(Pin2, LOW);
          digitalWrite(Pin3, LOW);
          digitalWrite(Pin4, HIGH);
          break;
        default:
          digitalWrite(Pin1, LOW);
          digitalWrite(Pin2, LOW);
          digitalWrite(Pin3, LOW);
          digitalWrite(Pin4, LOW);
          break;
      }
      if (dir) {
        _step++;
      } else {
        _step--;
      }
      if (_step > 7) {
        _step = 0;
      }
      if (_step < 0) {
        _step = 7;
      }
      count++;
      delay(2);

    }
  dir = !dir;
  count = 0;
  } else if (laSunset == current) {
     while (count < 4096) {
      switch (_step) {
        case 0:
          digitalWrite(Pin1, LOW);
          digitalWrite(Pin2, LOW);
          digitalWrite(Pin3, LOW);
          digitalWrite(Pin4, HIGH);
          break;
        case 1:
          digitalWrite(Pin1, LOW);
          digitalWrite(Pin2, LOW);
          digitalWrite(Pin3, HIGH);
          digitalWrite(Pin4, HIGH);
          break;
        case 2:
          digitalWrite(Pin1, LOW);
          digitalWrite(Pin2, LOW);
          digitalWrite(Pin3, HIGH);
          digitalWrite(Pin4, LOW);
          break;
        case 3:
          digitalWrite(Pin1, LOW);
          digitalWrite(Pin2, HIGH);
          digitalWrite(Pin3, HIGH);
          digitalWrite(Pin4, LOW);
          break;
        case 4:
          digitalWrite(Pin1, LOW);
          digitalWrite(Pin2, HIGH);
          digitalWrite(Pin3, LOW);
          digitalWrite(Pin4, LOW);
          break;
        case 5:
          digitalWrite(Pin1, HIGH);
          digitalWrite(Pin2, HIGH);
          digitalWrite(Pin3, LOW);
          digitalWrite(Pin4, LOW);
          break;
        case 6:
          digitalWrite(Pin1, HIGH);
          digitalWrite(Pin2, LOW);
          digitalWrite(Pin3, LOW);
          digitalWrite(Pin4, LOW);
          break;
        case 7:
          digitalWrite(Pin1, HIGH);
          digitalWrite(Pin2, LOW);
          digitalWrite(Pin3, LOW);
          digitalWrite(Pin4, HIGH);
          break;
        default:
          digitalWrite(Pin1, LOW);
          digitalWrite(Pin2, LOW);
          digitalWrite(Pin3, LOW);
          digitalWrite(Pin4, LOW);
          break;
      }
      if (dir) {
        _step++;
      } else {
        _step--;
      }
      if (_step > 7) {
        _step = 0;
      }
      if (_step < 0) {
        _step = 7;
      }
      count++;
      delay(2);

    }
  dir = !dir;
  count = 0;
  times();
  }
  digitalWrite(Pin1, LOW);        
  digitalWrite(Pin2, LOW);
  digitalWrite(Pin3, LOW);
  digitalWrite(Pin4, LOW);

}



