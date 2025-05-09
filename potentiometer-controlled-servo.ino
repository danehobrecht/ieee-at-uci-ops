#include <ESP32Servo.h>

// Pin assignments
const int POT = 4;
const int SERVO = 10;

// Servo declarations
Servo servo;

// Other constants
const int STD_ESP32_BAUD_RATE = 115200;

void setup() {
    // put your setup code here, to run once:
    Serial.begin(STD_ESP32_BAUD_RATE); // Set baud rate to facilitate healthy Serial communication

    pinMode(POT, INPUT); // Set POT pin to INPUT
    pinMode(SERVO, OUTPUT); // Set SERVO pin to OUTPUT

    servo.attach(SERVO);
    servo.write(0);
}

void loop() {
    // put your main code here, to run repeatedly:
    int POT_VAL = analogRead(POT);

    int POT_VAL_MAP = map(POT_VAL, 0, 4095, 0, 180); // Servo accepts value range 0-180 (181 values)

    Serial.printf("POT_VAL, POT_VAL_MAP: %i, %i\n", POT_VAL, POT_VAL_MAP);

    servo.write(POT_VAL_MAP);
}