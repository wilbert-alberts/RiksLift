#include <iostream>

#include <dzn/runtime.hh>
#include <dzn/locator.hh>

#include "System.hh"
#include "MQTTBridge.h"
#include "FButton.h"
#include "FMoveUpDown.hh"

// #include "FMoveUpDown.hh"

int main(int argc, char *argv[])
{
    if (argc > 1)
    {
        dzn::runtime rt;
        dzn::locator loc;

        loc.set(rt);

        MQTTBridge mqtt;

        FButton fUpButton;
        FButton fDownButton;

        System sys(loc);

        mqtt.setServer(argv[1]);
        
        fUpButton.setSystem(&sys);
        fUpButton.setMQTTBridge(&mqtt);
        fUpButton.setLocation(MQTT::Location::UP);
        fUpButton.connect();

        fDownButton.setSystem(&sys);
        fDownButton.setMQTTBridge(&mqtt);
        fDownButton.setLocation(MQTT::Location::DOWN);
        fDownButton.connect();

        sys.fmud.setMQTTBridge(&mqtt);
        sys.fmud.connect();       

        sys.safetySystem.foreignIdUp.setLocation(MQTT::Location::UP);
        sys.safetySystem.foreignIdUp.setMQTTBridge(&mqtt);
        sys.safetySystem.foreignIdUp.connect();

        sys.safetySystem.foreignIdDown.setLocation(MQTT::Location::DOWN);
        sys.safetySystem.foreignIdDown.setMQTTBridge(&mqtt);
        sys.safetySystem.foreignIdDown.connect();

        sys.safetySystem.logDown.setComponentID("intrusionDetectionDown");
        sys.safetySystem.logUp.setComponentID("intrusionDetectionUp");
        sys.flog.setComponentID("MoveUpDownBottomArmor");

        mqtt.processMessages();
    }
    else
    {
        std::cerr << "Warning:   no mqtt broker provided." << std::endl;
        std::cerr << "Usage:     RL.exe <mqtt broker>" << std::endl;
        std::cerr << "Example:   RL.exe mqtt://192.168.6.106:1883" << std::endl
                  << std::endl;
        exit(1);
    }
}