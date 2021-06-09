#if !defined(PD_NO_OPENGL)
#include "GLConstantBuffer.h"

#include "Pandora/Graphics/Backend/OpenGL/GLShader.h"

namespace pd {

// @TODO: look into uniform buffers

GLConstantBuffer::GLConstantBuffer() {
    renderer = VideoAPI::Get()->GetRenderer();
    shader = renderer->GetShader();
}

void GLConstantBuffer::SetBindTarget(ShaderType type) {
    // Not applicable
}

void GLConstantBuffer::SetBindSlot(int slot) {
    // Not applicable
}

void GLConstantBuffer::SetStruct(Slice<byte> bytes) {
    CheckActiveShader();

    Slice<DataElement> elements = layout.GetElements();

    GLShader* shader = this->shader.As<GLShader>();
    for (int i = 0; i < elements.Count(); i++) {
        StringView name = elements.Data()[i].name;
        void* data = (void*)&bytes[elements[i].offset];

        switch (elements[i].type) {
            case DataLayoutType::Float:
                shader->SetUniform(name, *(float*)data);
                break;

            case DataLayoutType::Float2:
                shader->SetUniform(name, *(Vec2*)data);
                break;

            case DataLayoutType::Float3:
                shader->SetUniform(name, *(Vec3*)data);
                break;

            case DataLayoutType::Float4:
                shader->SetUniform(name, *(Vec4*)data);
                break;

            case DataLayoutType::Int:
                shader->SetUniform(name, *(int*)data);
                break;

            case DataLayoutType::Int2:
                shader->SetUniform(name, *(Vec2i*)data);
                break;

            case DataLayoutType::Int3:
                shader->SetUniform(name, *(Vec3i*)data);
                break;

            case DataLayoutType::Int4:
                shader->SetUniform(name, *(Vec4i*)data);
                break;

            case DataLayoutType::Mat4:
                shader->SetUniform(name, *(Mat4*)data);
                break;
        }
    }
}

void GLConstantBuffer::SetElement(StringView name, bool data) {
    CheckActiveShader();
    shader.As<GLShader>()->SetUniform(name, data);
}

void GLConstantBuffer::SetElement(StringView name, f32 data) {
    CheckActiveShader();
    shader.As<GLShader>()->SetUniform(name, data);
}

void GLConstantBuffer::SetElement(StringView name, Vec2 data) {
    CheckActiveShader();
    shader.As<GLShader>()->SetUniform(name, data);
}

void GLConstantBuffer::SetElement(StringView name, Vec3 data) {
    CheckActiveShader();
    shader.As<GLShader>()->SetUniform(name, data);
}

void GLConstantBuffer::SetElement(StringView name, Vec4 data) {
    CheckActiveShader();
    shader.As<GLShader>()->SetUniform(name, data);
}

void GLConstantBuffer::SetElement(StringView name, int data) {
    CheckActiveShader();
    shader.As<GLShader>()->SetUniform(name, data);
}

void GLConstantBuffer::SetElement(StringView name, Vec2i data) {
    CheckActiveShader();
    shader.As<GLShader>()->SetUniform(name, data);
}

void GLConstantBuffer::SetElement(StringView name, Vec3i data) {
    CheckActiveShader();
    shader.As<GLShader>()->SetUniform(name, data);
}

void GLConstantBuffer::SetElement(StringView name, Vec4i data) {
    CheckActiveShader();
    shader.As<GLShader>()->SetUniform(name, data);
}

void GLConstantBuffer::SetElement(StringView name, Mat4 data) {
    CheckActiveShader();
    shader.As<GLShader>()->SetUniform(name, data);
}

void GLConstantBuffer::Upload() {
    // Does nothing
}

void GLConstantBuffer::CheckActiveShader() {
    // Check if the active shader changed
    Ref<Shader> newShader = renderer->GetShader();

    if (!shader || newShader.Get() != shader.Get()) {
        // New active shader, switch
        shader = newShader;
    }
}

}

#endif
