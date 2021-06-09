#include "FrameBuffer.h"

namespace pd {

Ref<Texture> FrameBuffer::GetTexture(RefType type) {
    return colorBuffer.NewRef(type);
}

}
