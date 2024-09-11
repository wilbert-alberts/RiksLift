#ifndef FLOG_HH
#define FLOG_HH

#include "ScreenLogger.h"

class FLog : public skel::FLog
{
public:
    FLog(dzn::locator const &locator)
        : skel::FLog(locator), component("none"), logger()
    {
    }

    void setComponentID(const std::string &cc)
    {
        component = cc;
    }

private:
    std::string component;
    ScreenLogger logger;

    virtual void p_logMsg(std::string msg)
    {
        logger.log(ERROR, component + ": " + msg);
    }
};

#endif // FLOG_HH