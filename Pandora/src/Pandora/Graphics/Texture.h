#pragma once

#include "Pandora/Core/Math/Color.h"
#include "Pandora/Core/Math/VectorInt.h"
#include "Pandora/Core/Data/Reference.h"
#include "Pandora/Core/Resources/Resource.h"

#if !defined(PD_NO_IMGUI)
#include "Pandora/Libs/ImGui/imgui.h"
#endif

// 16 is probably a decent amount
#define PD_MAX_TEXTURE_UNITS 16

namespace pd {

enum class TextureExportFormat : byte {
    PNG,
    JPG,
    BMP,
    TGA
};

enum class TextureWrapping : byte {
    Clamp,
    Repeat,
    Count
};

const StringView TEXTURE_WRAPPING_NAMES[] = {
    "Clamp",
    "Repeat"
};

enum class TextureFiltering : byte {
    Point,
    Bilinear,
    Trilinear,
    Anisotropic,
    Count
};

const StringView TEXTURE_FILTERING_NAMES[] = {
    "Point",
    "Bilinear",
    "Trilinear",
    "Anisotropic"
};

class Texture : public Resource {
public:
    static ResourceType GetType() { return ResourceType::Texture; }

    virtual ~Texture() = default;

    /**
     * \brief Loads the texture from the specified path.
     * 
     * \param path The path to the file.
     * \return Whether or not it loaded successfully.
     */
    virtual bool Load(StringView path) override;

    /**
     * \brief Loads the texture from the box.
     * 
     * \param box The box.
     * \param name The texture resource name.
     * \return Whether or not it loaded successfully.
     */
    virtual bool Load(Box& box, StringView name) override;

    /**
     * \brief Creates a blank texture with the specified size and format.
     * 
     * \param size The size.
     * \param white If true, the texture will be white, if false, the texture will be transparent.
     */
    void Create(Vec2i size, bool white = true);

    /**
     * \brief Creates a texture from the specified RGBA pixels.
     * 
     * \param pixels The pixel data in RGBA format.
     * \param stride The stride of each row in pixels.
     */
    void Create(Slice<byte> pixels, int stride);

    /**
     * \brief Creates a texture from the specified RGBA pixels.
     * 
     * \param pixels The pixel colors.
     * \param stride The stride of each row in pixels.
     */
    void Create(Slice<Color> pixels, int stride);

    /**
     * \brief Deletes the texture data. Gets called on destruction.
     * 
     */
    virtual void Delete() = 0;

    /**
     * \brief Uploads the pixel data to the GPU.
     * 
     * \param releasePixelData When true, it will release the pixel memory.
     */
    virtual void Upload(bool releasePixelData = true) = 0;

    /**
     * \brief Releases the pixel data.
     */
    void ReleasePixelData();

    /**
     * \brief Binds the texture to the specified slot.
     * 
     * \param textureSlot The slot.
     */
    virtual void Bind(int textureSlot = 0) = 0;

    /**
     * \brief Exports the pixel data to a file.
     * Pixel data must not have been released.
     * 
     * \param path The export path.
     * \param exportFormat The export format.
     */
    void Export(StringView path, TextureExportFormat exportFormat = TextureExportFormat::PNG);

    /**
     * \brief Gets the pixel color at the position.
     * Pixel data must not have been released.
     * 
     * \param position The pixel position.
     * \return The color of the pixel.
     */
    Color GetPixel(Vec2i position);

    /**
     * \brief Sets the pixel color at the specified position.
     * Pixel data must not have been released.
     * 
     * \param position The position.
     * \param color The color.
     */
    void SetPixel(Vec2i position, Color color);

    /**
     * \brief Writes a texture to a certain position.
     * Pixel data must not have been released for either textures.
     * 
     * \param position The position.
     * \param texture The texture.
     */
    void SetPixels(Vec2i position, const Ref<Texture>& texture);

    /**
     * \brief Writes a texture to a certain position.
     * Pixel data must not have been released.
     * 
     * \param position The position.
     * \param pixels The pixels in RGBA format.
     * \param stride The stride in pixels.
     */
    void SetPixels(Vec2i position, Slice<byte> pixels, int stride);

    /**
     * \brief Writes a texture to a certain position.
     * Pixel data must not have been released.
     * 
     * \param position The position.
     * \param pixels The pixels in RGBA format.
     * \param stride The stride in pixels.
     */
    void SetPixels(Vec2i position, Slice<Color> pixels, int stride);

    /**
     * \return The size in pixels.
     */
    Vec2i GetSize() const;

    /**
     * \return The raw pixel data.
     * Will return nullptr if the pixel data has been released.
     */
    byte* Data() const;

    /**
     * \return The texture hash.
     * Gets calculated on upload.
     */
    u64 GetHash() const;

    /**
     * \return Whether or not this texture is upside down and should be flipped vertically.
     */
    virtual bool ShouldFlipVertically();

#if !defined(PD_NO_IMGUI)
    /**
     * \return The ImGui texture ID.
     */
    virtual ImTextureID GetImGuiTextureID() = 0;
#endif

    TextureFiltering filtering = TextureFiltering::Bilinear;
    TextureWrapping wrapping = TextureWrapping::Clamp;
protected:

    /**
     * \brief Loads the pixel data from a file.
     * 
     * \param path The file.
     * \return Whether or not it loaded successfully.
     */
    bool LoadPixelsFromFile(StringView path);

    /**
     * \brief Loads the pixel data from memory.
     * 
     * \param data The RGBA pixel data.
     * \return Whether or not it loaded successfully.
     */
    bool LoadPixelsFromMemory(Slice<byte> data);

    /**
     * \brief Creates a blank texture.
     * 
     * \param size The size of the texture.
     * \param value The value for each subpixel.
     */
    void CreateBlankPixels(Vec2i size, byte value);

    /**
     * \brief Creates the backend-specific texture data.
     */
    virtual void CreateTextureData() = 0;

    byte* pixels = nullptr;

    Vec2i size;
    u64 hash = 0;
};

template<>
inline void PrintType(TextureFiltering& type, FormatInfo& info) {
    PrintType(TEXTURE_FILTERING_NAMES[(int)type], info);
}

template<>
inline void PrintType(TextureWrapping& type, FormatInfo& info) {
    PrintType(TEXTURE_WRAPPING_NAMES[(int)type], info);
}

}
