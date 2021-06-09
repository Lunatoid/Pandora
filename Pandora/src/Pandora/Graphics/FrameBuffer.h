#pragma once

#include "Pandora/Core/Data/Reference.h"

#include "Pandora/Core/Math/Color.h"
#include "Pandora/Core/Math/VectorInt.h"

#include "Pandora/Graphics/Texture.h"

namespace pd {

class FrameBuffer {
public:
    virtual ~FrameBuffer() = default;

    /**
     * \brief Creates a new frame buffer.
     * 
     * \param size The size of the frame buffer.
     */
    virtual void Create(Vec2i size) = 0;

    /**
     * \brief Deletes the frame buffer. Gets called on destruction.
     */
    virtual void Delete() = 0;

    /**
     * \brief Binds the frame buffer.
     */
    virtual void Bind() = 0;

    /**
     * \brief Binds the frame buffer and clears it with the specified color.
     * 
     * \param clearColor What color to clear the frame buffer with.
     */
    virtual void BindAndClear(Color clearColor = Color(0.0f, 0.0f)) = 0;

    /**
     * \brief Creates a new reference to the frame buffer.
     * 
     * \param type The reference type.
     * \return The new reference.
     */
    Ref<Texture> GetTexture(RefType type = RefType::Strong);

protected:
    Ref<Texture> colorBuffer;
    Ref<Texture> depthStencilBuffer;
};

}
