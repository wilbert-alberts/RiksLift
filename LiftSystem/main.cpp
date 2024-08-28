#include <iostream>

#include <dzn/runtime.hh>
#include <dzn/locator.hh>

#include "MQTTBridge.h"
#include "FMoveUpDown.h"
#include "LiftSystem.hh"

dzn::runtime rt;
dzn::locator loc;

LiftSystem liftSystem(loc);
FMoveUpDown fMoveUpDown;

static MQTTBridge mqttBridge;

void setup()
{
    loc.set(rt);

    liftSystem.fLog.setComponentID("mudArmor");
    
    fMoveUpDown.setSystem(&liftSystem);
    fMoveUpDown.setMQTTBridge(&mqttBridge);
    fMoveUpDown.connect();
}

void loop()
{
}

int main(int argc, char *argv[])
{

    setup();
    while (1)
        loop();
}