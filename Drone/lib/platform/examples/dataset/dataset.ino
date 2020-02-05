#include <platform.h>

int contLoop=0;
float value;
int t_ciclo=0;
unsigned long t1,t2,diff;
int SEC = 60;
String filename="dataset.txt";
String line = "";
uint8_t mode = 0;

void setup() {
  // put your setup code here, to run once:
  platform.initializeRTC();
  platform.adjustRTC();
  //platform.open(filename,mode);
  Serial.begin(9600);  // Re-establishes serial communication , this causes deletion of anything previously stored in the buffer or cache
  Serial.println("CLEARDATA");
  Serial.println("LABEL,Time,NoSample,Load(mA),Battery(mA),Panel(mA),Wind(m/s),Temperature(ºC),Humidity(%),Voltage(V),Time(ms)");
  //cols = ['Date', 'Time', 'Load', 'Battery', 'Panel','Wind', 'Temp', 'Humidity', 'Volt']
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.print("DATA,TIME,");
  Serial.print(contLoop);
  Serial.print(",");

  line = line + contLoop + ":";
  contLoop++;
   
  /* Sensor 1: ina1 associated to the load current */
  t1 = millis();
  value=readINA1();
  Serial.print(value);
  Serial.print(",");
  t_ciclo=millis()-t1;
  line = line + value + ":";

  /* Sensor 2: ina2 associated to the battery current */
  t1 = millis();
  value=readINA2();
  Serial.print(value,4);
  Serial.print(",");
  t2 = millis();
  diff=t2-t1;
  t_ciclo=t_ciclo+(diff);
  line = line + value + ":";

  /* Sensor 3: ina0 associated to the current of the panel */
  t1 = millis();  
  value=readINA0();
  Serial.print(value,4);
  Serial.print(",");
  t2 = millis();
  diff=t2-t1;
  t_ciclo=t_ciclo+(diff);
  line = line + value + ":";

  /* Sensor 4: get the speed of wind by reading the anenometer */
  t1 = millis();  
  value=platform.getSpeedOfWind();
  Serial.print(value,4);
  Serial.print(",");
  t2 = millis();
  diff=t2-t1;
  t_ciclo=t_ciclo+(diff);
  line = line + value + ":";

  /* Sensor 5: read temperature */
  t1 = millis(); 
  value=platform.getTemperature();
  Serial.print(value,4);
  Serial.print(",");
  t2 = millis();
  diff=t2-t1;
  t_ciclo=t_ciclo+(diff);
  line = line + value + ":";

  /* Sensor 6: read humidity */
  t1 = millis(); 
  value=platform.getHumidity();
  Serial.print(value,4);
  Serial.print(",");
  t2 = millis();
  diff=t2-t1;
  t_ciclo=t_ciclo+(diff);
  line = line + value + ":";
  
  /* Sensor 7: read voltage*/
  t1 = millis(); 
  value=platform.getHumidity();
  Serial.print("");
  Serial.print(",");
  t2 = millis();
  diff=t2-t1;
  t_ciclo=t_ciclo+(diff);
  line = line + value + ":";
  
  Serial.println(t_ciclo);
  t_ciclo=0;
  delay(SEC*1000); //60

  //platform.writeline(line);
  line = "";
  
}

float readINA0(){
  float value;

  value = platform.getPanelCurrent();
  return value;
}

float readINA1(){
  float value;

  value = platform.getLoadCurrent();
  return value;
}

float readINA2(){
  float value;

  value = platform.getBatteryCurrent();
  return value;
}
