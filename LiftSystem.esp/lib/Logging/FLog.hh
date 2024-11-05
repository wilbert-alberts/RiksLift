#ifndef FLOG_HH
#define FLOG_HH

// #include "ScreenLogger.h"
#ifdef _DEBUG
#undef _DEBUG
#endif

#define _DEBUG 1
#include "debug.h"

class FLog : public skel::FLog
{
public:
    FLog(dzn::locator const &locator)
        : skel::FLog(locator)
    {
    }
    virtual ~FLog() {}

    void setComponentID(const std::string &s)
    {
        componentID = s;
    }
    virtual void p_logMsg(std::string msg)
    {
        DEBUG((componentID + ": " + msg).c_str());
    }

private:
    std::string componentID;
};
#undef _DEBUG
#endif // FLOG_HH