#ifndef FRAME_BUFFER_H
#define FRAME_BUFFER_H

#include "../platform.h"

struct FrameBuffer {
    FrameBuffer() {}
    FrameBuffer(uint id);

    void bind(int width, int height);
    shared void unbind();
    
    uint getId() const;
private:
    uint id_;
};

#endif
