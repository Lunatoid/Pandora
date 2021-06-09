#include "FTFont.h"

#include "Pandora/Core/IO/Console.h"
#include "Pandora/Core/IO/File.h"

#include "Pandora/Graphics/VideoAPI.h"

#include <freetype/ftlcdfil.h>

// @GLOBAL
static bool initializedFreeType = false;
static FT_Library ft;

namespace pd {

FTFont::FTFont() {
    if (!initializedFreeType) {
        initializedFreeType = true;
        FT_Init_FreeType(&ft);
        FT_Library_SetLcdFilter(ft, FT_LCD_FILTER_DEFAULT);

        int major, minor, patch;
        FT_Library_Version(ft, &major, &minor, &patch);

        CONSOLE_LOG_DEBUG("  {}> FreeType {}.{}.{}{}\n", ConColor::Grey, major, minor, patch, ConColor::White);
    }
}

FTFont::~FTFont() {
    FT_Done_Face(face);
}

bool FTFont::Load(StringView path) {
    if (ReadEntireFile(path, fontMemory) == 0) {
        return false;
    }

    return LoadFromMemory();
}

bool FTFont::Load(Box& box, StringView name) {
    if (!box.HasResource(name)) return false;

    ResourceType type = box.GetResourceType(name);

    if (!(type == ResourceType::Binary || type == ResourceType::Font)) {
        // We cannot Load this resource as a font
        return false;
    }

    switch (type) {
        // Right now fonts are stored as their binary part
        case ResourceType::Font:
        case ResourceType::Binary: {
            box.GetResourceData(name, fontMemory);
            if (!LoadFromMemory()) {
                return false;
            }

            return true;
        }

        default:
            return false;
    }

    return false;
}

Glyph* FTFont::GetGlyph(codepoint point) {
    if (!glyphs.Contains(point)) {
        u32 glyphIndex = FT_Get_Char_Index(face, point);

        if (FT_Load_Glyph(face, glyphIndex, FT_LOAD_DEFAULT | FT_LOAD_COLOR) != 0) {
            return nullptr;
        }

        if (FT_Render_Glyph(face->glyph, FT_RENDER_MODE_LCD) != 0) {
            return nullptr;
        }

        Glyph glyph;

        Vec2i size = Vec2i(face->glyph->bitmap.width, face->glyph->bitmap.rows);

        Color* colors = nullptr;
        if (size.x * size.y > 0) {
            colors = (Color*)Alloc((u64)size.x * size.y * sizeof(Color));
            MemorySet(colors, (u64)size.x * size.y * sizeof(Color), 0);

            switch (face->glyph->bitmap.pixel_mode) {
                case FT_PIXEL_MODE_LCD: {
                    // Width is 3x the size in LCD mode
                    size.x /= 3;
                    
                    int colorY = size.y - 1;
                    for (int y = 0; y < size.y; y++) {
                        for (int x = 0; x < size.x * 3; x += 3) {
                            int i = y * face->glyph->bitmap.pitch + x;

                            f32 r = face->glyph->bitmap.buffer[i + 0] / 255.0f;
                            f32 g = face->glyph->bitmap.buffer[i + 1] / 255.0f;
                            f32 b = face->glyph->bitmap.buffer[i + 2] / 255.0f;

                            colors[colorY * size.x + (x / 3)] = Color(r, g, b, (r + g + b) / 3.0f);
                        }
                        colorY -= 1;
                    }
                    break;
                }

                case FT_PIXEL_MODE_BGRA: {
                    int colorY = size.y - 1;
                    for (int y = 0; y < size.y; y++) {
                        for (int x = 0; x < size.x * 4; x += 4) {
                            int i = y * face->glyph->bitmap.pitch + x;

                            f32 b = face->glyph->bitmap.buffer[i + 0] / 255.0f;
                            f32 g = face->glyph->bitmap.buffer[i + 1] / 255.0f;
                            f32 r = face->glyph->bitmap.buffer[i + 2] / 255.0f;
                            f32 a = face->glyph->bitmap.buffer[i + 3] / 255.0f;

                            colors[colorY * size.x + (x / 4)] = Color(r, g, b, a);
                        }
                        colorY -= 1;
                    }

                    glyph.hasColor = true;
                    break;
                }

                default: {
                    CONSOLE_LOG_DEBUG("[{}FreeType Error{}] unhandled pixel mode '{}'",
                        ConColor::Red, ConColor::Grey, face->glyph->bitmap.pixel_mode);
                    break;
                }
            }
        }

        PackGlyph(Slice<Color>(colors, size.x * size.y), size.x, glyph);
        glyph.size = size;
        glyph.bearing = Vec2((f32)face->glyph->metrics.horiBearingX, (f32)face->glyph->metrics.horiBearingY) / 64.0f;
        glyph.advance = (f32)face->glyph->metrics.horiAdvance / 64.0f;

        Free(colors);

        glyphs.Set(point, glyph);
    }

    return &glyphs.Get(point);
}

Vec2 FTFont::GetKerning(codepoint left, codepoint right) {
    if (!FT_HAS_KERNING(face)) return Vec2(0.0f);

    u32 leftIndex = FT_Get_Char_Index(face, left);
    u32 rightIndex = FT_Get_Char_Index(face, right);

    FT_Vector out;

    if (FT_Get_Kerning(face, leftIndex, rightIndex, FT_KERNING_UNFITTED, &out) != 0) {
        return Vec2(0.0f);
    }

    return Vec2((f32)out.x, (f32)out.y) / 64.0f;
}

f32 FTFont::GetAscender() {
    if (face->units_per_EM > 0) {
        return face->ascender * (f32)pixelHeight / face->units_per_EM;
    } else {
        return face->size->metrics.ascender / 64.0f;
    }
}

f32 FTFont::GetHeight() {
    if (face->height > 0 && face->units_per_EM > 0) {
        return face->height * f32(pixelHeight) / face->units_per_EM;
    } else {
        return face->size->metrics.height / 64.0f;
    }
}

bool FTFont::LoadFromMemory() {
    hash = DoHash(fontMemory.Slice());

    if (FT_New_Memory_Face(ft, fontMemory.Data(), (FT_Long)fontMemory.SizeInBytes(), 0, &face) != 0) {
        return false;
    }

    FT_Select_Charmap(face, FT_ENCODING_UNICODE);
    FT_Set_Pixel_Sizes(face, 0, pixelHeight);
    return true;
}

}
