#ifndef DELAY_HH 
#define DELAY_HH


#define TIMEOUT_STOP_WHILE_MOVING_SLOW (0.5)
#define TIMEOUT_ABORT_WHILE_MOVING_FAST (0.5)
#define TIMEOUT_ABORT_WHILE_MOVING_SLOW (0.5)


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