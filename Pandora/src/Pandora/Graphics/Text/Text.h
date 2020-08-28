#pragma once

#include "Pandora/Graphics/Text/Font.h"

#include "Pandora/Graphics/Sprite/SpriteRenderer.h"

namespace pd {

class Text {
public:
    Text();
    
    /// <summary>
    /// Loads the text.
    /// </summary>
    /// <param name="fontName">Name of the font.</param>
    /// <param name="spriteMat">The sprite material name.</param>
    void Load(StringView fontName, StringView spriteMat = "Shaders/Sprite");
    
    /// <summary>
    /// Draws the text.
    /// </summary>
    /// <param name="renderer">The renderer.</param>
    void Draw(SpriteRenderer& renderer);
    
    /// <summary>
    /// Generates the sprites.
    /// </summary>
    void GenerateSprites();
    
    /// <summary>
    /// Updates the properties.
    /// </summary>
    void UpdateProperties();
        
    /// <summary>
    /// Calculates the bounds of the text.
    /// </summary>
    /// <returns>Teh boudns of the text.</returns>
    Vec2 CalculateBounds();

    String text;
    Vec3 position;
    Color color = Color(1.0f);
    Vec2 scale = Vec2(1.0f);

private:
    // @TODO: @FIXME: destructing this without ever drawing it results in an access violation in some of the Ref<Texture>
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
