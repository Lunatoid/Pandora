#pragma once

#include "Pandora/Core/Math/Matrix.h"
#include "Pandora/Core/Math/VectorInt.h"

#include "Pandora/Graphics/DataLayout.h"
#include "Pandora/Graphics/Shader.h"

namespace pd {

class ConstantBuffer {
public:
    virtual ~ConstantBuffer() = default;
    
    /// <summary>
    /// Sets the bind target if applicable.
    /// </summary>
    /// <param name="type">The type.</param>
    virtual void SetBindTarget(ShaderType type) = 0;
    
    /// <summary>
    /// Sets the bind slot if applicable.
    /// </summary>
    /// <param name="slot">The slot.</param>
    virtual void SetBindSlot(int slot) = 0;

    void SetLayout(DataLayout& layout);
    
    /// <summary>
    /// Sets the entire layout structure.
    /// Make sure all elements are aligned to 4-byte boundaries.
    /// </summary>
    /// <param name="bytes">The bytes of the layout. All elements must be aligned to 4-byte boundaries.</param>
    virtual void SetStruct(Slice<byte> bytes) = 0;
    
    virtual void SetElement(StringView name, bool data) = 0;
    virtual void SetElement(StringView name, f32 data) = 0;
    virtual void SetElement(StringView name, Vec2 data) = 0;
    virtual void SetElement(StringView name, Vec3 data) = 0;
    virtual void SetElement(StringView name, Vec4 data) = 0;
    virtual void SetElement(StringView name, int data) = 0;
    virtual void SetElement(StringView name, Vec2i data) = 0;
    virtual void SetElement(StringView name, Vec3i data) = 0;
    virtual void SetElement(StringView name, Vec4i data) = 0;
    virtual void SetElement(StringView name, Mat4 data) = 0;

    virtual void Upload() = 0;

protected:
    DataLayout layout = DataLayout(true);
};

}
