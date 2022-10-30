#include "mesh.h"

Mesh::Mesh(ResourceLoader& loader, std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<ObjectTexture> textures) :
    model_(loader.setupMeshVAO(vertices, indices)), textures_(textures) {}

void Mesh::draw(Shader &shader)  {
    unsigned int n_diffuse = 1;
    unsigned int n_specular = 1;
    // unsigned int n_normal = 1;
    // unsigned int n_height = 1;
    for(int i = 0; i < textures_.size(); ++i) {
	Texture2D::activate(i);
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
	textures_[i].texture.bind();
    }

    model_.bind();

    IndexedModel::enableAttribute(0);
    IndexedModel::enableAttribute(1);
    IndexedModel::enableAttribute(2);
    
    glDrawElements(GL_TRIANGLES, model_.getIndicesCount(), GL_UNSIGNED_INT, 0);

    IndexedModel::disableAttribute(0);
    IndexedModel::disableAttribute(1);
    IndexedModel::disableAttribute(2);

    IndexedModel::unbind();
    Texture2D::deactivate();
}
