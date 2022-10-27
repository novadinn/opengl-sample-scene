#ifndef TEXTURE_H
#define TEXTURE_H

#include "../platform.h"

struct Texture2D {
    Texture2D() {}
    Texture2D(uint id, int width, int height);

    uint getId() const;
    uint getWidth() const;
    uint getHeight() const;

    void bind() const;
    shared void bind(uint id);
    shared void unbind();

    shared void activate(int index);
    shared void deactivate();
private:
    uint id_;
    
    uint width_, height_;
};

struct Texture3D {
    Texture3D(uint id);

    uint getId() const;
    
    void bind() const;
    shared void bind(uint id);
    shared void unbind();

    shared void activate(int index);
private:
    uint id_;
};

#endif
