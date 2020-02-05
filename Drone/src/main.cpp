#include <Arduino.h>
#include "platform.h"
#include "mtpPlatino.pb.h"


#ifndef DRONE_ID
#define IDDrone   (0x01)
#define IDDev     (0x0a)    
#endif


mtpPlatino_MSG_PHASE1_DRONE_ID msg1       = mtpPlatino_MSG_PHASE1_DRONE_ID_init_zero;
mtpPlatino_MSG_PHASE1_ENDDEVICE_ID msg2   = mtpPlatino_MSG_PHASE1_ENDDEVICE_ID_init_zero;
mtpPlatino_MSG_PHASE2_ENDDEVICE_DATA msg3 = mtpPlatino_MSG_PHASE2_ENDDEVICE_DATA_init_zero;
mtpPlatino_MSG_PHASE2_DRONE_NACK msg4     = mtpPlatino_MSG_PHASE2_DRONE_NACK_init_zero;

int MAX   = 5;        /* Max number of devices */
int SYNC  = 60;       /* Synchronization time */
int DroneID = DRONE_ID;   /* Drone ID */


typedef struct
{
    uint8_t id;
    uint8_t numPackets;
    uint8_t lastPacket;
}record_type;


/* Variables */
uint8_t i, ret, cont, len;
unsigned long t=0;
bool retransmission = 0;
uint8_t buffer[RF95_MAX_MESSAGE_LEN];
uint8_t numDevices = 0;
record_type record[5];

/* Buffers de entrada y salida para los mensajes */
pb_istream_t istream;
pb_ostream_t ostream;

/* Prototypes of functions */
int sendMessageSync();
int receiveResponseSync();
int recvDataFromNodes(int num);

void setup() {
  Serial.begin(9600);  
  while(!Serial){};
  platform.initializeLoRa();
  delay(1000);
}

void loop() {
  
      //Phase 1: the drone sends a message with its id
      ret = sendMessageSync();
      numDevices = receiveResponseSync();
      
      //Phase 2: data reception
      cont = 1;
      while(1){ ret = recvDataFromNodes(numDevices);}
      
      /* Sleep for the next time */
      delay(t);
}


int sendMessageSync(){
      msg1.id = DroneID;
      ostream = pb_ostream_from_buffer(buffer, sizeof(buffer));
      pb_encode(&ostream, mtpPlatino_MSG_PHASE1_DRONE_ID_fields, &msg1);
      len = ostream.bytes_written;
      platform.sendLoRaPB(buffer,len);
      return 1;
}

int receiveResponseSync(){
      // Keep receiving for a while
      cont = 0;
      int num = 0;
      
      while ((cont<SYNC) && (num<MAX)){
        platform.receiveLoRa(buffer,&len);
        istream = pb_istream_from_buffer(buffer, len);
        pb_decode(&istream, mtpPlatino_MSG_PHASE1_ENDDEVICE_ID_fields, &msg2);
        
        record[num].id = msg2.id;
        record[num].numPackets = msg2.numPackets;
        record[num].lastPacket = 0;
        
        num=num+1;
        cont=cont+1;
        delay(1000);
        
      }
      Serial.print("Drone: SYNC completed: ");
      Serial.print(num-1);
      Serial.println(" devices found");
      return num;
}

int recvDataFromNodes(int num){
      platform.receiveLoRa(buffer,&len);
      if (len == 0) { return -1; }
      
      istream = pb_istream_from_buffer(buffer, len);
      pb_decode(&istream, mtpPlatino_MSG_PHASE2_ENDDEVICE_DATA_fields, &msg3);
      
      /* Check which node is */
      i = 0;
      while (record[i].id != msg3.idDev){
        i=i+1;
      }
      if (i>num){
         Serial.println("Drone: Id.dev not recognized");
         return -1;
      }
      /* Check it is the next packet in the sequence */
      if (record[i].lastPacket+1 != msg3.idPacket){
          msg4.id = msg3.idDev;
          msg4.idPacket = record[i].lastPacket;
          ostream = pb_ostream_from_buffer(buffer, sizeof(buffer));
          pb_encode(&ostream, mtpPlatino_MSG_PHASE2_DRONE_NACK_fields, &msg4);
          len = ostream.bytes_written;
          platform.sendLoRaPB(buffer,len);
          Serial.print("Drone: NACK sent to ");
          Serial.print(msg3.idDev);
          Serial.print(" for Packet ID: ");
          Serial.print(record[i].lastPacket);
       }else{
              Serial.print("Drone received the packet: ");
              Serial.print(msg3.idDev);
              Serial.print(", ");
              Serial.print(msg3.idPacket);
              Serial.print(", ");
              Serial.print(msg3.nodeLoad);
              Serial.print(", ");
              Serial.print(msg3.batteryLoad);
              Serial.print(", ");
              Serial.print(msg3.panelLoad);
              Serial.print(", ");
              Serial.print(msg3.windSpeed);
              Serial.print(", ");
              Serial.print(msg3.temperature);
              Serial.print(", ");
              Serial.print(msg3.humidity);
              Serial.print(", ");
              Serial.print(msg3.batteryVolt);
              Serial.print(", ");
              Serial.print(msg3.samplingTime);
   
        }
        return 0;
}
