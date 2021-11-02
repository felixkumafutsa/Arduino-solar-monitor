
// include the library code:
#include <LiquidCrystal.h>
#include <Servo.h>

LiquidCrystal lcd(11, 10, 7, 6, 5, 4);
Servo myservo;

int initial_position = 0;
const int currentPin = A2;
int relay = 8;
int LDR1 = A0;          //Pin at which LDR is connected
int LDR2 = A1;          //Pin at which LDR is connected
int LDR3 = A4;
int error = 1;          //initializing variable for error
int sensitivity = 66;
int adcValue = 0;
int offsetVoltage = 2500;
double adcVoltage = 0;
double currentValue = 0;

void setup() {
  // set up the LCD's number of columns and rows:
  pinMode(relay, OUTPUT);
  Serial.begin(9600);
  Serial.println("WELCOME TO ARDUINO SOLAR POWER PLANT ");
  Serial.println("          MONITORING SYSTEM ");
  Serial.println("By Felix Kumafutsa(Bict1816)");
  Serial.println("                                                                                  ");
  lcd.begin(16, 2);

  digitalWrite(relay, HIGH);
  myservo.attach(9);
}

void loop() {
  int R1 = analogRead(LDR1); // reading value from LDR 1
  int R2 = analogRead(LDR2); // reading value from LDR 2
  int R3 = analogRead(LDR3);
  int diff1 = abs(R1 - R2);  // Calculating the difference between the LDR's
  int diff2 = abs(R2 - R1);


  adcValue = analogRead(currentPin);
  adcVoltage = (adcValue / 1024.0) * 5000;
  currentValue = ((adcVoltage - offsetVoltage) / sensitivity);
  int loadVoltage = currentValue * 12;
  int power = loadVoltage * currentValue * currentValue;
  lcd.print("  BiggiezTech");
  lcd.setCursor(0, 1);
  lcd.print("current = ");
  lcd.print(currentValue, 2);
  lcd.print("A");


  lcd.print("voltage = ");
  lcd.print(int(loadVoltage));
  lcd.print("v");

  lcd.setCursor(0, 0);
  delay(1000);

  lcd.print("power = ");
  lcd.print(power);
  lcd.print("watt             ");
  lcd.setCursor(0, 1);
  lcd.print("                 ");
  lcd.setCursor(0, 0);
  delay(1000);

  int sensorValue = analogRead(A1);
  // print out the value you read:
  Serial.print("Light intensity = ");
  Serial.print(sensorValue);
  Serial.println(" Lux ");
  Serial.print("current =");
  Serial.print(currentValue, 2);
  Serial.println(" Amps");
  Serial.print("voltage = ");
  Serial.print(double(loadVoltage));
  Serial.println(" Volts");
  Serial.print("power = ");
  Serial.print(power);
  Serial.println(" watts");
  delay(500);




  if (R1 > R2)
  {
    initial_position = --initial_position;  //Move the servo towards 0 degree
    myservo.write(initial_position); // write the position to servo
  }
  else {
    initial_position = ++initial_position; //Move the servo towards 180 degree
    myservo.write(initial_position); // write the position to servo
  }


  if (R3 <= 50) {
    digitalWrite(relay, LOW);
    Serial.println("its day , lights are off");

  }
  else {
    if (R3 >= 60) {
      digitalWrite(relay, HIGH);

      Serial.println("its night, lights are on");
    }
  }

}
