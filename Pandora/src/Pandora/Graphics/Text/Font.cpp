#include "Font.h"

namespace pd {

const int FONT_PAGE_SIZE = 512;
const int FONT_PADDING = 8;

Ref<Texture> Font::GetGlyphTexture(const Glyph& g, RefType type) {
    return packed[g.index].GetTexture(type);
}

void Font::PackGlyph(Slice<Color> pixels, int stride, Glyph& glyph) {
    for (int i = 0; i < packed.Count(); i++) {
        if (packed[i].PackTexture(pixels, stride, glyph.uv)) {
            glyph.index = i;
            return;
        }
    }

    // If we got here that means nothing got packed, create a new page
    packed.Add(FONT_PAGE_SIZE, FONT_PADDING, filtering);
    if (packed.Last().PackTexture(pixels, stride, glyph.uv)) {
        glyph.index = packed.Count() - 1;
        return;
    }

    PD_ASSERT_D(false, "Somehow the glyph could not be packed");
}

}
