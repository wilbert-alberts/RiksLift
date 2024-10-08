
#include <dzn/runtime.hh>

#include "LiftSystem.hh"
#include "FMotor.hh"
#include "vevorVFD.h"

FMotor::FMotor(dzn::locator const &locator)
    : skel::FMotor(locator) {

};

FMotor::~FMotor() {

};

void FMotor::p_moveUpFast() {
    /**
     * Goal: to start moving the lift at fast velocity in towards the upper floor.
     * 
     * Note: the speed of the fast movement must more or less correlate with the 
     *       velocity as defined in FPlanner.cc
     *
     */
    vevorVFD.startMove (true, VFD_HIGH);
};

void FMotor::p_moveUpSlow() {
    /**
     * Goal: to start moving the lift at slow velocity in towards the upper floor.
     * 
     * Note: the speed of the fast movement must more or less correlate with the 
     *       velocity as defined in FPlanner.cc
     *
     */
    vevorVFD.startMove (true, VFD_LOW);
};

void FMotor::p_moveDownFast() {
    /**
     * Goal: to start moving the lift at fast velocity in towards the lower floor.
     * 
     * Note: the speed of the fast movement must more or less correlate with the 
     *       velocity as defined in FPlanner.cc
     *
     */
    vevorVFD.startMove (false, VFD_HIGH);
};
void FMotor::p_moveDownSlow() {
    /**
     * Goal: to start moving the lift at slow velocity in towards the lower floor.
     * 
     * Note: the speed of the slow movement must more or less correlate with the 
     *       velocity as defined in FPlanner.cc
     *
     */
    vevorVFD.startMove (false, VFD_LOW);
};

void FMotor::p_stop() {
    /**
     * Goal: to stop moving the lift.
     */
    vevorVFD.brake ();
};
