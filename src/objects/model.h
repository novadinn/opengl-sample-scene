#ifndef MODEL_H
#define MODEL_H

#include <glad/glad.h> 
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <string>
#include <vector>

#include "game_object.h"
#include "../graphics/resource_loader.h"
#include "../graphics/mesh.h"

struct Model : public GameObject {
    Model(ResourceLoader& loader, Shader shader, std::string const &path);

    // TODO: add separate function useLight()
    virtual void draw(glm::mat4& projection, glm::mat4& view,
	      glm::vec3& view_pos, DirectionalLight& dir_light,
	      SpotLight& spot_light, PointLight& point_lighto);
    
private:
    struct LoadedTexture {
	ObjectTexture object_texture;
	std::string path;
    };
    
    void loadModel(ResourceLoader& loader, std::string const &path);
    void processNode(ResourceLoader& loader, aiNode *node, const aiScene *scene);
    Mesh processMesh(ResourceLoader& loader, aiMesh *mesh, const aiScene *scene);
    std::vector<ObjectTexture> loadMaterialTextures(ResourceLoader& loader, aiMaterial *mat, aiTextureType type);
    
    std::vector<LoadedTexture> textures_loaded_;
    std::string directory_;

protected:
    std::vector<Mesh> meshes_;
};

#endif
