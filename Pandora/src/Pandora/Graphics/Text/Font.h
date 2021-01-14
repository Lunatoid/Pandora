#pragma once

#include "Pandora/Core/Data/Reference.h"
#include "Pandora/Core/Data/Dictionary.h"
#include "Pandora/Core/Resources/Resource.h"

#include "Pandora/Graphics/TexturePacker.h"

namespace pd {

struct Glyph {
    Vec2 bearing;
    f32 advance = 0.0f;
    bool hasColor = false;
    Vec2i size;
    Vec4 uv;
    int index;
};

class Font : public Resource {
public:
    static ResourceType GetType() { return ResourceType::Font; }
    
    virtual ~Font() = default;

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
    /// The hash is calculated on Load.
    /// </summary>
    /// <returns>The hash.</returns>
    virtual u64 GetHash() { return hash; }

    /// <summary>
    /// Gets the texture for the corresponding glyph.
    /// </summary>
    /// <param name="g">The glyph.</param>
    /// <param name="type">The reference type.</param>
    /// <returns>A reference of the desired type to the texture.</returns>
    Ref<Texture> GetGlyphTexture(const Glyph& g, RefType type = RefType::Strong);

    // @TODO: add to box config
    TextureFiltering filtering = TextureFiltering::Anisotropic;

protected:
    void PackGlyph(Slice<Color> pixels, int stride, Glyph& glyph);

    Dictionary<codepoint, Glyph> glyphs;
    Array<TexturePacker> packed;
};

}
