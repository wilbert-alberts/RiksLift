#ifndef POSITION_HPP 
#define POSITION_HPP 

class Position{

public:
    Position(): position(0.0) {};
    Position(double p): position(p) {};

    virtual ~Position() {}

    double getPosition() { return position; }
    void setPosition(double position) {
        this->position = position;
    }

private:
    double position;
};

#endif