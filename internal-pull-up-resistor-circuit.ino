const int LED = 4;
const int BTN = 10;

void setup() {
    // put your setup code here, to run once:
    Serial.begin(115200);
    pinMode(LED, OUTPUT);
    pinMode(BTN, INPUT_PULLUP); // Use internal pull-up resistor to flip default from LOW -> HIGH
}

void loop() {
    // put your main code here, to run repeatedly:
    int BTN_VAL = digitalRead(BTN);
    digitalWrite(LED, BTN_VAL);
    Serial.println(BTN_VAL);
}