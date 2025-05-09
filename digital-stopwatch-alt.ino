// See https://espressif-docs.readthedocs-hosted.com/projects/arduino-esp32/en/latest/api/timer.html

#include <TM1637Display.h>

#define CLK     4
#define DIO     6
#define BTN_1   8
#define BTN_2   10
#define PZO     21

int mins = 0;
hw_timer_t *timer;
int timerState = 0; // 0 = STOPPED, 1 = RUNNING, 2 = PAUSED
TM1637Display display(CLK, DIO); // Uses I2C (SCLK, DIO [Data In/Out {Data Line (SDA)}])

// "Start/Pause/Resume" button
void BTN_1_ISR() {
    static unsigned long last_interrupt_time = 0;
    unsigned long interrupt_time = millis();
    // Debounce (200ms)
    if (interrupt_time - last_interrupt_time > 200)  {
        Serial.println("Button 1 pressed!");
        tone(PZO, 523, 1000); // Play C5 (523 Hz) on Piezzo Buzzer
        if (timerState == 0) { // IF STOPPED
            mins = 0; // Reset minutes
            timer = timerBegin(1000000); // Set timer frequency to 1 Mhz
            timerAttachInterrupt(timer, &MIN_BZR_ISR); // Attach interrupt
            timerAlarm(timer, 60000000, true, 0); // 60000000 microsecsonds = 1 minute
            timerState = 1; // Update timer state to RUNNING
            Serial.println("Timer started!");
        } else if (timerState == 2) { // IF PAUSED
            timerStart(timer);
            timerState = 1; // Update timer state to RUNNING
            Serial.println("Timer resumed!");
        } else if (timerState == 1) { // IF RUNNING
            timerStop(timer);
            timerState = 2; // Update timer state to PAUSED
            Serial.println("Timer paused!");
        }
    }
    last_interrupt_time = interrupt_time;
}

// "Stop" button
void BTN_2_ISR() {
    static unsigned long last_interrupt_time = 0;
    unsigned long interrupt_time = millis();
    // Debounce (200ms)
    if (interrupt_time - last_interrupt_time > 200)  {
        Serial.println("Button 2 pressed!");
        tone(PZO, 587, 1000); // Play D5 (587 Hz) on Piezzo Buzzer
        timerStop(timer);
        timerState = 0; // Update timer state to STOPPED
    }
    last_interrupt_time = interrupt_time;
}

// Minute buzzer
void MIN_BZR_ISR() {
    Serial.println("Minute elapsed!");
    tone(PZO, 659, 1000); // Play E5 (659 Hz) on Piezzo Buzzer
    mins++; // Increment minutes place on clock
    if (mins > 99) { // Maximum display value exceeded
        Serial.println("Maximum display value exceeded!");
        tone(PZO, 698, 1000); // Play F5 (698 Hz) on Piezzo Buzzer
        timerEnd(timer);
        timerState = 0; // Update timer state to STOPPED
    }
}

void setup() {
    // put your setup code here, to run once:
    Serial.begin(115200);

    // Pin configuration:
    pinMode(BTN_1, INPUT_PULLUP);
    pinMode(BTN_2, INPUT_PULLUP);
    pinMode(PZO, OUTPUT);

    // Assign interrupts to GPIO pins 8 and 10:
    attachInterrupt(digitalPinToInterrupt(BTN_1), BTN_1_ISR, FALLING);
    attachInterrupt(digitalPinToInterrupt(BTN_2), BTN_2_ISR, FALLING);

    // Configure 7-segment display:
    display.clear();
    display.setBrightness(7);
}

void loop() {
    // put your main code here, to run repeatedly:
    int mils = timerReadMillis(timer);
    int secs = mils % 60000 / 1000;
    int output = (mins * 100) + secs; // For example: (5 * 100) + 31 = 531

    Serial.printf("Milliseconds:\t%i\n", mils);
    Serial.printf("Minutes:\t%i\n", mins);
    Serial.printf("Seconds:\t%i\n", secs);
    Serial.printf("Output:\t\t%i\n\n", output);

    display.showNumberDecEx(output, 0x40, true);
    // Attribute behaviors:
    // - `output` is integer to be displayed. For example: "531" = "5:31"
    // - `0x40` enables colon in middle of display
    // - `true` prepends a 0 for padding. For example: "5:31" -> "05:31"

    delay(207); // Q: Why `207`?
}