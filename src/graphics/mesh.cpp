#include "mesh.h"

Mesh::Mesh(ResourceLoader& loader, Shader shader, std::vector<Vertex> vertices,
	   std::vector<unsigned int> indices, std::vector<ObjectTexture> textures) :
    model_(loader.setupMeshVAO(vertices, indices)), textures_(textures), shader_(shader) {

    shader_.bind();
    
    unsigned int n_diffuse = 1;
    unsigned int n_specular = 1;
    // unsigned int n_normal = 1;
    // unsigned int n_height = 1;
    for(int i = 0; i < textures_.size(); ++i) {
	std::string number;
	std::string name;
	if(textures_[i].type == DIFFUSE) {
	    number = std::to_string(n_diffuse++);
	    name = "material.diffuse";
	} else if(textures_[i].type == SPECULAR) {
	    number = std::to_string(n_specular++);
	    name = "material.specular";
	}
	// else if(name == "texture_normal")
	//     number = std::to_string(n_normal++);
	//  else if(name == "texture_height")
	//     number = std::to_string(n_height++);

	shader.setInteger((name+number).c_str(), i);
    }

    Shader::unbind();
}

void Mesh::draw()  {
    for(int i = 0; i < textures_.size(); ++i) {
	Texture2D::activate(i);
	textures_[i].texture.bind();
    }
    
    model_.bind();
    
    glDrawElements(GL_TRIANGLES, model_.getDataCount(), GL_UNSIGNED_INT, 0);

    RawModel::unbind();
    Texture2D::deactivate();
    Texture2D::unbind();
}
