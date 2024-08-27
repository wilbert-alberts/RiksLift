#ifndef FMOTOR_HH
#define FMOTOR_HH

class FMotor : public skel::FMotor
{
public:
    FMotor(dzn::locator const& locator): skel::FMotor(locator) {};
    virtual ~FMotor() {};

    virtual void p_moveUpFast() {/* TODO */};
    virtual void p_moveUpSlow()  {/* TODO */};
    virtual void p_moveDownFast()  {/* TODO */};
    virtual void p_moveDownSlow()  {/* TODO */};
    virtual void p_stop()  {/* TODO */};
};

#endif