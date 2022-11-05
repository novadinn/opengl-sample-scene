#include "raw_model.h"

#include <glad/glad.h>

RawModel::RawModel(uint VAO_id, int data_count) :
    VAO_id_(VAO_id), data_count_(data_count) {}

uint RawModel::getVAOId() const {
    return VAO_id_;
}

int RawModel::getDataCount() const {
    return data_count_;
}

void RawModel::bind() const {
    glBindVertexArray(VAO_id_);
}

void RawModel::unbind() {
    glBindVertexArray(0);
}
