#ifndef DELAY_HH 
#define DELAY_HH

class Delay {

public:
    Delay() {};
    Delay(double v): delay(v) {};
    virtual ~Delay() {}

    double getDelay() { return delay; }
    void setDelay(double delay) {
        this->delay = delay;
    }

private:
    double delay;
};

#endif