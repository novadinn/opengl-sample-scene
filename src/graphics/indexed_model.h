#ifndef INDEXED_MODEL_H
#define INDEXED_MODEL_H

#include "../platform.h"

struct IndexedModel {
    IndexedModel(uint VAO_id, int indices_count);

    uint getVAOId() const;
    uint getIndicesCount() const;

    void bind() const;
    shared void unbind();
    shared void enableAttribute(int number);
    shared void disableAttribute(int number);
private:
    uint VAO_id_;
    uint indices_count_;
};

#endif
