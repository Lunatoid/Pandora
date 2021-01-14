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
    
    /// <summary>
    /// Loads the texture from a file.
    /// </summary>
    /// <param name="path">The path.</param>
    /// <returns>Whether or not it loaded successfully.</returns>
    virtual bool Load(StringView path) override;

    /// <summary>
    /// Loads the texture from a box.
    /// </summary>
    /// <param name="box">The box.</param>
    /// <param name="name">The resource name.</param>
    /// <returns>Whether or not it loaded successfully.</returns>
    virtual bool Load(Box& box, StringView name) override;

    /// <summary>
    /// Creates a blank texture with the specified size and format.
    /// </summary>
    /// <param name="size">The size.</param>
    /// <param name="white">Creates a white texture if <c>true</c>, creates a transparent texture if <c>false</c>.</param>
    void Create(Vec2i size, bool white = true);

    /// <summary>
    /// Creates a texture from the specified RGBA pixels.
    /// </summary>
    /// <param name="pixels">The pixels in RGBA format.</param>
    /// <param name="stride">The stride in pixels.</param>
    void Create(Slice<byte> pixels, int stride);

    /// <summary>
    /// Creates a texture from the specified RGBA pixels.
    /// </summary>
    /// <param name="pixels">The pixel colors.</param>
    /// <param name="stride">The stride.</param>
    void Create(Slice<Color> pixels, int stride);

    /// <summary>
    /// Deletes the texture data. Gets called on destruction.
    /// </summary>
    virtual void Delete() = 0;
    
    /// <summary>
    /// Uploads the pixels to the GPU.
    /// Also recalculates the hash.
    /// </summary>
    /// <param name="releasePixelData">If set to <c>true</c>, it will release the pixel memory will be freed after upload.</param>
    virtual void Upload(bool releasePixelData = true) = 0;
        
    /// <summary>
    /// Releases the pixel data.
    /// </summary>
    void ReleasePixelData();

    /// <summary>
    /// Binds the texture to the specified slot.
    /// </summary>
    /// <param name="textureSlot">The texture slot.</param>
    virtual void Bind(int textureSlot = 0) = 0;
    
    /// <summary>
    /// Writes the pixel data to a file. Pixel data must not have been released.
    /// </summary>
    /// <param name="path">The path.</param>
    /// <param name="exportFormat">The export format.</param>
    void Export(StringView path, TextureExportFormat exportFormat = TextureExportFormat::PNG);
    
    /// <summary>
    /// Gets the pixel at the specified position. Pixel data must not have been released.
    /// </summary>
    /// <param name="position">The position.</param>
    /// <returns>The color of the pixel.</returns>
    Color GetPixel(Vec2i position);
    
    /// <summary>
    /// Sets the pixel at the specified position. Pixel data must not have been released.
    /// </summary>
    /// <param name="position">The position.</param>
    /// <param name="color">The color.</param>
    void SetPixel(Vec2i position, Color color);

    /// <summary>
    /// Writes a texture to the position of this texture
    /// </summary>
    /// <param name="position">The position.</param>
    /// <param name="texture">The texture.</param>
    void SetPixels(Vec2i position, const Ref<Texture>& texture);

    /// <summary>
    /// Writes a texture to the position of this texture
    /// </summary>
    /// <param name="position">The position.</param>
    /// <param name="pixels">The pixels in RGBA format.</param>
    /// <param name="stride">The stride in pixels.</param>
    void SetPixels(Vec2i position, Slice<byte> pixels, int stride);

    /// <summary>
    /// Writes a texture to the position of this texture
    /// </summary>
    /// <param name="position">The position.</param>
    /// <param name="pixels">The pixel colors.</param>
    /// <param name="stride">The stride in pixels.</param>
    void SetPixels(Vec2i position, Slice<Color> pixels, int stride);

    /// <summary>
    /// Returns the size of the texture.
    /// </summary>
    /// <returns>The size of the texture.</returns>
    Vec2i GetSize() const;
    
    /// <summary>
    /// Returns the pixel data. Will return <c>nullptr</c> if the pixel data has been released.
    /// </summary>
    /// <returns>The pixel data.</returns>
    byte* Data() const;
    
    /// <summary>
    /// Returns the hash of the pixel data.
    /// The hash is calculated on upload.
    /// </summary>
    /// <returns>The hash.</returns>
    u64 GetHash() const;

    /// <summary>
    /// Whether or not this texture is upside down and needs to be flipped vertically.
    /// </summary>
    /// <returns>Whether or not it should be flipped verically.</returns>
    virtual bool ShouldFlipVertically();

#if !defined(PD_NO_IMGUI)
    /// <summary>
    /// Returns the ID used for <c>ImGui::Image()</c>.
    /// </summary>
    virtual ImTextureID GetImGuiTextureID() = 0;
#endif

    TextureFiltering filtering = TextureFiltering::Bilinear;
    TextureWrapping wrapping = TextureWrapping::Clamp;
protected:    
    /// <summary>
    /// Loads the pixels from a file.
    /// </summary>
    /// <param name="path">The path.</param>
    /// <returns>Whether or not it loaded successfully.</returns>
    bool LoadPixelsFromFile(StringView path);

    /// <summary>
    /// Loads the pixels from a file in memory.
    /// </summary>
    /// <param name="data">The file.</param>
    /// <returns>Whether or not it loaded successfully.</returns>
    bool LoadPixelsFromMemory(Slice<byte> data);

    /// <summary>
    /// Creates a white texture with the specified size and format.
    /// </summary>
    /// <param name="size">The size.</param>
    /// <param name="iterator">The pixel iterator.</param>
    void CreateBlankPixels(Vec2i size, byte value);
    
    /// <summary>
    /// Creates the platform-specific texture data.
    /// </summary>
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
