#include <ESP32Servo.h>

// Pin assignments
const int PHT = 4;
const int SERVO = 10;

// Servo declarations
Servo servo;

// Other constants
const int STD_ESP32_BAUD_RATE = 115200;

void setup() {
    // put your setup code here, to run once:
    Serial.begin(STD_ESP32_BAUD_RATE); // Set baud rate to facilitate healthy Serial communication

    pinMode(PHT, INPUT); // Set PHT pin to INPUT
    pinMode(SERVO, OUTPUT); // Set SERVO pin to OUTPUT

    servo.attach(SERVO);
}

void loop() {
    // put your main code here, to run repeatedly:
    int PHT_VAL = analogRead(PHT);
    int PHT_VAL_MAP = map(PHT_VAL, 0, 4095, 0, 180); // Servo accepts value range 0-180 (181 values)
    int PHT_VAL_INV = (PHT_VAL_MAP - 180) * -1; // Invert to correctly correspond to diagram visuals

    Serial.printf("PHT_VAL, PHT_VAL_MAP, PHT_VAL_INV: %i, %i, %i\n", PHT_VAL, PHT_VAL_MAP, PHT_VAL_INV);

    servo.write(PHT_VAL_INV);
}