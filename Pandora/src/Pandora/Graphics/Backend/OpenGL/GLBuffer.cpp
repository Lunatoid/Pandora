#if !defined(PD_NO_OPENGL)
#include "GLBuffer.h"

#include "Pandora/Core/Math/Math.h"

namespace pd {

GLBuffer::~GLBuffer() {
    if (id != 0) {
        glBindBuffer(target, 0);
        glDeleteBuffers(1, &id);
    }
}

void GLBuffer::Init(GLenum target, GLenum usage) {
    if (id == 0) {
        this->target = target;
        this->usage = usage;

        glGenBuffers(1, &id);
    }
}

void GLBuffer::Bind() {
    glBindBuffer(target, id);
}

void GLBuffer::SetData(Slice<byte> bytes) {
    Bind();

    size = bytes.Count();
    if (capacity < size) {
        capacity = RoundToPow2(size);
        glBufferData(target, capacity, nullptr, usage);
    }

    glBufferSubData(target, 0, size, bytes.Data());
}

int GLBuffer::GetCount() const {
    return size;
}

}

#endif
