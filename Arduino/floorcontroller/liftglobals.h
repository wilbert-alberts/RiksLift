//
// liftglobals.h -- items shared by all lift processing elements
//
// BSla, 29 jul 2024
//
#ifndef _LIFTGLOBALS_H_
#define _LIFTGLOBALS_H_

#include <Arduino.h>


// time between 'automatic' MQTT reports
#define REPORT_INTERVAL (30 SECONDS)

// Possible lift states
enum LiftState   {lsMovingUp, lsMovingDown, lsUp, lsDown, lsStopping, lsStandstill, lsNone};
enum LiftCommand {lcGoUp, lcGoDown, lcStop, lcNone};

extern LiftState stringToLiftState (const String &s);
extern const char *liftStateToString (LiftState ls);
extern LiftCommand stringToLiftCommand (const String &s);
extern const char *liftCommandToString (LiftCommand lc);


// MQTT topics
extern const char *mqtLift;
extern const char *mqtEmergencyButton;
extern const char *mqtButton;
extern const char *mqtIntrusion;
extern const char *mqtLiftSensor;
extern const char *mqtDisplay;
extern const char *mqtFirmware;

// MQTT subtopics
extern const char *mqsUp;
extern const char *mqsDown;
extern const char *mqsMotor;

extern const char *mqsCommand;
extern const char *mqsStatus;
extern const char *mqsVersion;
extern const char *mqsXpMoveTime;

// MQTT_commands and statuses

extern const char *mqcGoUp;
extern const char *mqcGoDown;
extern const char *mqcStop;

extern const char *mqsTriggered;
extern const char *mqsCleared;
extern const char *mqsPressed;
extern const char *mqsReleased;

extern const char *mqsGoingUp;
extern const char *mqsGoingDown;
extern const char *mqsIsUp;
extern const char *mqsIsDown;
extern const char *mqsIsStandstill;

extern const char *mqsArrivalDetected;

extern const char *IDLowerFloor;
extern const char *IDUpperFloor;
extern const char *IDMotorControl;
extern const char *IDIllegal;

#endif