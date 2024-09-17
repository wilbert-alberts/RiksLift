#ifndef FMOTOR_HH
#define FMOTOR_HH

class FMotor : public skel::FMotor
{
public:
    FMotor(dzn::locator const &locator): skel::FMotor(locator){};
    virtual ~FMotor(){};

    virtual void p_moveUpFast(){};
    virtual void p_moveUpSlow(){};
    virtual void p_moveDownFast(){};
    virtual void p_moveDownSlow(){};
    virtual void p_stop(){};
};

#endif