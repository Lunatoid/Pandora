#if !defined(PD_NO_OPENGL)
#pragma once

#include "Pandora/Libs/glad/glad.h"

#include "Pandora/Core/Data/Slice.h"

namespace pd {

class GLBuffer {
public:
    ~GLBuffer();

    void Init(GLenum target, GLenum usage = GL_STREAM_DRAW);
    void Bind();

    void SetData(Slice<byte> bytes);

    int GetCount() const;

private:
    GLenum id = 0;
    GLenum target = 0;
    GLenum usage = 0;

    int capacity = 0;
    int size = 0;
};

}

#endif
