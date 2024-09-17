#ifndef FLOG_HH
#define FLOG_HH

// #include "ScreenLogger.h"

class FLog : public skel::FLog
{
public:
    FLog(dzn::locator const &locator)
        : skel::FLog(locator)
    {
    }
    virtual ~FLog() {}
    virtual void p_logMsg(std::string msg) {}
};

#endif // FLOG_HH