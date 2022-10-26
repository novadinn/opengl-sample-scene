#ifndef GUI_ELEMENT_H
#define GUI_ELEMENT_H

#include <glm/glm.hpp>

#include "resource_loader.h"

struct GuiElement {
    GuiElement(Texture2D texture, ResourceLoader& loader);

    void draw();
    
    glm::vec2 position;
    glm::vec2 size;
    float rotation;
private:
    Texture2D texture_;
    Shader shader_;
    RawModel model_;
};

#endif
