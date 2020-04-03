#include <Arduino.h>

#include "mtpPlatino.pb.h"

#include <platform.h>
#include <platino.pb.h>

#include <pb_common.h>
#include <pb_encode.h>
#include <pb_decode.h>
#include <pb.h>

#include <energy_assignment.h>

#include <apps.h>

#include <Thread.h>

// mtpPlatino_MSG_PHASE1_DRONE_ID msg1 = mtpPlatino_MSG_PHASE1_DRONE_ID_init_zero;
// mtpPlatino_MSG_PHASE1_ENDDEVICE_ID msg2 = mtpPlatino_MSG_PHASE1_ENDDEVICE_ID_init_zero;
// mtpPlatino_MSG_PHASE2_ENDDEVICE_DATA msg3 = mtpPlatino_MSG_PHASE2_ENDDEVICE_DATA_init_zero;
// mtpPlatino_MSG_PHASE2_DRONE_NACK msg4 = mtpPlatino_MSG_PHASE2_DRONE_NACK_init_zero;

// int N = 1;
// #ifndef DRONE_ID
// #define IDDrone (0x01)
// #define IDDev (0x0a)
// #endif

bool assigned = false;

/* Prototype of functions */

int *plan_assignment;
Thread app_thread = Thread();

assignmentClass assignment = assignmentClass();

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial1.begin(9600);
  while (!Serial)
  {
  };
  while (!Serial1)
  {
  };

  platform.initializeDisplay();
  platform.initializeLoRa();
  platform.initializeRTC();
  platform.adjustRTC();
  platform.initializeSD();

  Serial.println("CLEARDATA");
  Serial.println("LABEL,Time,NoSample,Load(mA),Battery(mA),Panel(mA),Wind(m/s),Temperature(ºC),Humidity(%),Voltage(V),Time(ms)");
  //cols = ['Date', 'Time', 'Load', 'Battery', 'Panel','Wind', 'Temp', 'Humidity', 'Volt']

  plan_assignment = assignment.assign_plan(platform.getTime());

  app_thread.enabled = true;
  app_thread.setInterval(TIME_SLOTS * 1000);

  delay(1000);
}

void loop()
{
  int *aux_assignment;

  for (int i = 0; i < SLOTS; i++)
  {
    if (platform.getHour() == 23 && !assigned)
    {
      int *time = platform.getTime();

      time[0] += 1;
      time[3] = 0;
      time[4] = 0;
      time[5] = 0;

      aux_assignment = assignment.assign_plan(time);
      assigned = true;
    }
    else if (platform.getHour() == 0)
      plan_assignment = aux_assignment;

    int plan = plan_assignment[i];

    switch (plan)
    {
      case 1:
        app_thread.onRun(apps.app1);
        break;

      case 2:
        app_thread.onRun(apps.app2);
        break;

      case 3:
        app_thread.onRun(apps.app3);
        break;

      case 4:
        app_thread.onRun(apps.app4);
        break;

      case 5:
        app_thread.onRun(apps.app5);
        break;
    }
    
    if (app_thread.shouldRun())
      app_thread.run();

    while (!app_thread.shouldRun())
      continue;
  }
}