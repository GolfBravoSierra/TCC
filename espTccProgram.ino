#include <Stepper.h>
#include <time.h>
#include "Wifi.h"
#include <Keypad.h>

#define IN1 19
#define IN2 18
#define IN3 5
#define IN4 17

const int stepsPerRevolution = 2048;

const byte lines = 4;
const bytes columns = 4;

char keys[lines][columns] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

byte linePins[lines] = {13, 12, 14, 27};
byte colunmsPins[columns] = {26, 25, 33, 32};
Keypad keyBoard = Keypad(makeKeymap(keys), linePins, columnsPins, lines, columns);
String keyboardInput = "";

const char* ssid = "";
const char* password = "";
const char* ntpServer = "pool.ntp.org";
const long  brasiliaTime = -3 * 3600;
const int   summerTimeOffset = 0;

const float storageCarouselSlots[21] = {
  0.0000,   17.1429,  34.2857,  51.4286,  68.5714,
  85.7143,  102.8571, 120.0000, 137.1429, 154.2857,
  171.4286, 188.5714, 205.7143, 222.8571, 240.0000,
  257.1429, 274.2857, 291.4286, 308.5714, 325.7143,
  342.8571
};

void setup() {
  Serial.begin(115200);

  WiFi.begin(ssid, password);
  Serial.print("connecting to wifi");
  while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
  }
  Serial.println("\nWifi -> OK");
  configTime(brasiliaTime, summerTimeOffset, ntpServer);


}

void loop() {
  // put your main code here, to run repeatedly:

}
