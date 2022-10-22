#include "raw_model.h"

#include <glad/glad.h>

RawModel::RawModel(uint VAO_id, int vertex_count) :
    VAO_id_(VAO_id), vertex_count_(vertex_count) {}

uint RawModel::getVAOId() const {
    return VAO_id_;
}

int RawModel::getVertexCount() const {
    return vertex_count_;
}

void RawModel::bind() const {
    glBindVertexArray(VAO_id_);
}

void RawModel::unbind() {
    glBindVertexArray(0);
}
