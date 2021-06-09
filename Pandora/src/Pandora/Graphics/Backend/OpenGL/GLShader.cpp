#if !defined(PD_NO_OPENGL)
#include "GLShader.h"

#include "Pandora/Libs/glad/glad.h"

#include "Pandora/Core/IO/File.h"
#include "Pandora/Core/IO/Console.h"

#include "Pandora/Graphics/VideoAPI.h"

namespace pd {

GLShader::~GLShader() {
    if (vertexID != 0) {
        glDeleteShader(vertexID);
    }

    if (pixelID != 0) {
        glDeleteShader(pixelID);
    }
    if (programID != 0) {
        glDeleteProgram(programID);
    }
}

bool GLShader::Load(StringView name) {
    String vertexPath(Allocator::Temporary);
    String pixelPath(Allocator::Temporary);

    vertexPath.Set(name);
    vertexPath.Append(".Vertex.glsl");

    pixelPath.Set(name);
    pixelPath.Append(".Pixel.glsl");

    return Load(vertexPath, pixelPath);
}

bool GLShader::Load(Box& box, StringView name) {

    if (box.HasResource(name)) {
        if (box.GetResourceType(name) != ResourceType::Shader) {
            // Type mismatch
            return false;
        }

        Array<byte> data;
        box.GetResourceData(name, data);

        // Format is as following: <byte, backend> <int, vertex size> <int, pixel size> <vertex...> <pixel...> ...
        MemoryStream memory(data);

        byte backendCount;
        if (memory.ReadByte(&backendCount) != 1) {
            return false;
        }

        if (backendCount == 0) {
            // This shader somehow has no supported backends
            return false;
        }

        u32 vertexSize;
        u32 pixelSize;

        VideoBackend backend = VideoBackend::Count;
        for (int i = 0; i < (int)backendCount; i++) {
            if (memory.Read(&backend) != sizeof(backend)) {
                return false;
            }

            if (backend != VideoBackend::OpenGL) {
                // Skip to next backend
                if (memory.Read(&vertexSize) != sizeof(vertexSize)) {
                    return false;
                }

                memory.Seek((i64)vertexSize);

                if (memory.Read(&pixelSize) != sizeof(pixelSize)) {
                    return false;
                }

                memory.Seek((i64)pixelSize);
            } else {
                // Read sources
                if (memory.Read(&vertexSize) != sizeof(vertexSize)) {
                    return false;
                }

                // Get vertex shader
                i64 vertexStart = memory.Position();
                vertexFile.Set(StringView(data.Data() + vertexStart, 0, vertexSize));

                memory.Seek((i64)vertexSize);
                if (memory.Read(&pixelSize) != sizeof(pixelSize)) {
                    return false;
                }

                // Get pixel shader
                i64 pixelStart = memory.Position();
                pixelFile.Set(StringView(data.Data() + pixelStart, 0, pixelSize));

                CreateFromFile();
                return true;
            }
        }

        // We couldn't find a shader for this backend
        return false;
    }

    String vertexPath;
    vertexPath.Set(name);
    vertexPath.Append(".Vertex.glsl");
    if (box.HasResource(vertexPath)) {
        if (box.GetResourceType(vertexPath) != ResourceType::Binary) {
            // Type mismatch
            return false;
        }

        Array<byte> vertexData(Allocator::Temporary);
        box.GetResourceData(vertexPath, vertexData);
        vertexFile.Set(vertexData.Data());
    }

    String pixelPath;
    pixelPath.Set(name);
    pixelPath.Append(".Pixel.glsl");
    if (box.HasResource(pixelPath)) {
        if (box.GetResourceType(pixelPath) != ResourceType::Binary) {
            // Type mismatch
            return false;
        }

        Array<byte> pixelData(Allocator::Temporary);
        box.GetResourceData(pixelPath, pixelData);
        pixelFile.Set(pixelData.Data());
    }

    CreateFromFile();
    return true;
}

bool GLShader::Load(StringView vertexPath, StringView pixelPath) {
    u64 readVS = ReadEntireFile(vertexPath, vertexFile);
    u64 readPS = ReadEntireFile(pixelPath, pixelFile);

    if (readVS == 0 || readPS == 0) {
        CONSOLE_LOG_DEBUG("[{}OpenGL{}] Vertex or Pixel shader is either empty or does not exist\n\t{}\n\t{}\n",
                    ConColor::Red, ConColor::White, vertexPath, pixelPath);
        return false;
    }

    CreateFromFile();
    return true;
}

const GLenum DATA_FORMATS[] = {
    GL_INVALID_ENUM, // None
    GL_FLOAT,        // Float
    GL_FLOAT,        // Float2
    GL_FLOAT,        // Float3
    GL_FLOAT,        // Float4
    GL_INT,          // Int
    GL_INT,          // Int2
    GL_INT,          // Int3
    GL_INT,          // Int4
    GL_INVALID_ENUM  // Mat4
};

void GLShader::SetLayout(DataLayout& layout) {
    // If our new layout is shorter than our old layout, disable the excess elements
    if (this->layout.Count() > layout.Count()) {
        for (int i = layout.Count(); i < this->layout.Count(); i++) {
            glDisableVertexAttribArray(i);
        }
    }

    // Set new layout
    this->layout.Delete();

    Slice<DataElement> elements = layout.GetElements();
    
    int stride = layout.GetStride();
    for (int i = 0; i < elements.Count(); i++) {
        DataElement* e = &elements.Data()[i];

        this->layout.Add(e->name, e->type);

        glEnableVertexAttribArray(i);
        glVertexAttribPointer(i, DataLayoutComponentCount[(int)e->type],
                              DATA_FORMATS[(int)e->type], GL_FALSE, stride, (void*)(u64)e->offset);
    }

    this->layout.Finish();
}

DataLayout& GLShader::GetLayout() {
    return layout;
}

void GLShader::Bind() {
    glUseProgram(programID);
    ActivateLayout();
}

void GLShader::ActivateTextureSlot(int slot) {
    // Set the sampler called "tex0..PD_MAX_TEXTURE_SLOTS" to the slot
    BoundedString<8> samplerName;
    samplerName.Format("tex{}", slot);

    SetUniform(samplerName, slot);
}

Slice<byte> GLShader::GetVertexData() {
    return vertexFile.View().ToSlice();
}

Slice<byte> GLShader::GetPixelData() {
    return pixelFile.View().ToSlice();
}

int GLShader::GetLocation(StringView name) {
    Bind();

    int location = glGetUniformLocation(programID, name.CStr());

    if (location == GL_INVALID_INDEX) {
        CONSOLE_LOG_DEBUG("[{}OpenGL{}] Uniform '{}' not found\n", ConColor::Red, ConColor::White, name);
    }

    return location;
}

void GLShader::SetUniform(StringView name, bool data) {
    int location = GetLocation(name);
    if (location == GL_INVALID_INDEX) return;

    glUniform1i(location, (data) ? 1 : 0);
}

void GLShader::SetUniform(StringView name, f32 data) {
    int location = GetLocation(name);
    if (location == GL_INVALID_INDEX) return;

    glUniform1f(location, data);
}

void GLShader::SetUniform(StringView name, Vec2 data) {
    int location = GetLocation(name);
    if (location == GL_INVALID_INDEX) return;

    glUniform2f(location, data.x, data.y);
}

void GLShader::SetUniform(StringView name, Vec3 data) {
    int location = GetLocation(name);
    if (location == GL_INVALID_INDEX) return;

    glUniform3f(location, data.x, data.y, data.z);
}

void GLShader::SetUniform(StringView name, Vec4 data) {
    int location = GetLocation(name);
    if (location == GL_INVALID_INDEX) return;

    glUniform4f(location, data.x, data.y, data.z, data.w);
}

void GLShader::SetUniform(StringView name, int data) {
    int location = GetLocation(name);
    if (location == GL_INVALID_INDEX) return;

    glUniform1i(location, data);
}

void GLShader::SetUniform(StringView name, Vec2i data) {
    int location = GetLocation(name);
    if (location == GL_INVALID_INDEX) return;

    glUniform2i(location, data.x, data.y);
}

void GLShader::SetUniform(StringView name, Vec3i data) {
    int location = GetLocation(name);
    if (location == GL_INVALID_INDEX) return;

    glUniform3i(location, data.x, data.y, data.z);
}

void GLShader::SetUniform(StringView name, Vec4i data) {
    int location = GetLocation(name);
    if (location == GL_INVALID_INDEX) return;

    glUniform4i(location, data.x, data.y, data.z, data.w);
}

void GLShader::SetUniform(StringView name, Mat4 data) {
    int location = GetLocation(name);
    if (location == GL_INVALID_INDEX) return;

    glUniformMatrix4fv(location, 1, GL_FALSE, &data[0][0]);
}

void GLShader::CreateFromFile() {
    // Hash each source and then hash THAT to get our final hash
    u64 hashes[2];

    hashes[0] = DoHash(vertexFile);
    hashes[1] = DoHash(pixelFile);

    hash = DoHash(hashes, sizeof(hashes));

    vertexID = glCreateShader(GL_VERTEX_SHADER);
    pixelID = glCreateShader(GL_FRAGMENT_SHADER);

    PD_ASSERT_D(vertexID != 0 && pixelID != 0, "failed to create vertex/pixel shader (%d, %d)", vertexID, pixelID);

    const char* vertexFilePtr = vertexFile.CStr();
    glShaderSource(vertexID, 1, &vertexFilePtr, nullptr);
    glCompileShader(vertexID);

    // Check Parse status
    GLint success = 0;
    glGetShaderiv(vertexID, GL_COMPILE_STATUS, &success);
    if (success == GL_FALSE) {
        GLint len;
        glGetShaderiv(vertexID, GL_INFO_LOG_LENGTH, &len);

        char* infoLog = (char*)Alloc((u64)len + 1, Allocator::Temporary);
        glGetShaderInfoLog(vertexID, len, nullptr, infoLog);
        infoLog[len] = '\0';

        PD_ASSERT_D(false, "Failed to compile vertex shader.\nInfo log:\n%s", infoLog);
    }

    const char* pixelFilePtr = pixelFile.CStr();
    glShaderSource(pixelID, 1, &pixelFilePtr, nullptr);
    glCompileShader(pixelID);

    // Check Parse status
    glGetShaderiv(pixelID, GL_COMPILE_STATUS, &success);
    if (success == GL_FALSE) {
        GLint len;
        glGetShaderiv(pixelID, GL_INFO_LOG_LENGTH, &len);

        char* infoLog = (char*)Alloc((u64)len + 1, Allocator::Temporary);
        glGetShaderInfoLog(pixelID, len, nullptr, infoLog);
        infoLog[len] = '\0';

        PD_ASSERT_D(false, "Failed to compile pixel shader.\nInfo log:\n%s", infoLog);
    }

    // Generate program
    programID = glCreateProgram();

    glAttachShader(programID, vertexID);
    glAttachShader(programID, pixelID);

    glLinkProgram(programID);

    glGetProgramiv(programID, GL_LINK_STATUS, &success);
    if (success == GL_FALSE) {
        GLint len;
        glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &len);

        char* infoLog = (char*)Alloc((u64)len + 1, Allocator::Temporary);
        glGetProgramInfoLog(programID, len, nullptr, infoLog);
        infoLog[len] = '\0';

        PD_ASSERT_D(false, "Failed to link shaders.\nInfo log:\n%s", infoLog);
    }

    // Detach and delete
    glDetachShader(programID, vertexID);
    glDetachShader(programID, pixelID);

    glDeleteShader(vertexID);
    glDeleteShader(pixelID);

#if !defined(PD_DEBUG)
    // Not used anymore so just set them to zero
    vertexID = 0;
    pixelID = 0;
#endif
}

void GLShader::ActivateLayout() {
    Slice<DataElement> elements = layout.GetElements();

    int stride = layout.GetStride();
    for (int i = 0; i < elements.Count(); i++) {
        DataElement* e = &elements.Data()[i];

        glEnableVertexAttribArray(i);
        glVertexAttribPointer(i, DataLayoutComponentCount[(int)e->type],
            DATA_FORMATS[(int)e->type], GL_FALSE, stride, (void*)(u64)e->offset);
    }

}

}

#endif
