#include <Timer.h>
#include <TM1637Display.h>

#define CLK     4
#define DIO     6
#define BTN_1   8
#define BTN_2   10
#define PZO     21

Timer timer;
TM1637Display display(CLK, DIO); // Uses I2C (SCLK, DIO [Data In/Out {Data Line (SDA)}])

void debounce_wrapper(void *func) {
    static unsigned long last_interrupt_time = 0;
    unsigned long interrupt_time = millis();
    int debounce_time = 200; // Milliseconds
    if (interrupt_time - last_interrupt_time > debounce_time) {
        func(); // WRAPPED FUNCTION CALLED HERE
    }
    last_interrupt_time = interrupt_time;
}

// "Start/Pause/Resume" button
void BTN_1_ISR() {
    Serial.println("Button 1 pressed!");
    tone(PZO, 523, 1000); // Play C5 (523 Hz) on Piezzo Buzzer
    if (timer.state() == STOPPED) {
        Serial.println("Timer started!");
        timer.stop(); // Reset clock
        timer.start();
    } else if (timer.state() == PAUSED) {
        timer.resume();
        Serial.println("Timer resumed!");
    } else if (timer.state() == RUNNING) {
        timer.pause();
        Serial.println("Timer paused!");
    }
}

// "Stop" button
void BTN_2_ISR() {
    Serial.println("Button 2 pressed!");
    tone(PZO, 587, 1000); // Play D5 (587 Hz) on Piezzo Buzzer
    timer.stop();
    Serial.println("Timer stopped!");
}

void setup() {
    // put your setup code here, to run once:
    Serial.begin(115200);

    // Pin configuration:
    pinMode(BTN_1, INPUT_PULLUP);
    pinMode(BTN_2, INPUT_PULLUP);
    pinMode(PZO, OUTPUT);

    // Assign interrupts to GPIO pins 8 and 10:
    attachInterrupt(digitalPinToInterrupt(BTN_1), debounce_wrapper(BTN_1_ISR), FALLING);
    attachInterrupt(digitalPinToInterrupt(BTN_2), debounce_wrapper(BTN_2_ISR), FALLING);

    // Configure 7-segment display:
    display.clear();
    display.setBrightness(7);
}

void loop() {
    // put your main code here, to run repeatedly:
    int mils = timer.read();
    int mins = mils / 60000;
    int secs = mils % 60000 / 1000;
    int output = (mins * 100) + secs; // For example: (5 * 100) + 31 = 531

    // Beep every minute
    if ((timer.state() == RUNNING) && (mils >= 240) && (mils % 60000) <= 240) {
        Serial.println("Minute elapsed!");
        tone(PZO, 659, 1000); // Play E5 (659 Hz) on Piezzo Buzzer
    }

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