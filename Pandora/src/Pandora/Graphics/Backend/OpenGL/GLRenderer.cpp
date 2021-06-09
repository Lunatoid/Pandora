#if !defined(PD_NO_OPENGL)
#include "GLRenderer.h"

#include "Pandora/Graphics/Backend/OpenGL/GLShader.h"

namespace pd {

GLRenderer::GLRenderer() {
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    vbo.Init(GL_ARRAY_BUFFER);
    vbo.Bind();

    ibo.Init(GL_ELEMENT_ARRAY_BUFFER);
    ibo.Bind();
}

GLRenderer::~GLRenderer() {
    glDeleteVertexArrays(1, &vao);
}

void GLRenderer::Submit(int indexOffset, int indexCount, bool clearData) {
    glBindVertexArray(vao);

    if (indexCount <= 0) {
        indexCount = indices.Count() - indexOffset;
    }

    vbo.SetData(vertices);
    ibo.SetData(indices.SliceAs<byte>(indexOffset, indexCount));

    glDrawElements(GL_TRIANGLES, indices.Count(), GL_UNSIGNED_INT, nullptr);

    if (clearData) {
        ClearData();
    }
}

}

#endif
