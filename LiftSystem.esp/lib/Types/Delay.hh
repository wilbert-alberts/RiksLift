#ifndef DELAY_HH 
#define DELAY_HH

// all times in seconds
#define TIMEOUT_STOP_WHILE_MOVING_SLOW (2.0)
#define TIMEOUT_ABORT_WHILE_MOVING_FAST (2.0)
#define TIMEOUT_ABORT_WHILE_MOVING_SLOW (2.0)
#define TIMEOUT_STOPPING_AFTER_FAILING (2.0)

#define HOMING_MOVING_SLOW_OUT (10.0)
#define HOMING_MOVING_FAST_IN (30.0)
#define HOMING_STOPPING_FAST_IN (2.0)
#define HOMING_MOVING_SLOW_OUT (10.0)
#define HOMING_MOVING_EXTRA_OUT (2.0)
#define HOMING_STOPPING_SLOW_OUT (2.0)
#define HOMING_MOVING_SLOW_IN (10.0)
#define HOMING_STOPPING_SLOW_IN (2.0)
#define HOMING_FAILING_STOPPING (10.0)
#define HOMING_ABORTING (2.0)

class Delay {

public:
    Delay() {};
    Delay(double v): delay(v) {};
    virtual ~Delay() {}

    double getDelay() { return delay; }
    void setDelay(double delay) {
        this->delay = delay;
    }
    Delay plus(const Delay& other) {
        return Delay(other.delay + this->delay);
    }

private:
    double delay;
};


#endif