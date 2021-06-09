#include "Text.h"

namespace pd {

// @TODO: BBCode style tags (bold, color, effects, icons)
// @TODO: proper layout engine so we can support RTL, Arabic and more

Text::Text() : catalog(ResourceCatalog::Get()) {}

void Text::Load(StringView fontName, StringView spriteMat) {
    font = catalog.Get<Font>(fontName);
    this->spriteMat.Set(spriteMat);
}

void Text::Draw(SpriteRenderer& renderer) {
    for (int i = 0; i < letters.Count(); i++) {
        // Only disallowing newlines seems really hacky
        if (letters[i].point != '\n') {
            renderer.Draw(letters[i].sprite);
        }
    }
}

void Text::GenerateSprites() {
    letters.Clear();

    for (int i = 0; i < text.Count(); i++) {
        codepoint point = text[i];
        Glyph* g = font->GetGlyph(point);

        if (!g) continue;

        letters.Reserve(1);
        letters.Last().point = point;

        letters.Last().sprite.Load(font->GetGlyphTexture(*g), spriteMat);
        letters.Last().sprite.size = g->size;
        letters.Last().sprite.SetNormalizedClippingMask(g->uv);
    }

    UpdateProperties();
}

void Text::UpdateProperties() {
    Vec3 penPos = position + Vec3(0.0f, font->GetAscender() * scale.y, 0.0f);

    for (int i = 0; i < letters.Count(); i++) {
        // Handle newlines
        if (letters[i].point == '\n') {
            penPos.y -= font->GetHeight() * scale.y;
            penPos.x = position.x;
            continue;
        }

        // Apply kerning
        Vec2 kerning;
        if (i + 1 < letters.Count()) {
            kerning = font->GetKerning(letters[i].point, letters[i + 1].point);
            penPos.x += kerning.x * scale.x;
        }

        Glyph* g = font->GetGlyph(letters[i].point);

        if (!g) continue;

        // Set position and scale, only set color if the glyph is grayscale
        f32 xOffset = g->bearing.x * scale.x;
        f32 yOffset = ((g->bearing.y - (f32)g->size.y) + kerning.y) * scale.y;

        letters[i].sprite.position = penPos + Vec2(xOffset, yOffset);
        letters[i].sprite.scale = scale;

        if (!g->hasColor) {
            letters[i].sprite.color = color;
        }

        penPos.x += g->advance * scale.x;
    }
}

Vec2 Text::CalculateBounds() {
    Vec2 penPos = Vec2(0.0f, font->GetAscender() * scale.y);
    f32 xMax = 0.0f;

    for (int i = 0; i < text.Count(); ++i) {
        if (text[i] == '\n') {
            penPos.y += f32(font->GetHeight()) * scale.y;

            if (penPos.x > xMax) {
                xMax = penPos.x;
            }

            penPos.x = position.x;
            continue;
        }

        // Add kerning
        Vec2 kerning = Vec2(0.0f);
        if (i + 1 < text.Count()) {
            kerning = font->GetKerning(text[i], text[i + 1]);
            kerning.x += kerning.x * scale.x;
        }

        Glyph* g = font->GetGlyph(text[i]);

        if (!g) continue;

        penPos.x += g->advance * scale.x;
    }

    if (penPos.x < xMax) {
        penPos.x = xMax;
    }

    return penPos;
}

}
