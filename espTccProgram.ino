  #include <Stepper.h>
  #include <time.h>
  #include <WiFi.h>
  #include <WiFiManager.h>
  #include <ESP_Mail_Client.h>  

  #include <ESP_Mail_Client.h>
  #define SMTP_HOST "smtp.gmail.com"
  #define SMTP_PORT 465
  #define EMAIL_REMETENTE "giromedalerta@gmail.com"
  #define SENHA_APP "aohq whpv sxot yvyx " 
  #define EMAIL_CUIDADOR "giovanibelliniestudos@gmail.com"
  SMTPSession smtp;

  void sendEmail(String assunto, String corpoMensagem) {
    
    Serial.println("Conectando ao servidor do Google...");

    ESP_Mail_Session session;
    session.server.host_name = SMTP_HOST;
    session.server.port = SMTP_PORT;
    session.login.email = EMAIL_REMETENTE;
    session.login.password = SENHA_APP;
    session.login.user_domain = "";


    SMTP_Message message;
    message.sender.name = "Máquina GiroMed";
    message.sender.email = EMAIL_REMETENTE;
    
    message.addRecipient("Cuidador Responsável", EMAIL_CUIDADOR);
    
    message.subject = assunto;
    message.text.content = corpoMensagem;

    if (!smtp.connect(&session)) {
      Serial.println("Erro de conexão SMTP!");
      return;
    }

    if (!MailClient.sendMail(&smtp, &message)) {
      Serial.print("Erro ao enviar o e-mail: "); 
      Serial.println(smtp.errorReason());
    } else {
      Serial.println(">>> E-MAIL ENVIADO COM SUCESSO! <<<");
    }

    smtp.sendingResult.clear();
  }

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

  struct tm currentTime;

  #include <ESPmDNS.h>
  #include <WebServer.h>
  WebServer server(80);
  #include "web_page.h"

  void handleSavingPrograms() {

    if (server.hasArg("slot") && server.hasArg("hora") && server.hasArg("minuto") && server.hasArg("dia")) {
      

      int newSlot = server.arg("slot").toInt();
      int newHour = server.arg("hora").toInt();
      int newMinute = server.arg("minuto").toInt();
      int newDay = server.arg("dia").toInt();

      medsProgramCalender[medsProgramTotalCount].slot = newSlot;
      medsProgramCalender[medsProgramTotalCount].hour = newHour;
      medsProgramCalender[medsProgramTotalCount].minutes = newMinute;
      medsProgramCalender[medsProgramTotalCount].weekDay = newDay;
      
      medsProgramTotalCount++;

      Serial.printf("\n>>> NOVO REMEDIO RECEBIDO VIA WIFI <<<\n");
      Serial.printf("Gaveta %d para as %02d:%02d no dia %d\n", newSlot, newHour, newMinute, newDay);

      server.send(200, "text/html", sucesso_html);
      
    } else {
      server.send(400, "text/plain", "Erro: Dados incompletos");
    }
  }

  void handleList() {
    String html = String(lista_topo);
    
    const char* daysOfTheWeek[] = {"Dom", "Seg", "Ter", "Qua", "Qui", "Sex", "Sab"};

    if (medsProgramTotalCount == 0) {
      html += "<tr><td colspan='3'>Nenhuma programação encontrada.</td></tr>";
    } else {
      for (int i = 0; i < medsProgramTotalCount; i++) {
        html += "<tr>";
        
        html += "<td>";
        html += medsProgramCalender[i].slot; 
        html += "</td>";
        
        char bufferHora[10];
        sprintf(bufferHora, "%02d:%02d", medsProgramCalender[i].hour, medsProgramCalender[i].minutes);
        html += "<td>";
        html += bufferHora;
        html += "</td>";
        
        html += "<td>";
        html += daysOfTheWeek[medsProgramCalender[i].weekDay]; 
        html += "</td>";
        
        html += "</tr>";
      }
    }

    html += String(lista_fim);
    server.send(200, "text/html", html);
  }

  bool isSunday(){
    
    if (!getLocalTime(&currentTime)) {
      return false; 
    } 
    
    if (currentTime.tm_wday == 0) {
      return true; 
    } else {
      return false;
    }
    
  }

  bool thereAreProgramsForToday(){
    
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

    
    if (!getLocalTime(&currentTime)) {
      return 0; 
    } 

    int theMedForThisHour;
    
    for(int i = 0; i < 21; i++){
      if(currentTime.tm_hour == medsProgramCalender[i].hour && currentTime.tm_min == medsProgramCalender[i].minutes){
        return true;
      }
    }

    return false;
  }


  int findTodaysPrograms(int* foundedProgram){
    
    if (!getLocalTime(&currentTime)) {
      delay(1000);
      return 0;
    }
    
    int counter = 0;
    
    for(int programNumber = 0; programNumber < 21; programNumber++){
      if(currentTime.tm_wday == medsProgramCalender[programNumber].weekDay){
        foundedProgram[counter] = programNumber;
        counter ++;
      }
    }
    
    return counter;
  }

  int findSlotsForThisHour(int* todaysPrograms){
    
    if(!getLocalTime(&currentTime)){
      Serial.printf("erro na hora\n");
    }
    
    for(int i = 0; i < 3; i++){
      if (currentTime.tm_hour == medsProgramCalender[todaysPrograms[i]].hour && currentTime.tm_min == medsProgramCalender[todaysPrograms[i]].minutes   ){
        Serial.printf("\n etá restornando slot ");
        return todaysPrograms[i];
      }
    }
    return -1;
  }


  const int stepsPerRevolution = 2048;
  int currentSlot = 0;
  #define IN1 19
  #define IN2 18
  #define IN3 5
  #define IN4 174

  Stepper myStepper(stepsPerRevolution, IN1, IN3, IN2, IN4);

  void rotateToSlot(int targetSlot){
    
    Serial.printf("\n--- INICIANDO ENTREGA ---\n");
    Serial.printf("Carrossel atual: Gaveta %d | Destino: Gaveta %d\n", currentSlot, targetSlot);

    float currentAngle = storageCarouselSlots[currentSlot];
    float targetAngle = storageCarouselSlots[targetSlot];

    float differenceBetweenCurrentAndTargetAngle = targetAngle - currentAngle;

    long stepsToWalk = (differenceBetweenCurrentAndTargetAngle / 360.0) * stepsPerRevolution;

    Serial.printf("Girando o motor em %ld passos...\n", stepsToWalk);

    myStepper.step(stepsToWalk);

    Serial.println(">>> REMEDIO DISPENSADO COM SUCESSO! <<<");

    currentSlot = targetSlot;

    return;
  }

  const char* ssid = ""; 
  const char* password = "";
  const char* ntpServer = "pool.ntp.org";
  const long  brasiliaTime = -3 * 3600;
  const int   summerTimeOffset = 0;

  void setup() {
    Serial.begin(115200);
    
    WiFiManager wm;

    Serial.printf("waiting for wifi connectioni\n");
    bool connected = wm.autoConnect("giromed", "27042026");

    if (!connected) {
      Serial.println("ERRO ESP32 was unable to connect to the Wi-Fi.\n");
      delay(3000);
      ESP.restart();
    }

    Serial.println("\nWifi -> OK");
    configTime(brasiliaTime, summerTimeOffset, ntpServer);
    
    myStepper.setSpeed(5);
    
  if (MDNS.begin("giromed")) {
    Serial.println("MDNS iniciado com sucesso!");
    Serial.println("Acesse no navegador: http://giromed.local");
  }

    server.on("/", []() {
    server.send(200, "text/html", index_html);});
    server.on("/salvar", handleSavingPrograms); 
    server.on("/lista", handleList);
    server.begin();
    
  }

int lastVerifiedMinute = -1;

void loop() {

  if(!getLocalTime(&currentTime)){
    return;
  }

  Serial.printf("%02d:%02d:%02d\n", currentTime.tm_hour, currentTime.tm_min, currentTime.tm_sec);

  if (currentTime.tm_min != lastVerifiedMinute && thereAreProgramsForToday()) {

    if(thereAreProgramsForToday()){
  
      if(isSunday()){
        // aqui vai ter uma parte de notificação 
      }
      
      int todaysPrograms[3];
      int quantityOfProgramsForToday = findTodaysPrograms(todaysPrograms);
      
      Serial.printf("Os remadios para hoje sao:\n");
      for(int i = 0; i < 3; i++){
        Serial.printf(" Slot %02d;\n Horario %02d:%02d;\n Dia %02d.\n", medsProgramCalender[todaysPrograms[i]].slot, medsProgramCalender[todaysPrograms[i]].hour, medsProgramCalender[todaysPrograms[i]].minutes, medsProgramCalender[todaysPrograms[i]].weekDay);
      }
  
      if (isTimeForMeds()){
        int slotForThisHour = findSlotsForThisHour(todaysPrograms);
        rotateToSlot(slotForThisHour);
      }
  
    }else{
      delay(10000);
      // alguma coisa pra ele dormir aumentar o delay do loop para ele não ficar atualizando de 15 em 15 min
    }

    lastVerifiedMinute = currentTime.tm_min;
  }


}
