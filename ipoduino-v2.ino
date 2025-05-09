#include <HardwareSerial.h>
#include <DFRobotDFPlayerMini.h>

#define BTN_POP 3
#define BTN_NXT 4
#define PIN_RX 20
#define PIN_TX 21

HardwareSerial UART(1); // Use "UART1"
DFRobotDFPlayerMini player; // DFPlayer object

void setup() {
	// put your setup code here, to run once:
	Serial.begin(115200);

	// Use internal pull-ups for button functionality
	pinMode(BTN_POP, INPUT_PULLUP);
	pinMode(BTN_NXT, INPUT_PULLUP);

	UART.begin(9600, SERIAL_8N1, PIN_RX, PIN_TX); // Initialize the pins for HardwareSerial (No `pinMode` calls needed)

	Serial.begin(115200);

	// Use internal pull-ups for button functionality
	pinMode(BTN_POP, INPUT_PULLUP);
	pinMode(BTN_NXT, INPUT_PULLUP);

	UART.begin(9600, SERIAL_8N1, PIN_RX, PIN_TX); // Initialize the pins for HardwareSerial (No `pinMode` calls needed)
	delay(1000); // Necessary delay to properly initialize `HardwareSerial`

	if (!player.begin(UART)) { Serial.println("Connecting to DFPlayer Mini failed!"); } // Start communication with DFPlayer Mini

	player.volume(24); // Set initial player volume to 80% (0 to 30)

	player.play(1); // Play the first file in "mp3" folder on microSD card
	Serial.printf("Playing index %i\n", player.readCurrentFileNumber());
}

void loop() {
	// put your main code here, to run repeatedly:
	int BTN_POP_VAL = digitalRead(BTN_POP); // Read "Play/Pause" button value
	int BTN_NXT_VAL = digitalRead(BTN_NXT); // Read "Next" button value

	// "Play/Pause" button handler
	if (BTN_POP_VAL == 0) {
		if (player.readState() == 513) { // Check for "Play" state (Code 513)
			player.pause();
			Serial.printf("Paused index %i\n", player.readCurrentFileNumber());
		} else if (player.readState() == 514) { // Check for "Pause" state (Code 514)
			player.start(); // Resume playback from paused position
			Serial.printf("Playing index %i\n", player.readCurrentFileNumber());
		}
		delay(160); // Debounce
	}

	// "Next" button handler
	if (BTN_NXT_VAL == 0) {
		player.next();
		int index = player.readCurrentFileNumber();
		Serial.printf("Playing index %i\n", index);

		// Rickroll at maximum volume
		if (index == 3) {
			player.volume(30); 
		} else { player.volume(24); }

		delay(160); // Debounce
	}
}