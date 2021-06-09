#pragma once

#include "Pandora/Graphics/Text/Font.h"

#define FT_CONFIG_OPTION_SUBPIXEL_RENDERING
#include <ft2build.h>
#include FT_FREETYPE_H

namespace pd {

class FTFont final : public Font {
public:
    FTFont();
    ~FTFont();

    virtual bool Load(StringView path) override;
    virtual bool Load(Box& box, StringView name) override;

    virtual Glyph* GetGlyph(codepoint point) override;
    virtual Vec2 GetKerning(codepoint left, codepoint right) override;

    virtual f32 GetAscender() override;
    virtual f32 GetHeight() override;

private:
    bool LoadFromMemory();

    // Where the font gets loaded into, must stay alive until font gets freed
    Array<byte> fontMemory;
    FT_Face face = nullptr;

    // Should probably be in Font
    u32 pixelHeight = 128;
};

}
