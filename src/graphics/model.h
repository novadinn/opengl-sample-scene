#ifndef MODEL_H
#define MODEL_H

#include <glad/glad.h> 
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <string>
#include <vector>

#include "../graphics/resource_loader.h"
#include "../graphics/mesh.h"

struct Model {
    Model(ResourceLoader& loader, Shader shader, std::string const &path);

    void draw();
    
private:
    struct LoadedTexture {
	ObjectTexture object_texture;
	std::string path;
    };
    
    void loadModel(ResourceLoader& loader, Shader shader, std::string const &path);
    void processNode(ResourceLoader& loader, Shader shader, aiNode *node, const aiScene *scene);
    Mesh processMesh(ResourceLoader& loader, Shader shader, aiMesh *mesh, const aiScene *scene);
    std::vector<ObjectTexture> loadMaterialTextures(ResourceLoader& loader, aiMaterial *mat, aiTextureType type);
    
    std::vector<LoadedTexture> textures_loaded_;
    std::vector<Mesh> meshes_;
    std::string directory_;
};

#endif
