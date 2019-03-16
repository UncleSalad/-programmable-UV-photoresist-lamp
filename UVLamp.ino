//Libraries//
	#include <Arduino-LiquidCrystal-I2C-library-master/LiquidCrystal_I2C.h>
	#include <Wire.h>
	#include <EEPROM.h>
	#include <EEWrap-master/EEWrap.h>
//Variables//
	//Encoder//
		//Pins//
			#define clk 2
			#define dt 3
			#define sw 4
		//Flags and timers//
			long currentTime, loopTime;
			boolean encoder_A, encoder_B, encoder_sw, encoder_A_prev;
			boolean encoder_sw_flag;
	//Relay/MOSFET//
		#define lamp 13
	//Service variables//
		uint16_e preset[3];
		unsigned int presetRun[3] = { 0, 0, 0 };
		char* presetName[3] = {
			"Preset1: ",
			"Preset2: ",
			"Preset3: "
		};
		#define presetLimit 360 
		byte choice = 0;
		byte choicePreset = 1;
		unsigned int stepDelay;
		boolean asssignentFlag;
		boolean run = 0;
//Initialization
	LiquidCrystal_I2C lsd(0x3f, 16, 2);
void setup() {
	//PinModes//
		pinMode(clk, INPUT_PULLUP);
		pinMode(dt, INPUT_PULLUP);
		pinMode(sw, INPUT_PULLUP);
		pinMode(lamp, OUTPUT);
	//Begins//
		lsd.begin();
	//Others//
		currentTime = millis();
		loopTime = currentTime;
		//for (int i = 1; i <= 3; i++) preset[i] = 0;
}
void loop() {
	//Workout encoder//
		currentTime = millis();
		if (currentTime >= (loopTime + 5)) {
			encoder_A = !digitalRead(clk);
			encoder_B = !digitalRead(dt);
			if ((!encoder_A) && (encoder_A_prev)) {
				if (encoder_B) {
					if (!run) {
						switch (choice) {
						case 0: if (choicePreset < 3) choicePreset++;
							break;
						case 1:
							switch (choicePreset) {
							case 1: if (preset[1] < presetLimit) preset[1]++;
								break;
							case 2: if (preset[2] < presetLimit) preset[2]++;
								break;
							case 3: if (preset[3] < presetLimit) preset[3]++;
								break;
							}
							break;
						case 2:
							run = true;
						}
					}
				}
				else {
					if (!run) {
						switch (choice) {
						case 0: if (choicePreset > 1) choicePreset--;
							break;
						case 1:
							switch (choicePreset) {
							case 1: if (preset[1] > 0) preset[1]--;
								break;
							case 2: if (preset[2] > 0) preset[2]--;
								break;
							case 3: if (preset[3] > 0) preset[3]--;
								break;
							}
							break;
						}
					}
					if (run) run = false;
				}
			}
			encoder_A_prev = encoder_A;
			loopTime = currentTime;
		}
		if (!run) {
			encoder_sw = !digitalRead(sw);
			if (encoder_sw && !encoder_sw_flag) {
				encoder_sw_flag = 1;
				if (choice != 2) choice++;
				else choice = 0;
			}
			if (!encoder_sw && encoder_sw_flag) {
				encoder_sw_flag = 0;
			}
		}
	//Workout LCD//
		if (!run) {
			switch (choice) {
			case 0:
				switch (choicePreset) {
				case 1: printChoicePreset(0, preset[1]);
					break;
				case 2: printChoicePreset(1, preset[2]);
					break;
				case 3: printChoicePreset(2, preset[3]);
					break;
				}
				break;
			case 1:
				switch (choicePreset) {
				case 1: printChoiceTime(0, preset[1]);
					break;
				case 2: printChoiceTime(1, preset[2]);
					break;
				case 3: printChoiceTime(2, preset[3]);
					break;
				}
				break;
			case 2:
				switch (choicePreset) {
				case 1: printDisplaySet(0, preset[1]);
					break;
				case 2: printDisplaySet(1, preset[2]);
					break;
				case 3: printDisplaySet(2, preset[3]);
					break;
				}
			}
		}
		else {
			switch (choicePreset) {
			case 1: printDisplayRunning(0, presetRun[1]);
				break;
			case 2: printDisplayRunning(1, presetRun[2]);
				break;
			case 3: printDisplayRunning(2, presetRun[3]);
				break;
			}
		}
	//Operation with variables//
		if (run) {
			if (asssignentFlag)	for (int i = 1; i <= 3; i++) presetRun[i] = preset[i];
			asssignentFlag = false;
			if (millis() - stepDelay >= 1000) {
				switch (choicePreset) {
				case 1: if (presetRun[1] <= 0) run = false;
					presetRun[1]--;
					break;
				case 2: if (presetRun[2] <= 0) run = false;
					presetRun[2]--;
					break; 
				case 3: if (presetRun[3] <= 0) run = false;
					presetRun[3]--;
					break;
				}
				stepDelay = millis();
			}
		}
		else asssignentFlag = true;
	//Workout UV lamp//
		if (run) digitalWrite(lamp, HIGH);
		else digitalWrite(lamp, LOW);
}
void printChoicePreset (int cn, int cp) {
	lsd.setCursor(0, 0);
	lsd.print(">");
	lsd.print(presetName[cn]);
	lsd.print(cp);
	lsd.print(" s   ");
	lsd.setCursor(2, 1);
	lsd.print("  run  ");
}
void printChoiceTime(int cn, int cp) {
	lsd.setCursor(0, 0);
	lsd.print(" ");
	lsd.print(presetName[cn]);
	lsd.print(">");
	lsd.print(cp);
	lsd.print(" s<    ");
	lsd.setCursor(2, 1);
	lsd.print("  run  ");
}
void printDisplaySet(int cn, int cp) {
	lsd.setCursor(0, 0);
	lsd.print(" ");
	lsd.print(presetName[cn]);
	lsd.print(cp);
	lsd.print(" s    ");
	lsd.setCursor(2, 1);
	lsd.print(">>run>> ");
}
void printDisplayRunning(int cn, int cp) {
	lsd.setCursor(0, 0);
	lsd.print(" ");
	lsd.print(presetName[cn]);
	lsd.print(cp);
	lsd.print(" s    ");
	lsd.setCursor(2, 1);
	lsd.print("<<stop<<");
}