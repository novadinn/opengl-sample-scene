#ifndef MESH_H
#define MESH_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <string>
#include <vector>

#include "resource_loader.h"
#include "../platform.h"

struct Mesh {
    Mesh(ResourceLoader& loader, Shader shader, std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<ObjectTexture> textures);
    
    void draw();
    
private:
    IndexedModel model_;
    std::vector<ObjectTexture> textures_;
    Shader shader_;
};

#endif
