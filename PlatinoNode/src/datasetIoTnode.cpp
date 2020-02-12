#include <Arduino.h>

#include <platform.h>

String input = "";         // a String to hold incoming data
float value = -100.0;
boolean complete=false;

void serialEvent() {
  while (Serial1.available()) {
    char inChar = (char)Serial1.read();
    input += inChar;
    if (inChar == '\n') {
      complete = true;
    }
  }
}

void setup() {

  // initialize serial:
  Serial.begin(9600);
  Serial1.begin(9600);
  while(!Serial){;}
  while(!Serial1){;}
  input.reserve(20);
  platform.initIoTNode();
}

void loop() {
  
 
  if (complete){
    //Serial.print(input);
    if (input == "TEMPERATURE\n"){
      value=platform.readTemperature() ;
      Serial1.println(value,2);
      
    }
    if (input == "HUMIDITY\n"){
      value=platform.readHumidity();
      Serial1.println(value,2);
    } 
    if (input == "BATTERYVOLT\n"){
      value=platform.readBatteryVoltage();
      Serial1.println(value,2);
    }
    
    Serial.print("order: ");
    Serial.print(input);
    Serial.print(">");
    Serial.println(value,2);

    complete=false;
    input="";
  }
  serialEvent();
}

