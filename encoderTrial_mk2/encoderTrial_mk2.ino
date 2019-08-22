#define InA1            11                      // INA motor pin
#define InB1            16                      // INB motor pin
#define PWM1            6                       // PWM motor pin
#define encodPinA1      30                       // encoder A pin
#define encodPinB1      27                       // encoder B pin

#define LOOPTIME        100                     // PID loop time
#define FORWARD         1                       // direction of rotation
#define BACKWARD        2                       // direction of rotation

unsigned long lastMilli = 0;                    // loop timing
unsigned long lastMilliPrint = 0;               // loop timing
long count = 0;                                 // rotation counter
long countInit;
long tickNumber = 0;
boolean run = false;                                     // motor moves

void setup() {
 pinMode(InA1, OUTPUT);
 pinMode(InB1, OUTPUT);
 pinMode(PWM1, OUTPUT);
 pinMode(encodPinA1, INPUT);
 pinMode(encodPinB1, INPUT);
 digitalWrite(encodPinA1, HIGH);                      // turn on pullup resistor
 digitalWrite(encodPinB1, HIGH);
 attachInterrupt(1, rencoder, FALLING);
}

void loop() {
 moveMotor(FORWARD, 50, 1200*2);                        // direction, PWM, ticks number
 delay(5000);
 moveMotor(BACKWARD, 100, 2400*2);                           // 464=360Â°
 delay(3000);
}

void moveMotor(int direction, int PWM_val, long tick)  {
 countInit = count;    // abs(count)
 tickNumber = tick;
 if(direction==FORWARD)          motorForward(PWM_val);
 else if(direction==BACKWARD)    motorBackward(PWM_val);
}

void rencoder()  {                                    // pulse and direction, direct port reading to save cycles
 if (encodPinB1 & 0b00000001)    count++;                  // if(digitalRead(encodPinB1)==HIGH)   count_r ++;
 else                      count--;                  // if (digitalRead(encodPinB1)==LOW)   count_r --;
 if(run)  
   if((abs(abs(count)-abs(countInit))) >= tickNumber)      motorBrake();
}

void motorForward(int PWM_val)  {
 analogWrite(PWM1, PWM_val);
 digitalWrite(InA1, LOW);
 digitalWrite(InB1, HIGH);
 run = true;
}

void motorBackward(int PWM_val)  {
 analogWrite(PWM1, PWM_val);
 digitalWrite(InA1, HIGH);
 digitalWrite(InB1, LOW);
 run = true;
}

void motorBrake()  {
 analogWrite(PWM1, 0);
 digitalWrite(InA1, HIGH);
 digitalWrite(InB1, HIGH);
 run = false;
}
