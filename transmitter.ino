#include <Wire.h> // Built-in library for serial communication
#include <WiFiConfig.h>
#include <Adafruit_AHTX0.h> // Adafruit AHTX0 library

#define PHT_RES 4
#define I2C_SDA 6
#define I2C_SCL 7

uint8_t RECEIVER_ADDRESS[] = {0x34, 0xb7, 0xda, 0xf6, 0x3e, 0x9c}; // "0x" specifies hexadecimal (Base 16)

Adafruit_AHTX0 aht;
WeatherReport report;

void setup() {
    // put your setup code here, to run once:
    Serial.begin(115200);
    Wire.begin(I2C_SDA, I2C_SCL);
    aht.begin(); // Finds the AHT sensor via the SDA and SCL lines

    pinMode(PHT_RES, INPUT);

    wifi_setup(); // Enter WiFi mode and start ESP-NOW protocol
    peer_setup(RECEIVER_ADDRESS); // Configure receiving address
}

void loop() {
    // put your main code here, to run repeatedly:
    sensors_event_t humidity, temperature;

    aht.getEvent(&humidity, &temperature); // Populate `temperature` and `humidity` objects with new data

    report.temperature = temperature.temperature;
    report.humidity = humidity.relative_humidity;
    if (analogRead(PHT_RES) > 0) {
        report.light = true;
    } else { report.light = false; }

    Serial.printf("Temperature:\t%i℃\n", report.temperature);
    Serial.printf("Humidity:\t%irH\n", report.humidity);
    Serial.printf("Light:\t\t%i\n\n", report.light); // No format specifier for `bool`

    send_report(RECEIVER_ADDRESS, report);

    delay(10000);
}