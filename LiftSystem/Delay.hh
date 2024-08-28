#ifndef DELAY_HH 
#define DELAY_HH


#define TIMEOUT_STOP_WHILE_MOVING_SLOW (0.5)
#define TIMEOUT_ABORT_WHILE_MOVING_FAST (0.5)
#define TIMEOUT_ABORT_WHILE_MOVING_SLOW (0.5)
#define TIMEOUT_STOPPING_AFTER_FAILING (0.5)

#define HOMING_MOVING_SLOW_OUT (0.5)
#define HOMING_MOVING_FAST_IN (0.5)
#define HOMING_STOPPING_FAST_IN (0.5)
#define HOMING_MOVING_SLOW_OUT (0.5)
#define HOMING_MOVING_EXTRA_OUT (0.5)
#define HOMING_STOPPING_SLOW_OUT (0.5)
#define HOMING_MOVING_SLOW_IN (0.5)
#define HOMING_STOPPING_SLOW_IN (0.5)
#define HOMING_FAILING_STOPPING (0.5)
#define HOMING_ABORTING (0.5)

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