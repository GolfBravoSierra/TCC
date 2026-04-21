#include <Stepper.h>
#include <time.h>
#include <WiFi.h>

#define IN1 19
#define IN2 18
#define IN3 5
#define IN4 17

const int stepsPerRevolution = 2048;
Stepper myStepper(stepsPerRevolution, IN1, IN3, IN2, IN4);

//const byte lines = 4;
//const byte columns = 4;
//
//char keys[lines][columns] = {
//  {'1','2','3','A'},
//  {'4','5','6','B'},
//  {'7','8','9','C'},
//  {'*','0','#','D'}
//};
//
//byte linePins[lines] = {13, 12, 14, 27};
//byte columnsPins[columns] = {26, 25, 33, 32};
//Keypad keyBoard = Keypad(makeKeymap(keys), linePins, columnsPins, lines, columns);
//String keyboardInput = "";

const char* ssid = "SANTOS"; //Pixel7Giovani
const char* password = "santos374"; //naotemsenha
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

struct medicineTimeAndSlot {
  int weekDay;
  int hour;
  int minutes; 
  int slot;
};

medicineTimeAndSlot medsProgramCalender[21];
int medsProgramTotalCount = 0;

bool isNotSunday(){
  struct tm currentTime;
  if (!getLocalTime(&currentTime)) {
    return false; 
  } 

  if (currentTime.tm_wday != 0) {
    return true; 
  } else {
    return false;
  }
  
}

bool haveProgramToday(){
  struct tm currentTime;
  if (!getLocalTime(&currentTime)) {
    return false; 
  } 

  for(int i=0; i < 21 ;i++){
    if(currentTime.tm_wday == medsProgramCalender[i].weekDay){
      return true;
    }
  }
  return false;
}

bool isTimeForMeds(){
  struct tm currentTime;
  if (!getLocalTime(&currentTime)) {
    return 0; 
  } 

  Serial.println("vendo se e hora de remedio");
  for(int i=0; i < 21; i++){
    if(currentTime.tm_hour == medsProgramCalender[i].hour){
      return true;
      Serial.println("sim he hora do remedio");
    }
  }
  return false;
}

void makeNewProgram(){
  Serial.println("Digite qual compartimento deseja programar:(1 a 21)\n");
  while (Serial.available() == 0){ delay(10); }
  int selectedSlot = Serial.parseInt();
  while (Serial.available() > 0) {
  Serial.read();
  }

  Serial.println("digite a hora do remedio:\n");
  while (Serial.available() == 0){ delay(10); }
  medsProgramCalender[selectedSlot].hour = Serial.parseInt();
  while (Serial.available() > 0) {
  Serial.read();
  }

  Serial.printf("digite os minutos:\n");
  while (Serial.available() == 0){ delay(10); }
  medsProgramCalender[selectedSlot].minutes = Serial.parseInt();
  while (Serial.available() > 0) {
  Serial.read();
  }

  Serial.println("digite o dia do remedio:\n");
  while (Serial.available() == 0){ delay(10); }
  medsProgramCalender[selectedSlot].weekDay = Serial.parseInt();
  while (Serial.available() > 0) {
    Serial.read();
  }

  Serial.printf("programacao salva: Slot %02d, Horario %02d:%02d, Dia %02d.\n", selectedSlot, medsProgramCalender[selectedSlot].hour, medsProgramCalender[selectedSlot].minutes, medsProgramCalender[selectedSlot].weekDay);

}

void sleepyDayMode(){

  if (haveProgramToday() == true) {
    return; 
  }

  Serial.println("\n--- MODO SLEEPY DAY ATIVADO ---");
  Serial.println("Nenhum remedio para hoje. Aguardando a meia-noite...");

  while (true) {
    
    // Atualiza o relógio
    if (!getLocalTime(&currentTime)) {
      delay(1000);
      return 0 
    }

    if (currentTime.tm_hour == 0 && currentTime.tm_min == 0) {
      Serial.println("\nMeia-noite! Acordando para um novo dia...");
      break;
    }

    if (currentTime.tm_min == 0 && currentTime.tm_sec < 5) {
      Serial.printf("Zzz... Sleepy Mode... %02d:%02d\n", currentTime.tm_hour, currentTime.tm_min);
    }

    delay(30000);
  }
}

int findTodayPrograms(int* foudedProgramsFoToday){
   
  struct tm currentTime;
  if (!getLocalTime(&currentTime)) {
    delay(1000);
    return 0
  }

  int foundedProgramsForToday[3]; 
  int counter = 0;

  for(int programNumber = 0; programNumber < 21; programNumber++){
    if(currentTime.tm_wday == medsProgramCalender[programNumber].weekDay){
      foundedProgramsForToday[counter] = programNumber;
      counter ++;
    }
  }

  return couter;
}

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

  
  myStepper.setSpeed(5);

  makeNewProgram();

}


void loop() {
  struct tm currentTime;
  if(!getLocalTime(&currentTime)){
    return;
  }

  sleepyDayMode();

  Serial.printf("%02d:%02d:%02d\n", currentTime.tm_hour, currentTime.tm_min, currentTime.tm_sec);
  
  if(isNotSunday() && haveProgramToday()){
    int todayPrograms[3];
    int programQuantityForToday = findTodayPrograms(todayPrograms);

    if(isTimeForMeds()){
      Serial.println("it is time for meds");
      delay(10000);
    }
  }

  delay(10000);
}
