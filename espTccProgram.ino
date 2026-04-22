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

bool isSunday(){
  struct tm currentTime;
  if (!getLocalTime(&currentTime)) {
    return false; 
  } 

  if (currentTime.tm_wday == 0) {
    return true; 
  } else {
    return false;
  }
  
}

bool HaveProgramsForToday(){
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

  int theMedForThisHour;

  if (currentTime.tm_min % 15 == 0){
    for(int i = 0; i < 21; i++){
      if(currentTime.tm_hour == medsProgramCalender[i].hour && currentTime.tm_min == medsProgramCalender[i].minutes){
        return true;
      }
    }
  }

}

void makeNewProgram(){

  Serial.println("Digite qual compartimento deseja programar:(1 a 21)\n");
  while (Serial.available() == 0){ delay(10); }
  medsProgramCalender[medsProgramTotalCount].slot = Serial.parseInt();
  while (Serial.available() > 0) {
    Serial.read();
  }

  Serial.println("digite a hora do remedio:\n");
  while (Serial.available() == 0){ delay(10); }
  medsProgramCalender[medsProgramTotalCount].hour = Serial.parseInt();
  while (Serial.available() > 0) {
  Serial.read();
  }
  
  Serial.printf("digite os minutos:\n");
  while (Serial.available() == 0){ delay(10); }
  medsProgramCalender[medsProgramTotalCount].minutes = Serial.parseInt();
  while (Serial.available() > 0) {
  Serial.read();
  }

  Serial.println("digite o dia do remedio:\n");
  while (Serial.available() == 0){ delay(10); }
  medsProgramCalender[medsProgramTotalCount].weekDay = Serial.parseInt();
  while (Serial.available() > 0) {
    Serial.read();
  }

  medsProgramTotalCount ++;
  Serial.printf("programacao salva: Slot %02d, Horario %02d:%02d, Dia %02d.\n", medsProgramTotalCount, medsProgramCalender[medsProgramTotalCount].hour, medsProgramCalender[medsProgramTotalCount].minutes, medsProgramCalender[medsProgramTotalCount].weekDay);

}


int findTodayPrograms(int* foudedProgramsFoToday){
   
  struct tm currentTime;
  if (!getLocalTime(&currentTime)) {
    delay(1000);
    return 0;
  }

  int foundedProgramsForToday[3]; 
  int counter = 0;

  for(int programNumber = 0; programNumber < 21; programNumber++){
    if(currentTime.tm_wday == medsProgramCalender[programNumber].weekDay){
      foundedProgramsForToday[counter] = programNumber;
      counter ++;
    }
  }

  return counter;
}


void setup() {
  Serial.begin(115200);
  
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);
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

const char* ssid = "******"; 
const char* password = "*****";
const char* ntpServer = "pool.ntp.org";
const long  brasiliaTime = -3 * 3600;
const int   summerTimeOffset = 0;

void loop() {
  struct tm currentTime;
  if(!getLocalTime(&currentTime)){
    return;
  }

  Serial.printf("%02d:%02d:%02d\n", currentTime.tm_hour, currentTime.tm_min, currentTime.tm_sec);

  if(HaveProgramsForToday){

    if(isSunday){

    }else {
      // aqui vai ter uma parte de notificação 
    }
    
    int todayPrograms[3];
    int programQuantityForToday = findTodayPrograms(todayPrograms);
    
    Serial.printf("Os remadios para hoje sao:\n");
    for(int i = 0; i < 3; i++){
      Serial.printf(" Slot %02d;\n Horario %02d:%02d;\n Dia %02d.\n", medsProgramCalender[todayPrograms[i]].slot, medsProgramCalender[todayPrograms[i]].hour, medsProgramCalender[todayPrograms[i]].minutes, medsProgramCalender[todayPrograms[i]].weekDay);
    }

    isTimeForMeds();

  }else{
    delay(10000);
    // alguma coisa pra ele dormir aumentar o delay do loop para ele não ficar atualizando de 15 em 15 min
  }


}
