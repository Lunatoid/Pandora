#pragma once

#include "Pandora/Core/Data/Reference.h"
#include "Pandora/Core/Data/Dictionary.h"
#include "Pandora/Core/Resources/Resource.h"

#include "Pandora/Graphics/TexturePacker.h"

namespace pd {

struct Glyph {
    /**
     * \brief The glyph bearing.
     */
    Vec2 bearing;

    /**
     * \brief The glyph advance.
     */
    f32 advance = 0.0f;

    /**
     * \brief Whether or not the glyph is monochrome.
     */
    bool hasColor = false;

    /**
     * \brief The size of the glyph texture.
     */
    Vec2i size;

    /**
     * \brief The UV within the page.
     */
    Vec4 uv;

    /**
     * \brief The page index.
     */
    int index;
};

class Font : public Resource {
public:
    static ResourceType GetType() { return ResourceType::Font; }

    virtual ~Font() = default;

    /**
     * \brief Loads the font from a file.
     * 
     * \param path The path to the file.
     * \return Whether or not it loaded successfully.
     */
    virtual bool Load(StringView path) = 0;

    /**
     * \brief Loads the font from a box.
     * 
     * \param box The box.
     * \param name The font resource name.
     * \return Whether or not it loaded successfully.
     */
    virtual bool Load(Box& box, StringView name) = 0;

    // @TODO: the Glyph* can be changed to just Glyph.

    /**
     * \brief Gets the glyph for the specified codepoint.
     * 
     * \param point The codepoint.
     * \return A pointer to the glyph info.
     */
    virtual Glyph* GetGlyph(codepoint point) = 0;

    /**
     * \brief Gets the kerning between two codepoints.
     * 
     * \param left The left codepoint.
     * \param right The right codepoint.
     * \return The kerning.
     */
    virtual Vec2 GetKerning(codepoint left, codepoint right) = 0;

    /**
     * \return The font ascender.
     */
    virtual f32 GetAscender() = 0;

    /**
     * \return The font height.
     */
    virtual f32 GetHeight() = 0;

    /**
     * \return The hash of the raw font data.
     * Gets calculated on `Load()`.
     */
    virtual u64 GetHash() { return hash; }

    /**
     * \brief Gets the texture for the corresponding glyph.
     * 
     * \param g The glyph.
     * \param type The reference type.
     * \return A new reference to the glyph texture.
     */
    Ref<Texture> GetGlyphTexture(const Glyph& g, RefType type = RefType::Strong);

    // @TODO: add to box config
    TextureFiltering filtering = TextureFiltering::Anisotropic;

protected:

    /**
     * \brief Packs the glyph texture.
     * 
     * \param pixels The glyph pixels.
     * \param stride The glyph width.
     * \param glyph The output glyph.
     */
    void PackGlyph(Slice<Color> pixels, int stride, Glyph& glyph);

    Dictionary<codepoint, Glyph> glyphs;

    /**
     * \brief All the glyph pages.
     * Once a texture packer becomes full, it will create a new page.
     */
    Array<TexturePacker> packed;
};

}
