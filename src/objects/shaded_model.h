#ifndef SHADER_MODEL_H
#define SHADER_MODEL_H

#include "game_object.h"
#include "../graphics/model.h"
#include "../graphics/resource_loader.h"

struct ShadedModel : public GameObject {
    ShadedModel(ResourceLoader& loader, Shader shader, std::string const &path);

    void draw(glm::mat4& projection, glm::mat4& view);
    
protected:
    void prepareDrawing();
    void draw();
    void endDrawing();

private:
    
    Model model_;
};

#endif
