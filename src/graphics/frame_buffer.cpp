#include "frame_buffer.h"

#include <glad/glad.h>

#include "../display.h"

FrameBuffer::FrameBuffer(uint id) :
    id_(id) {}

void FrameBuffer::bind(int width, int height) {
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, id_);
    glViewport(0, 0, width, height);
}

void FrameBuffer::unbind() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, Display::window_width, Display::window_height);
}
    
uint FrameBuffer::getId() const {
    return id_;
}
