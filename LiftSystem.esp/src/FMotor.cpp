
#include <dzn/runtime.hh>

#include "LiftSystem.hh"
#include "FMotor.hh"

FMotor::FMotor(dzn::locator const &locator)
    : skel::FMotor(locator) {

};

FMotor::~FMotor() {

};

void FMotor::p_moveUpFast() {
    /* TODO */

    /**
     * Goal: to start moving the lift at fast velocity in towards the upper floor.
     * 
     * Note: the speed of the fast movement must more or less correlate with the 
     *       velocity as defined in FPlanner.cc
     *
     */
};

void FMotor::p_moveUpSlow() {
    /* TODO */

    /**
     * Goal: to start moving the lift at slow velocity in towards the upper floor.
     * 
     * Note: the speed of the fast movement must more or less correlate with the 
     *       velocity as defined in FPlanner.cc
     *
     */
};

void FMotor::p_moveDownFast() {
    /* TODO */

    /**
     * Goal: to start moving the lift at fast velocity in towards the lower floor.
     * 
     * Note: the speed of the fast movement must more or less correlate with the 
     *       velocity as defined in FPlanner.cc
     *
     */
};
void FMotor::p_moveDownSlow() {
    /* TODO */

    /**
     * Goal: to start moving the lift at slow velocity in towards the lower floor.
     * 
     * Note: the speed of the slow movement must more or less correlate with the 
     *       velocity as defined in FPlanner.cc
     *
     */
};

void FMotor::p_stop() {
    /* TODO */

    /**
     * Goal: to stop moving the lift.
     */
};
