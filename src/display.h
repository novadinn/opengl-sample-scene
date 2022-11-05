#ifndef DISPLAY_H
#define DISPLAY_H

#include "platform.h"

struct Display {

    shared int kWindowWidth;
    shared int kWindowHeight;
    
    shared float kNearPlane;
    shared float kFarPlane;
    
private:
    Display() {}
};

#endif
