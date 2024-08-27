#ifndef FMOVEUPDOWN
#define FMOVEUPDOWN

struct FMoveUpDown: public skel::FMoveUpDown {
    FMoveUpDown(const dzn::locator &locator) 
    : skel::FMoveUpDown(locator) 
    {}
    virtual ~FMoveUpDown() {}

      virtual void p_moveUp () { /* TODO */}
      virtual void p_moveDown () { /* TODO */}
      virtual void p_abort () { /* TODO */}
    
};


#endif