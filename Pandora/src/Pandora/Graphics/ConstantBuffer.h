#pragma once

#include "Pandora/Core/Math/Matrix.h"
#include "Pandora/Core/Math/VectorInt.h"

#include "Pandora/Graphics/DataLayout.h"
#include "Pandora/Graphics/Shader.h"

namespace pd {

class ConstantBuffer {
public:
    virtual ~ConstantBuffer() = default;

    /**
     * \brief Sets the bind target.
     * May not be necessary for all backends.
     * 
     * \param type The shader type to bind to.
     */
    virtual void SetBindTarget(ShaderType type) = 0;

    /**
     * \brief Sets the bind slot.
     * May not be necessary for all backends.
     * 
     * \param slot The slot to bind to.
     */
    virtual void SetBindSlot(int slot) = 0;

    /**
     * \brief Sets the layout of the buffer.
     * 
     * \param layout The layout.
     */
    void SetLayout(DataLayout& layout);

    /**
     * \brief Sets the entire layout structure.
     * Make sure all elements are aligned to 4-byte boundaries, as it is required for some backends.
     * 
     * \param bytes The raw structure bytes.
     */
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

    /**
     * \brief Uploads the buffer to the GPU.
     * 
     */
    virtual void Upload() = 0;

protected:
    DataLayout layout;
};

}
