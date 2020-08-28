#pragma once

#include "Pandora/Core/Data/Reference.h"
#include "Pandora/Core/Data/Dictionary.h"

#include "Pandora/Core/Resources/Resource.h"

#include "Pandora/Graphics/Texture.h"

namespace pd {

struct Glyph {
    Ref<Texture> texture;
    Vec2 bearing;
    f32 advance = 0.0f;
    bool hasColor = false;
};

class Font : public Resource {
public:
    static ResourceType GetType() { return ResourceType::Font; }
    
    /// <summary>
    /// Loads the font from a file.
    /// </summary>
    /// <param name="path">The path.</param>
    /// <returns>Whether or not it loaded successfully.</returns>
    virtual bool Load(StringView path) = 0;
    
    /// <summary>
    /// Loads the font from a box.
    /// </summary>
    /// <param name="box">The box.</param>
    /// <param name="name">The resource name.</param>
    /// <returns>Whether or not it loaded successfully.</returns>
    virtual bool Load(Box& box, StringView name) = 0;
    
    /// <summary>
    /// Gets the glyph for the specified codepoint.
    /// </summary>
    /// <param name="point">The codepoint.</param>
    /// <returns>The glyph.</returns>
    virtual Glyph* GetGlyph(codepoint point) = 0;
    
    /// <summary>
    /// Gets the kerning between two codepoints.
    /// </summary>
    /// <param name="left">The left codepoint.</param>
    /// <param name="right">The right codepoint.</param>
    /// <returns>The kerning.</returns>
    virtual Vec2 GetKerning(codepoint left, codepoint right) = 0;
    
    /// <summary>
    /// Gets the font ascender.
    /// </summary>
    /// <returns>The font ascender.</returns>
    virtual f32 GetAscender() = 0;
    
    /// <summary>
    /// Gets the font height.
    /// </summary>
    /// <returns>The font height.</returns>
    virtual f32 GetHeight() = 0;
    
    /// <summary>
    /// Gets the hash of the font file.
    /// The hash is calculated on load.
    /// </summary>
    /// <returns>The hash.</returns>
    virtual u64 GetHash() { return hash; }

protected:
    // @TODO: convert this to TextureFiltering and add this to the box import settings.
    bool filtering = true;
    Dictionary<codepoint, Glyph> glyphs;
};

}
