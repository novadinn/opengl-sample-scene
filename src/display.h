#ifndef DISPLAY_H
#define DISPLAY_H

#include "platform.h"

struct Display {

    shared int window_width;
    shared int window_height;
    
    shared float near_plane;
    shared float far_plane;
    
private:
    Display() {}
};

#endif
