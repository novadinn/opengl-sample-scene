#ifndef RAW_MODEL_H
#define RAW_MODEL_H

#include "../platform.h"

struct RawModel {
    RawModel(uint VAO_id, int data_count);

    uint getVAOId() const;
    int getDataCount() const;

    void bind() const;
    shared void unbind();
    shared void enableAttribute(int number);
    shared void disableAttribute(int number);
private:
    uint VAO_id_;
    int data_count_;
};

#endif
