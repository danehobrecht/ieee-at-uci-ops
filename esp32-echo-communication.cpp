void setup() {
	Serial.begin(115200);
}

void loop() {
	if (Serial.available() > 0) { // Check if ESP32 UART has recieved any data
		int receivedByte = Serial.read(); // Stores the incoming byte
		Serial.printf("\nreceivedByte: %u\n", receivedByte);
		if (receivedByte >= 0 && receivedByte <= 255) {
			Serial.write(receivedByte);
		}
	}
}