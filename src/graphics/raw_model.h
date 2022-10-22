#ifndef RAW_MODEL_H
#define RAW_MODEL_H

#include "../platform.h"

struct RawModel {
    RawModel(uint VAO_id, int vertex_count);

    uint getVAOId() const;
    int getVertexCount() const;

    void bind() const;
    shared void unbind();
    
private:
    uint VAO_id_;
    int vertex_count_;
};

#endif
