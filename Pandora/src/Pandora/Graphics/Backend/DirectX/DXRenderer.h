#if !defined(PD_NO_DIRECTX)
#pragma once

#include <d3d11.h>

#include "Pandora/Graphics/Renderer.h"

#include "Pandora/Graphics/Backend/DirectX/DXVideoAPI.h"
#include "Pandora/Graphics/Backend/DirectX/DXBuffer.h"

namespace pd {

class DXRenderer final : public Renderer {
public:
    DXRenderer();
    
    virtual void Submit(int indexOffset = 0, int indexCount = -1, bool clearData = true) override;

private:
    DXBuffer vertexBuffer;
    DXBuffer indexBuffer;
};

}

#endif
