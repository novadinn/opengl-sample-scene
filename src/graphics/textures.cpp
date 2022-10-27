#include "textures.h"

#include <glad/glad.h>

Texture2D::Texture2D(uint id, int width, int height)
    : id_(id), width_(width), height_(height) {}

uint Texture2D::getId() const {
    return id_;
}

uint Texture2D::getWidth() const {
    return width_;
}

uint Texture2D::getHeight() const {
    return height_;
}

void Texture2D::bind() const {
    glBindTexture(GL_TEXTURE_2D, id_);
}

void Texture2D::bind(uint id) {
    glBindTexture(GL_TEXTURE_2D, id);
}

void Texture2D::unbind() {
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture2D::activate(int index) {
    glActiveTexture(GL_TEXTURE0+index);
}

void Texture2D::deactivate() {
    glActiveTexture(GL_TEXTURE0);
}

Texture3D::Texture3D(uint id) :
    id_(id) {}

uint Texture3D::getId() const {
    return id_;
}
    
void Texture3D::bind() const {
    glBindTexture(GL_TEXTURE_CUBE_MAP, id_);
}

void Texture3D::bind(uint id) {
    glBindTexture(GL_TEXTURE_CUBE_MAP, id);
}

void Texture3D::unbind() {
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

void Texture3D::activate(int index) {
    glActiveTexture(GL_TEXTURE0+index);
}
