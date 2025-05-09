#include <WiFiConfig.h>
#include <LiquidCrystal_I2C.h> // LiquidCrystal I2C library

#define ADDRESS 0x27
#define COLUMNS 16
#define ROWS 2

WeatherReport report;
LiquidCrystal_I2C lcd(ADDRESS, COLUMNS, ROWS); // Instantiate LCD object with default address 0x27

void setup() {
    // put your setup code here, to run once:
    Serial.begin(115200);
    lcd.init();
    lcd.backlight();

    wifi_setup(); // Enter WiFi mode and start ESP-NOW protocol
    set_report_receiver(&report); // Configure receiver to update `report` upon reception
}

void loop() {
    // put your main code here, to run repeatedly:
    Serial.printf("Temperature:\t%i℃\n", report.temperature);
    Serial.printf("Humidity:\t%iRH\n", report.humidity);
    Serial.printf("Light:\t\t%i\n\n", report.light);

    lcd.clear();

    // First row
    lcd.setCursor(0, 0); // (x, y)
    lcd.print("Temp: "); lcd.print(report.temperature); lcd.print("C, ");        // Same
    if (report.light == true) { lcd.print("Day"); } else { lcd.print("Night"); } // line

    // Second row
    lcd.setCursor(0, 1); // (x, y)
    lcd.print("Hum:  "); lcd.print(report.humidity); lcd.print("% RH");

    delay(10000);
}