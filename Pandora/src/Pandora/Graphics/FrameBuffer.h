#pragma once

#include "Pandora/Core/Data/Reference.h"

#include "Pandora/Core/Math/Color.h"
#include "Pandora/Core/Math/VectorInt.h"

#include "Pandora/Graphics/Texture.h"

namespace pd {

class FrameBuffer {
public:
    virtual ~FrameBuffer() = default;
    
    virtual void Create(Vec2i size) = 0;
    virtual void Delete() = 0;

    virtual void Bind() = 0;

    /// <summary>
    /// Binds the frame buffer and clears it to the correct color.
    /// </summary>
    /// <param name="clearColor">The clear color.</param>
    virtual void BindAndClear(Color clearColor = Color(0.0f, 0.0f)) = 0;

    Ref<Texture> GetTexture(RefType type = RefType::Strong);

protected:
    Ref<Texture> colorBuffer;
    Ref<Texture> depthStencilBuffer;
};

}
