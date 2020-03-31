#ifndef appsClass_h
#define appsClass_h

#include <mtpPlatino.pb.h>

class appsClass
{
public:
    void app1();
    void app2();
    void app3();
    void app4();
    void app5();

private:
    void sendToDrone();
    int receiveSYNC();
    int sendMyIDToDrone();
    int sendDataToDrone(mtpPlatino_MSG_PHASE2_ENDDEVICE_DATA *msg3);
};

extern appsClass apps;

#endif