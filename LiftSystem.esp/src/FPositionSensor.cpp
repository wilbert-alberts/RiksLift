#include "LiftSystem.hh"
#include "Position.hh"

FPositionSensor::FPositionSensor(dzn::locator const &locator) 
: skel::FPositionSensor(locator) {

                                                                }

FPositionSensor::~FPositionSensor() {

}

void FPositionSensor::p_getCurrentPosition(Position posInM) {
  /* TODO*/
}

void FPositionSensor::p_getLastEndstopPosition(Position posInM) {
  /* TODO*/
}
