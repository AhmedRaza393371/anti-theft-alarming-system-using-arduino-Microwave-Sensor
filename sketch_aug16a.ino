// include the library code:
#include <LiquidCrystal.h>

int calibrationTime = 30;        

//the time when the sensor outputs a low impulse
long unsigned int lowIn;         

//the amount of milliseconds the sensor has to be low 
//before we assume all motion has stopped
long unsigned int pause = 5000;  

boolean lockLow = true;
boolean takeLowTime;  

int microPin = 7;    //the digital pin connected to the Microwave sensor's output
int ledPin = 5;    //the digital pin connected to the LED output
int Buzzer = 6;    //the digital pin connected to the BUZZER output
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

void setup() {
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  // Print a message to the LCD.
//  lcd.print("WELCOME!");
  Serial.begin(9600);
  pinMode(microPin, INPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(Buzzer, OUTPUT);
  digitalWrite(microPin, LOW);

  //give the sensor some time to calibrate
  Serial.print("calibrating sensor ");
  lcd.clear();
  lcd.print("calibrating sensor ");
    for(int i = 0; i < calibrationTime; i++){
      Serial.print(".");
      lcd.print(".");
      delay(1000);
      }
    Serial.println(" done");
    lcd.clear();
    lcd.println(" done");
    lcd.clear();
    lcd.println("SENSOR ACTIVE");
    Serial.println("SENSOR ACTIVE");
    delay(50);
}  

void loop() {
  // set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting begins with 0):
  lcd.setCursor(0, 1);
 if(digitalRead(microPin) == HIGH){
       digitalWrite(ledPin, HIGH);   //the led visualizes the sensors output pin state
       tone(Buzzer,500);
       if(lockLow){  
         //makes sure we wait for a transition to LOW before any further output is made:
         lockLow = false;            
         Serial.println("---");
          lcd.println("---");
         Serial.print("motion detected at ");
         lcd.clear();
         lcd.print("motion detected at ");
         Serial.print(millis()/1000);
          lcd.print(millis()/1000);
         Serial.println(" sec"); 
         lcd.println(" sec"); 
         delay(50);
         }         
         takeLowTime = true;
       }

     if(digitalRead(microPin) == LOW){       
       digitalWrite(ledPin, LOW);  //the led visualizes the sensors output pin state
       noTone(Buzzer);
       if(takeLowTime){
        lowIn = millis();          //save the time of the transition from high to LOW
        takeLowTime = false;       //make sure this is only done at the start of a LOW phase
        }
       //if the sensor is low for more than the given pause, 
       //we assume that no more motion is going to happen
       if(!lockLow && millis() - lowIn > pause){  
           //makes sure this block of code is only executed again after 
           //a new motion sequence has been detected
           lockLow = true;                        
           Serial.print("motion ended at ");  
           lcd.clear();                                                         
           lcd.print("motion ended at");  
           Serial.print((millis() - pause)/1000);
           lcd.print((millis() - pause)/1000);
           Serial.println(" sec");
           lcd.println(" sec");
           delay(50);
           }
       }
}
