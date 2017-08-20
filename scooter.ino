#include <Servo.h>

Servo myservo;

int acc = 0,
    old_acc = 0,
    brake = 0,
    throttle = 0,
    i = 0,
    blink = 0;

void setup() {
    Serial.begin(250000);           // DIVISIBLE BY 1MIL
    pinMode(3, INPUT_PULLUP);       // BRAKE-REVERSE ON DIGITAL PIN 3
    pinMode(LED_BUILTIN, OUTPUT);   // ONBOARD LED TO SHOW STATUS
    delay(500);                     // WAIT FOR SERVO TO BOOT
    myservo.attach(9);              // ATTACH SERVO ON DIGITAL PIN 9
    myservo.write(90);              // SET "ZERO" (MIDDLE BETWEEN FORWARD AND BACKWARD DRIVE)
    delay(500);                     // WAIT FOR SERVO TO LOAD "ZERO"
}

void loop() {
    blink = 1 - blink;              // TOGGLE ONBOARD LED
    digitalWrite(LED_BUILTIN, (brake == 0) ? 1 : blink);
    
    acc = 0;      // RESET ACCELERATION
    brake = 0;    // RESET BRAKE
    
    for (i = 0; i <16; i++) {   // ACCUMULATE ANALOG ACCELERATION VALUES
      brake += digitalRead(3);  // READ BRAKE STATUS FROM DIGITAL PIN 3
      acc += analogRead(A0);    // READ ACCELERATION FROM ANALOG PIN 0
      delay(1);                 // 16MS IN TOTAL
    }
    
    acc = acc >> 4;             // SMOOTHEN ANALOG ACCELERATION VALUES (16X / 4 BIT OVERSAMPLING)
  
    if(brake > 15) {            // FORWARD DRIVE
        old_acc = (old_acc + acc) >> 1; // AVERAGE CURRENT ACCELERATION WITH PREVIOUS ACCELERATION
        throttle = old_acc * .17 + 62;  // SPEED FORWARD
    } else {                            // REVERSE DRIVE
        old_acc = 0;                    // RESET COAST SPEED
        throttle = 105 - (acc >> 3);    // SPEED IN REVERSE
    }
    
    Serial.println(String(throttle, DEC));    // SHOW THROTTLE IN DEBUG
  
    myservo.write(throttle);                  // SEND 
}
