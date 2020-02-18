#include <Arduino.h>


#include "mtpPlatino.pb.h"
#include "platform.h"

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


void setup() {
  Serial.begin(9600);  
  while(!Serial){};
  platform.initializeLoRa();
  platform.initializeRTC();
 // platform.initializeSD();
  delay(1000);
}

void loop() {
     
     //Phase 1: the end-device receives a message from the drone
     ret = receiveSYNC();
     
     /* And Sends its identification */
	   ret = sendMyIDToDrone();
	  
	   //Phase 2: data transmission
     cont = 0;
     while(cont<N){
        
        /* Fill the message */
        fillDataMessage(&msg3,cont);
        ret = sendDataToDrone(&msg3);
        cont = cont + 1;
        if (cont == N) { continue; }     
     }
     /* Sleep for the next time */
     delay(t);
}

/* TODO: This function should read from the store of the device the data previously collected */
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
