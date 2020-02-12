#include <Arduino.h>

#include <platform.h>
#include <platino.pb.h>

#include <pb_common.h>
#include <pb_encode.h>
#include <pb_decode.h>
#include <pb.h>



int contLoop=0;
float value;
int t_ciclo=0;
unsigned long t1,t2,diff;
int SEC = 5;
String filename="test.txt";
String line = "";
uint8_t mode = 0;
pb_PlatinoEvent platino  = pb_PlatinoEvent_init_zero;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);  
  Serial1.begin(9600);  
  while(!Serial){};
  while(!Serial1){};
  
  platform.initializeDisplay();

  /*platform.initINA0();
  platform.initINA2();
  platform.initINA2();
  */
  
  platform.initializeRTC();
  platform.adjustRTC();
  platform.initializeSD();
  
  Serial.println("CLEARDATA");
  Serial.println("LABEL,Time,NoSample,Load(mA),Battery(mA),Panel(mA),Wind(m/s),Temperature(ºC),Humidity(%),Voltage(V),Time(ms)");
  //cols = ['Date', 'Time', 'Load', 'Battery', 'Panel','Wind', 'Temp', 'Humidity', 'Volt']

  delay(1000);
}

void loop() {
  // put your main code here, to run repeatedly:
  platino.numPacket = contLoop;
  Serial.print("DATA,TIME,");
  Serial.print(contLoop);
  Serial.print(",");

  line = line + contLoop + ":";
  contLoop++;
   
  /* Sensor 1: ina1 associated to the load current */
  t1 = millis();
  value=readINA1();
  platino.nodeLoad = value;
  Serial.print(value);
  Serial.print(",");
  t_ciclo=millis()-t1;
  line = line + value + ":";

  /* Sensor 2: ina2 associated to the battery current */
  t1 = millis();
  value=readINA2();
  platino.batteryLoad = value;
  Serial.print(value,4);
  Serial.print(",");
  t2 = millis();
  diff=t2-t1;
  t_ciclo=t_ciclo+(diff);
  line = line + value + ":";

  /* Sensor 3: ina0 associated to the current of the panel */
  t1 = millis();  
  value=readINA0();
  platino.panelLoad = value;
  Serial.print(value,4);
  Serial.print(",");
  t2 = millis();
  diff=t2-t1;
  t_ciclo=t_ciclo+(diff);
  line = line + value + ":";

  /* Sensor 4: get the speed of wind by reading the anenometer */
  t1 = millis();  
  value=platform.getSpeedOfWind();
  platino.windSpeed = value;
  Serial.print(value,4);
  Serial.print(",");
  t2 = millis();
  diff=t2-t1;
  t_ciclo=t_ciclo+(diff);
  line = line + value + ":";
  
  /* Sensor 5: read temperature */
  t1 = millis(); 
  value=platform.getTemperature();
  platino.temperature = value;
  Serial.print(value,4);
  Serial.print(",");
  t2 = millis();
  diff=t2-t1;
  t_ciclo=t_ciclo+(diff);
  line = line + value + ":";

  /* Sensor 6: read humidity   */
  t1 = millis(); 
  value=platform.getHumidity();
  platino.humidity = value;
  Serial.print(value,4);
  Serial.print(",");
  t2 = millis();
  diff=t2-t1;
  t_ciclo=t_ciclo+(diff);
  line = line + value + ":";
  
  /* Sensor 7: read voltage*/
  t1 = millis(); 
  value=platform.getBatteryVoltage();
  platino.batteryVolt = value;
  Serial.print(value,4);
  Serial.print(",");
  t2 = millis();
  diff=t2-t1;
  t_ciclo=t_ciclo+(diff);
  platino.samplingTime = t_ciclo;
  
  line = line + value + ":";
  line = line + t_ciclo;
  Serial.println(t_ciclo);
  

  /* Writing SD */
  platform.open(filename,mode);
  platform.writeline(line);
  platform.close();
  platform.displayLCD("",line);

  /* Send a PB with LoRa*/
  platform.sendLoRaPB(platino);
  
  t_ciclo=0;
  line = "";
  delay(SEC*1000); //60

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
