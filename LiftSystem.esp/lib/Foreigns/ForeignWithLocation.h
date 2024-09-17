#ifndef FOREIGN_WITH_LOCATION_H
#define FOREIGN_WITH_LOCATION_H

#include "MQTT.h"

#include "Foreign.h"

class ForeignWithLocation: public Foreign
{
public:
    ForeignWithLocation();
    virtual ~ForeignWithLocation();

    void setLocation(MQTT::Location location);
    virtual void connect() override;

protected:
    MQTT::Location location;
};

#endif