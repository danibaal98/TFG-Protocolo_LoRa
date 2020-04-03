#ifndef appsClass_h
#define appsClass_h

#include <mtpPlatino.pb.h>

class appsClass
{
public:
    static void app1();
    static void app2();
    static void app3();
    static void app4();
    static void app5();

protected:
    static void sendToDrone(void);
    static int receiveSYNC();
    static int sendMyIDToDrone();
    static int sendDataToDrone(mtpPlatino_MSG_PHASE2_ENDDEVICE_DATA *msg3);
};

extern appsClass apps;

#endif