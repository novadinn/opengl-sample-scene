#include "indexed_model.h"

IndexedModel::IndexedModel(uint VAO_id, int indices_count) :
    VAO_id_(VAO_id), indices_count_(indices_count) {}

uint IndexedModel::getVAOId() const {
    return VAO_id_;
}

uint IndexedModel::getIndicesCount() const {
    return indices_count_;
}

void IndexedModel::bind() const {
    glBindVertexArray(VAO_id_);
}

void IndexedModel::unbind() {
    glBindVertexArray(0);
}

void IndexedModel::enableAttribute(int number) {
    glEnableVertexAttribArray(number);
}

void IndexedModel::disableAttribute(int number) {
    glDisableVertexAttribArray(number);
}
