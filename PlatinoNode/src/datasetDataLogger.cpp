#include <Arduino.h>
//#include "mtpPlatino.pb.h"

#include <platform.h>


#include <platino.pb.h>

#include <pb_common.h>
#include <pb_encode.h>
#include <pb_decode.h>
#include <pb.h>




mtpPlatino_MSG_PHASE1_DRONE_ID msg1       = mtpPlatino_MSG_PHASE1_DRONE_ID_init_zero;
mtpPlatino_MSG_PHASE1_ENDDEVICE_ID msg2   = mtpPlatino_MSG_PHASE1_ENDDEVICE_ID_init_zero;
mtpPlatino_MSG_PHASE2_ENDDEVICE_DATA msg3 = mtpPlatino_MSG_PHASE2_ENDDEVICE_DATA_init_zero;
mtpPlatino_MSG_PHASE2_DRONE_NACK msg4     = mtpPlatino_MSG_PHASE2_DRONE_NACK_init_zero;

int   N = 1;
#ifndef DRONE_ID
#define IDDrone   (0x01)
#define IDDev     (0x0a)    
#endif

uint8_t cont, len, ret;
unsigned long t=0;
uint8_t buffer[RF95_MAX_MESSAGE_LEN];
pb_istream_t istream;
pb_ostream_t ostream;

/* Prototype of functions */
void fillDataMessage(mtpPlatino_MSG_PHASE2_ENDDEVICE_DATA* msg3, uint8_t cont);
int receiveSYNC();
int sendMyIDToDrone();
int sendDataToDrone(mtpPlatino_MSG_PHASE2_ENDDEVICE_DATA* msg3);
void sendLora();
float readINA0();
float readINA1();
float readINA2();



int contLoop=0;
float value;
int t_ciclo=0;
unsigned long t1,t2,diff;
int SEC = 5;
String filename="test.txt";
String line = "";
uint8_t mode = 0;
//pb_PlatinoEvent platino  = pb_PlatinoEvent_init_zero;

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
  
  platform.initializeLoRa();

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
  //platino.numPacket = contLoop;
  msg2.numPackets = contLoop;
  Serial.print("DATA,TIME,");
  Serial.print(contLoop);
  Serial.print(",");

  line = line + contLoop + ":";
  contLoop++;
   
  /* Sensor 1: ina1 associated to the load current */
  t1 = millis();
  value = readINA1();
  //platino.nodeLoad = value;
  msg3.nodeLoad = value;
  Serial.print(value);
  Serial.print(",");
  t_ciclo = millis() - t1;
  line = line + value + ":";

  /* Sensor 2: ina2 associated to the battery current */
  t1 = millis();
  value=readINA2();
  // platino.batteryLoad = value;
  msg3.batteryLoad = value;
  Serial.print(value,4);
  Serial.print(",");
  t2 = millis();
  diff = t2 - t1;
  t_ciclo = t_ciclo + (diff);
  line = line + value + ":";

  /* Sensor 3: ina0 associated to the current of the panel */
  t1 = millis();  
  value = readINA0();
  // platino.panelLoad = value;
  msg3.panelLoad = value;
  Serial.print(value,4);
  Serial.print(",");
  t2 = millis();
  diff = t2 - t1;
  t_ciclo = t_ciclo + (diff);
  line = line + value + ":";

  /* Sensor 4: get the speed of wind by reading the anenometer */
  t1 = millis();  
  value = platform.getSpeedOfWind();
  // platino.windSpeed = value;
  msg3.windSpeed = value;
  Serial.print(value,4);
  Serial.print(",");
  t2 = millis();
  diff = t2 - t1;
  t_ciclo = t_ciclo + (diff);
  line = line + value + ":";
  
  /* Sensor 5: read temperature */
  t1 = millis(); 
  value = platform.getTemperature();
  // platino.temperature = value;
  msg3.temperature = value;
  Serial.print(value,4);
  Serial.print(",");
  t2 = millis();
  diff = t2 - t1;
  t_ciclo = t_ciclo+(diff);
  line = line + value + ":";

  /* Sensor 6: read humidity   */
  t1 = millis(); 
  value = platform.getHumidity();
  //platino.humidity = value;
  msg3.humidity = value;
  Serial.print(value,4);
  Serial.print(",");
  t2 = millis();
  diff = t2 - t1;
  t_ciclo = t_ciclo + (diff);
  line = line + value + ":";
  
  /* Sensor 7: read voltage*/
  t1 = millis(); 
  value = platform.getBatteryVoltage();
  //platino.batteryVolt = value;
  msg3.batteryVolt = value;
  Serial.print(value,4);
  Serial.print(",");
  t2 = millis();
  diff = t2 - t1;
  t_ciclo = t_ciclo + (diff);
  //platino.samplingTime = t_ciclo;
  msg3.samplingTime = t_ciclo;
  
  line = line + value + ":";
  line = line + t_ciclo;
  Serial.println(t_ciclo);
  

  /* Writing SD */
  platform.open(filename,mode);
  platform.writeline(line);
  platform.close();
  platform.displayLCD("",line);

  /* Send a PB with LoRa*/
  // platform.sendLoRaPB(platino);
  sendLora();
  
  t_ciclo = 0;
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

void sendLora() {
  //Phase 1: the end-device receives a message from the drone
  ret = receiveSYNC();

  //sends its identifier
  ret = sendMyIDToDrone();

  //Phase 2: data transmission
  cont = 0;
  while(cont < N) {
    //Fill the message
    fillDataMessage(&msg3, cont);
    ret = sendDataToDrone(&msg3);
    cont = cont + 1;
    if (cont == N) continue;
  }
  delay(t);
}

void fillDataMessage(mtpPlatino_MSG_PHASE2_ENDDEVICE_DATA* msg3, uint8_t cont){
  msg3->idDev = IDDev;
  msg3->idPacket = cont;
  msg3->nodeLoad = 1.2;
  msg3->batteryLoad = 2.1;
  msg3->panelLoad = 0.2;
  msg3->windSpeed = 3.5;
  msg3->temperature = 30;
  msg3->humidity = 80;
  msg3->batteryVolt = 0.2;
  msg3->samplingTime = t;
}

int receiveSYNC(){
  platform.receiveLoRa(buffer,&len);
  istream = pb_istream_from_buffer(buffer, len);
  pb_decode(&istream, mtpPlatino_MSG_PHASE1_DRONE_ID_fields, &msg1);

  Serial.print("Node ");
  Serial.print(IDDev);
  Serial.print(":");
  Serial.print(msg1.id);
  
  if (msg1.id!=IDDrone){
    Serial.println(" DRONE not recognized");
  }else{
    Serial.println(" DRONE  recognized");
  }
  return 0;
}
        
int sendMyIDToDrone(){
  msg2.id = IDDev;
  msg2.numPackets = N;
  ostream = pb_ostream_from_buffer(buffer, sizeof(buffer));
  pb_encode(&ostream, mtpPlatino_MSG_PHASE1_ENDDEVICE_ID_fields, &msg2);
  len = ostream.bytes_written;
  platform.sendLoRaPB(buffer,len);
  return 0;
}


int sendDataToDrone(mtpPlatino_MSG_PHASE2_ENDDEVICE_DATA* msg3){
  bool retransmission = 0;
  while (!retransmission){
    ostream = pb_ostream_from_buffer(buffer, sizeof(buffer));
    pb_encode(&ostream, mtpPlatino_MSG_PHASE2_ENDDEVICE_DATA_fields, &msg3);
    len = ostream.bytes_written;
    platform.sendLoRaPB(buffer,len);

    /* And wait for a while to see if you receive a NACK from the drone */
    platform.receiveLoRa(buffer,&len);
    if (len == 0) { return 0; }
    
    istream = pb_istream_from_buffer(buffer, len);
    pb_decode(&istream, mtpPlatino_MSG_PHASE2_DRONE_NACK_fields, &msg4);

    // We need to check that msg4.idPacket == msg3.idPacket
    if (msg4.idPacket == msg3->idPacket){
      retransmission = 1;
    }else{
      retransmission = 0;
    }
  }
  return 0;
}