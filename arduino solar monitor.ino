#include <EmonLib.h>
#include <SoftwareSerial.h>
#include <Servo.h>
//#define RX 2
//#define TX 3
#define emon_apin A4 // Analog Pin sensor is connected to
int initial_position = 90;   //Declaring the initial position at 90
int LDR1 = A0;          //Pin at which LDR is connected
int LDR2 = A1;          //Pin at which LDR is connected
int LDR3 = A2;
int error = 5;          //initializing variable for error
int servopin=9;
int relay = 4;


String AP = "ICTLAB";       // AP NAME
String PASS = ""; // AP PASSWORD
String API = "YB96VJANX0STWJ5C";   // Write API KEY
String HOST = "api.thingspeak.com";
String PORT = "80";
int countTrueCommand;
int countTimeCommand; 
boolean found = false; 
int valSensor = 1;
  
SoftwareSerial esp8266(2,3); 
EnergyMonitor emon1; 
Servo sg90;             //initializing a variable for servo named sg90           
void setup() {
  Serial.begin(9600);
  emon1.current(3,30 );
  esp8266.begin(115200);
  sendCommand("AT",5,"OK");
  sendCommand("AT+CWMODE=1",5,"OK");
  sendCommand("AT+CWJAP=\""+ AP +"\",\""+ PASS +"\"",20,"OK");
  rotateSolar();
  automaticLighting();
}

void loop() {
 
 String getData = "GET /update?api_key="+ API +"&field1="+getCurrentValue()+"&field2="+getPowerValue();
 sendCommand("AT+CIPMUX=1",5,"OK");
 sendCommand("AT+CIPSTART=0,\"TCP\",\""+ HOST +"\","+ PORT,15,"OK");
 sendCommand("AT+CIPSEND=0," +String(getData.length()+4),4,">");
 esp8266.println(getData);delay(1500);countTrueCommand++;
 sendCommand("AT+CIPCLOSE=0",5,"OK");
}


String getCurrentValue(){
   Serial.print(" current(A)= ");
   double Irms = emon1.calcIrms(1480);
   Serial.println(Irms*230.0); 
   delay(50);
   return String(Irms); 
  
}


String getPowerValue(){
   Serial.print(" power(W)= ");
   double Irms = emon1.calcIrms(1480);
   Serial.println(Irms); 
   delay(50);
   return String(Irms);  
  
}
void rotateSolar(){
   int R1 = analogRead(LDR1); // reading value from LDR 1
  int R2 = analogRead(LDR2); // reading value from LDR 2

  int diff1= abs(R1 - R2);   // Calculating the difference between the LDR's
  int diff2= abs(R2 - R1);

  if((diff1 <= error) || (diff2 <= error)) {
    //if the difference is under the error then do nothing
  } else {    
    if(R1 > R2)
    {
      initial_position = --initial_position;  //Move the servo towards 0 degree
    }
    if(R1 < R2) 
    {
      initial_position = ++initial_position; //Move the servo towards 180 degree
    }
  }
  sg90.write(initial_position); // write the position to servo
  
  }
void automaticLighting(){
  int R3 = analogRead(LDR3);
  if(R3<50){
     digitalWrite(relay,LOW);
     Serial.println("its night , lights are on");
     Serial.print("LIght intensity of lighting controller =");
     Serial.println(R3);
//     Serial.print("Li"); 
//     Serial.println(R3);
    
  }
  else{
      if(R3>=50){
        digitalWrite(relay,HIGH);
        Serial.print("LIght intensity of lighting controller =");
        Serial.println(R3);
        Serial.println("its day, lights are off");
        }
    }
  
  }
void sendCommand(String command, int maxTime, char readReplay[]) {
  Serial.print(countTrueCommand);
  Serial.print(". at command => ");
  Serial.print(command);
  Serial.print(" ");
  while(countTimeCommand < (maxTime*1))
  {
    esp8266.println(command);//at+cipsend
    if(esp8266.find(readReplay))//ok
    {
      found = true;
      break;
    }
  
    countTimeCommand++;
  }
  
  if(found == true)
  {
    Serial.println("OK");
    countTrueCommand++;
    countTimeCommand = 0;
  }
  
  if(found == false)
  {
    Serial.println("Fail");
    countTrueCommand = 0;
    countTimeCommand = 0;
  }
  
  found = false;
 }
