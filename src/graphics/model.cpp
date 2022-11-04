#include "model.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <fstream>
#include <sstream>

#include "file_system.h"

Model::Model(ResourceLoader& loader, Shader shader, std::string const &path) {
    loadModel(loader, shader, path);
}

void Model::draw() {    
    for(int i = 0; i < meshes_.size(); ++i)
	meshes_[i].draw();
}

void Model::loadModel(ResourceLoader& loader, Shader shader, std::string const &path) {
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate |
					     aiProcess_GenSmoothNormals | aiProcess_CalcTangentSpace);

    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
	std::cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
	return;
    }

    directory_ = file_system::path_directory(path);

    processNode(loader, shader, scene->mRootNode, scene);
}

void Model::processNode(ResourceLoader& loader, Shader shader, aiNode *node, const aiScene *scene) {
    for(int i = 0; i < node->mNumMeshes; ++i) {
	aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
	meshes_.push_back(processMesh(loader, shader, mesh, scene));
    }
    for(int i = 0; i < node->mNumChildren; ++i) {
	processNode(loader, shader, node->mChildren[i], scene);
    }
}

Mesh Model::processMesh(ResourceLoader& loader, Shader shader, aiMesh *mesh, const aiScene *scene) {
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<ObjectTexture> textures;

    for(int i = 0; i < mesh->mNumVertices; ++i) {
	Vertex vertex;
	glm::vec3 vector;

	vector.x = mesh->mVertices[i].x;
	vector.y = mesh->mVertices[i].y;
	vector.z = mesh->mVertices[i].z;
	vertex.position = vector;
	    
	if (mesh->HasNormals()) {
	    vector.x = mesh->mNormals[i].x;
	    vector.y = mesh->mNormals[i].y;
	    vector.z = mesh->mNormals[i].z;
	    vertex.normal = vector;
	}

	if(mesh->mTextureCoords[0]) {
	    glm::vec2 vec;
	    vec.x = mesh->mTextureCoords[0][i].x; 
	    vec.y = mesh->mTextureCoords[0][i].y;
	    vertex.tex_coords = vec;

	    vector.x = mesh->mTangents[i].x;
	    vector.y = mesh->mTangents[i].y;
	    vector.z = mesh->mTangents[i].z;
	    vertex.tangent = vector;

	    vector.x = mesh->mBitangents[i].x;
	    vector.y = mesh->mBitangents[i].y;
	    vector.z = mesh->mBitangents[i].z;
	    vertex.bitangent = vector;
	}
	else {
	    vertex.tex_coords = glm::vec2(0.0f, 0.0f);
	}

	vertices.push_back(vertex);
    }

    for(int i = 0; i < mesh->mNumFaces; ++i) {
	aiFace face = mesh->mFaces[i];
	for(int j = 0; j < face.mNumIndices; j++)
	    indices.push_back(face.mIndices[j]);        
    }

    aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];    

    std::vector<ObjectTexture> diffuse_maps = loadMaterialTextures(loader, material, aiTextureType_DIFFUSE);
    textures.insert(textures.end(), diffuse_maps.begin(), diffuse_maps.end());

    std::vector<ObjectTexture> specular_maps = loadMaterialTextures(loader, material, aiTextureType_SPECULAR);
    textures.insert(textures.end(), specular_maps.begin(), specular_maps.end());
    
    // std::vector<Texture> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
    // textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());

    // std::vector<Texture> heightMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
    // textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());
        
    return Mesh(loader, shader, vertices, indices, textures);
}

std::vector<ObjectTexture> Model::loadMaterialTextures(ResourceLoader& loader, aiMaterial *mat,
						       aiTextureType type) {
    std::vector<LoadedTexture> textures;
    for(int i = 0; i < mat->GetTextureCount(type); ++i) {
	aiString str;
	mat->GetTexture(type, i, &str);
	bool skip = false;
	for(int j = 0; j < textures_loaded_.size(); j++) {
	    if(std::strcmp(textures_loaded_[j].path.data(), str.C_Str()) == 0) {
		textures.push_back(textures_loaded_[j]);
		skip = true;
		break;
	    }
	}
	if(!skip) {
	    LoadedTexture texture;
	    texture.object_texture.texture = loader.loadTexture(
		file_system::join((directory_ + '\\' + str.data)).c_str());
	    if(type == aiTextureType_DIFFUSE) {
		texture.object_texture.type = DIFFUSE;
	    } else if(type == aiTextureType_SPECULAR) {
		texture.object_texture.type = SPECULAR;
	    }
	    texture.path = str.C_Str();
	    textures.push_back(texture);
	    textures_loaded_.push_back(texture);
	}
    }

    std::vector<ObjectTexture> object_textures;
    for(int i = 0; i < textures.size(); ++i) {
	object_textures.push_back(textures[i].object_texture);
    }
	
    return object_textures;
}
