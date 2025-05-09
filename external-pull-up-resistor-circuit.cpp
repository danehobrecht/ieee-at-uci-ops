const int LED = 4;
const int BTN = 10;

void setup() {
    // put your setup code here, to run once:
    Serial.begin(115200);
    pinMode(LED, OUTPUT);
    pinMode(BTN, INPUT);
}

void loop() {
    // put your main code here, to run repeatedly:
    digitalWrite(LED, digitalRead(BTN));
}