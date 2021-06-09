#pragma once

#include "Pandora/Graphics/Text/Font.h"
#include "Pandora/Graphics/Sprite/SpriteRenderer.h"

namespace pd {

class Text {
public:
    Text();

    /**
     * \brief Loads the text.
     * 
     * \param fontName The font resource name.
     * \param spriteMat The shader resource name for the letter sprites.
     */
    void Load(StringView fontName, StringView spriteMat = "Shaders/Sprite");

    /**
     * \brief Draws the text.
     * 
     * \param renderer The renderer to use.
     */
    void Draw(SpriteRenderer& renderer);

    /**
     * \brief Generates the sprites for the letters.
     */
    void GenerateSprites();

    /**
     * \brief Updates the sprite properties.
     */
    void UpdateProperties();

    /**
     * \return The bounds of the text.
     */
    Vec2 CalculateBounds();

    String text;
    Vec3 position;
    Color color = Color(1.0f);
    Vec2 scale = Vec2(1.0f);

private:
    // @TODO: @FIXME: destructing this without ever drawing it results in an access violation
    // in some of the Ref<Texture>
    struct Letter {
        codepoint point = '\0';
        Sprite sprite;
    };

    ResourceCatalog& catalog;
    Ref<Font> font;
    Array<Letter> letters;
    String spriteMat;
};

}
