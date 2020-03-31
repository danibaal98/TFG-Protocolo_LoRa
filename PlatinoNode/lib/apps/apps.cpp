#include <Arduino.h>

#include "apps.h"

#include "mtpPlatino.pb.h"

#include <platform.h>

#include <pb_common.h>
#include <pb_encode.h>
#include <pb_decode.h>
#include <pb.h>

#include <energy_assignment.h>

mtpPlatino_MSG_PHASE1_DRONE_ID msg1 = mtpPlatino_MSG_PHASE1_DRONE_ID_init_zero;
mtpPlatino_MSG_PHASE1_ENDDEVICE_ID msg2 = mtpPlatino_MSG_PHASE1_ENDDEVICE_ID_init_zero;
mtpPlatino_MSG_PHASE2_ENDDEVICE_DATA msg3 = mtpPlatino_MSG_PHASE2_ENDDEVICE_DATA_init_zero;
mtpPlatino_MSG_PHASE2_DRONE_NACK msg4 = mtpPlatino_MSG_PHASE2_DRONE_NACK_init_zero;

int N = 1;
#ifndef DRONE_ID
#define IDDrone (0x01)
#define IDDev (0x0a)
#endif

uint8_t cont, len, ret;
unsigned long t = 0;
uint8_t buffer[RF95_MAX_MESSAGE_LEN];
pb_istream_t istream;
pb_ostream_t ostream;

int contLoop = 0;
float value;
int t_ciclo = 0;
unsigned long t1, t2, diff;
int SEC = 5;
String filename = "test.txt";
String line = "";
uint8_t mode = 0;

void appsClass::app1()
{
    while (1)
    {
    }
}

void appsClass::app2()
{
    while (1)
    {
        msg2.numPackets = contLoop;
        Serial.print("DATA,TIME,");
        Serial.print(contLoop);
        Serial.print(",");

        /* Sensor 1: ina1 associated to the load current */
        t1 = millis();
        value = platform.getLoadCurrent();
        Serial.print(value);
        Serial.print(",");
        t_ciclo = millis() - t1;

        /* Sensor 2: ina2 associated to the battery current */
        t1 = millis();
        value = platform.getBatteryCurrent();
        Serial.print(value, 4);
        Serial.print(",");
        t2 = millis();
        diff = t2 - t1;
        t_ciclo = t_ciclo + (diff);

        /* Sensor 3: ina0 associated to the current of the panel */
        t1 = millis();
        value = platform.getPanelCurrent();
        Serial.print(value, 4);
        Serial.print(",");
        t2 = millis();
        diff = t2 - t1;
        t_ciclo = t_ciclo + (diff);

        /* Sensor 4: get the speed of wind by reading the anenometer */
        t1 = millis();
        value = platform.getSpeedOfWind();
        Serial.print(value, 4);
        Serial.print(",");
        t2 = millis();
        diff = t2 - t1;
        t_ciclo = t_ciclo + (diff);

        /* Sensor 5: read temperature */
        t1 = millis();
        value = platform.getTemperature();
        Serial.print(value, 4);
        Serial.print(",");
        t2 = millis();
        diff = t2 - t1;
        t_ciclo = t_ciclo + (diff);

        /* Sensor 6: read humidity   */
        t1 = millis();
        value = platform.getHumidity();
        Serial.print(value, 4);
        Serial.print(",");
        t2 = millis();
        diff = t2 - t1;
        t_ciclo = t_ciclo + (diff);

        /* Sensor 7: read voltage*/
        t1 = millis();
        value = platform.getBatteryVoltage();
        Serial.print(value, 4);
        Serial.print(",");
        t2 = millis();
        diff = t2 - t1;
        t_ciclo = t_ciclo + (diff);

        Serial.println(t_ciclo);
    }
    
}

void appsClass::app3()
{
    while (1)
    {
        msg2.numPackets = contLoop;
        Serial.print("DATA,TIME,");
        Serial.print(contLoop);
        Serial.print(",");

        line = line + contLoop + ":";
        contLoop++;

        /* Sensor 1: ina1 associated to the load current */
        t1 = millis();
        value = platform.getLoadCurrent();
        Serial.print(value);
        Serial.print(",");
        t_ciclo = millis() - t1;
        line = line + value + ":";

        /* Sensor 2: ina2 associated to the battery current */
        t1 = millis();
        value = platform.getBatteryCurrent();
        Serial.print(value, 4);
        Serial.print(",");
        t2 = millis();
        diff = t2 - t1;
        t_ciclo = t_ciclo + (diff);
        line = line + value + ":";

        /* Sensor 3: ina0 associated to the current of the panel */
        t1 = millis();
        value = platform.getPanelCurrent();
        Serial.print(value, 4);
        Serial.print(",");
        t2 = millis();
        diff = t2 - t1;
        t_ciclo = t_ciclo + (diff);
        line = line + value + ":";

        /* Sensor 4: get the speed of wind by reading the anenometer */
        t1 = millis();
        value = platform.getSpeedOfWind();
        Serial.print(value, 4);
        Serial.print(",");
        t2 = millis();
        diff = t2 - t1;
        t_ciclo = t_ciclo + (diff);
        line = line + value + ":";

        /* Sensor 5: read temperature */
        t1 = millis();
        value = platform.getTemperature();
        Serial.print(value, 4);
        Serial.print(",");
        t2 = millis();
        diff = t2 - t1;
        t_ciclo = t_ciclo + (diff);
        line = line + value + ":";

        /* Sensor 6: read humidity   */
        t1 = millis();
        value = platform.getHumidity();
        Serial.print(value, 4);
        Serial.print(",");
        t2 = millis();
        diff = t2 - t1;
        t_ciclo = t_ciclo + (diff);
        line = line + value + ":";

        /* Sensor 7: read voltage*/
        t1 = millis();
        value = platform.getBatteryVoltage();
        Serial.print(value, 4);
        Serial.print(",");
        t2 = millis();
        diff = t2 - t1;
        t_ciclo = t_ciclo + (diff);

        line = line + value + ":";
        line = line + t_ciclo;
        Serial.println(t_ciclo);

        /* Writing SD */
        platform.open(filename, mode);
        platform.writeline(line);
        platform.close();
        platform.displayLCD("", line);
    }
}

void appsClass::app4()
{
    while (1)
    {
        msg2.numPackets = contLoop;
        Serial.print("DATA,TIME,");
        Serial.print(contLoop);
        Serial.print(",");

        msg3.idDev = IDDev;

        /* Sensor 1: ina1 associated to the load current */
        t1 = millis();
        value = platform.getLoadCurrent();
        msg3.nodeLoad = value;
        Serial.print(value);
        Serial.print(",");
        t_ciclo = millis() - t1;

        /* Sensor 2: ina2 associated to the battery current */
        t1 = millis();
        value = platform.getBatteryCurrent();
        msg3.batteryLoad = value;
        Serial.print(value, 4);
        Serial.print(",");
        t2 = millis();
        diff = t2 - t1;
        t_ciclo = t_ciclo + (diff);

        /* Sensor 3: ina0 associated to the current of the panel */
        t1 = millis();
        value = platform.getPanelCurrent();
        msg3.panelLoad = value;
        Serial.print(value, 4);
        Serial.print(",");
        t2 = millis();
        diff = t2 - t1;
        t_ciclo = t_ciclo + (diff);

        /* Sensor 4: get the speed of wind by reading the anenometer */
        t1 = millis();
        value = platform.getSpeedOfWind();
        msg3.windSpeed = value;
        Serial.print(value, 4);
        Serial.print(",");
        t2 = millis();
        diff = t2 - t1;
        t_ciclo = t_ciclo + (diff);

        /* Sensor 5: read temperature */
        t1 = millis();
        value = platform.getTemperature();
        msg3.temperature = value;
        Serial.print(value, 4);
        Serial.print(",");
        t2 = millis();
        diff = t2 - t1;
        t_ciclo = t_ciclo + (diff);

        /* Sensor 6: read humidity   */
        t1 = millis();
        value = platform.getHumidity();
        msg3.humidity = value;
        Serial.print(value, 4);
        Serial.print(",");
        t2 = millis();
        diff = t2 - t1;
        t_ciclo = t_ciclo + (diff);

        /* Sensor 7: read voltage*/
        t1 = millis();
        value = platform.getBatteryVoltage();
        msg3.batteryVolt = value;
        Serial.print(value, 4);
        Serial.print(",");
        t2 = millis();
        diff = t2 - t1;
        t_ciclo = t_ciclo + (diff);
        msg3.samplingTime = t_ciclo;

        Serial.println(t_ciclo);

        sendToDrone();
    }
    
}

void appsClass::app5()
{
    while (1)
    {
        msg2.numPackets = contLoop;
        Serial.print("DATA,TIME,");
        Serial.print(contLoop);
        Serial.print(",");

        line = line + contLoop + ":";
        contLoop++;

        msg3.idDev = IDDev;

        /* Sensor 1: ina1 associated to the load current */
        t1 = millis();
        value = platform.getLoadCurrent();
        msg3.nodeLoad = value;
        Serial.print(value);
        Serial.print(",");
        t_ciclo = millis() - t1;
        line = line + value + ":";

        /* Sensor 2: ina2 associated to the battery current */
        t1 = millis();
        value = platform.getBatteryCurrent();
        msg3.batteryLoad = value;
        Serial.print(value, 4);
        Serial.print(",");
        t2 = millis();
        diff = t2 - t1;
        t_ciclo = t_ciclo + (diff);
        line = line + value + ":";

        /* Sensor 3: ina0 associated to the current of the panel */
        t1 = millis();
        value = platform.getPanelCurrent();
        msg3.panelLoad = value;
        Serial.print(value, 4);
        Serial.print(",");
        t2 = millis();
        diff = t2 - t1;
        t_ciclo = t_ciclo + (diff);
        line = line + value + ":";

        /* Sensor 4: get the speed of wind by reading the anenometer */
        t1 = millis();
        value = platform.getSpeedOfWind();
        msg3.windSpeed = value;
        Serial.print(value, 4);
        Serial.print(",");
        t2 = millis();
        diff = t2 - t1;
        t_ciclo = t_ciclo + (diff);
        line = line + value + ":";

        /* Sensor 5: read temperature */
        t1 = millis();
        value = platform.getTemperature();
        msg3.temperature = value;
        Serial.print(value, 4);
        Serial.print(",");
        t2 = millis();
        diff = t2 - t1;
        t_ciclo = t_ciclo + (diff);
        line = line + value + ":";

        /* Sensor 6: read humidity   */
        t1 = millis();
        value = platform.getHumidity();
        msg3.humidity = value;
        Serial.print(value, 4);
        Serial.print(",");
        t2 = millis();
        diff = t2 - t1;
        t_ciclo = t_ciclo + (diff);
        line = line + value + ":";

        /* Sensor 7: read voltage*/
        t1 = millis();
        value = platform.getBatteryVoltage();
        msg3.batteryVolt = value;
        Serial.print(value, 4);
        Serial.print(",");
        t2 = millis();
        diff = t2 - t1;
        t_ciclo = t_ciclo + (diff);
        msg3.samplingTime = t_ciclo;

        line = line + value + ":";
        line = line + t_ciclo;
        Serial.println(t_ciclo);

        /* Writing SD */
        platform.open(filename, mode);
        platform.writeline(line);
        platform.close();
        platform.displayLCD("", line);

        /* Send a PB with LoRa*/
        sendToDrone();

        t_ciclo = 0;
        line = "";
        delay(SEC * 1000); //60
    }
}

void appsClass::sendToDrone()
{
    //Phase 1: the end-device receives a message from the drone
    ret = receiveSYNC();
    if (ret == -1) return;

    //sends its identifier
    ret = sendMyIDToDrone();

    //Phase 2: data transmission
    cont = 0;
    while (cont < N)
    {
        //Send the data to the drone
        msg3.idPacket = cont;
        ret = sendDataToDrone(&msg3);
        cont = cont + 1;
        if (cont == N)
            continue;
    }
    delay(t);
}

int appsClass::receiveSYNC()
{
    platform.receiveLoRa(buffer, &len);
    istream = pb_istream_from_buffer(buffer, len);
    pb_decode(&istream, mtpPlatino_MSG_PHASE1_DRONE_ID_fields, &msg1);

    Serial.print("Node ");
    Serial.print(IDDev);
    Serial.print(":");
    Serial.print(msg1.id);

    if (msg1.id != IDDrone)
    {
        Serial.println(" DRONE not recognized");
        return -1;
    }
    else
    {
        Serial.println(" DRONE  recognized");
    }
    return 0;
}

int appsClass::sendMyIDToDrone()
{
    msg2.id = IDDev;
    msg2.numPackets = N;
    ostream = pb_ostream_from_buffer(buffer, sizeof(buffer));
    pb_encode(&ostream, mtpPlatino_MSG_PHASE1_ENDDEVICE_ID_fields, &msg2);
    len = ostream.bytes_written;
    platform.sendLoRaPB(buffer, len);
    return 0;
}

int appsClass::sendDataToDrone(mtpPlatino_MSG_PHASE2_ENDDEVICE_DATA *msg3)
{
    bool retransmission = 0;
    while (!retransmission)
    {
        ostream = pb_ostream_from_buffer(buffer, sizeof(buffer));
        pb_encode(&ostream, mtpPlatino_MSG_PHASE2_ENDDEVICE_DATA_fields, &msg3);
        len = ostream.bytes_written;
        platform.sendLoRaPB(buffer, len);

        /* And wait for a while to see if you receive a NACK from the drone */
        platform.receiveLoRa(buffer, &len);
        if (len == 0)
        {
            return 0;
        }

        istream = pb_istream_from_buffer(buffer, len);
        pb_decode(&istream, mtpPlatino_MSG_PHASE2_DRONE_NACK_fields, &msg4);

        // We need to check that msg4.idPacket == msg3.idPacket
        if (msg4.idPacket == msg3->idPacket)
        {
            retransmission = 1;
        }
        else
        {
            retransmission = 0;
        }
    }
    return 0;
}
