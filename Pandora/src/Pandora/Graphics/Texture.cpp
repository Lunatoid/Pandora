#include "Texture.h"

#include "Pandora/Core/Data/Memory.h"
#include "Pandora/Core/IO/File.h"
#include "Pandora/Core/IO/Console.h"

#include "Pandora/Libs/stb/stb_image.h"
#include "Pandora/Libs/stb/stb_image_write.h"

namespace pd {

bool Texture::Load(StringView path) {
    if (!LoadPixelsFromFile(path)) {
        return false;
    }

    CreateTextureData();
    Upload(false);

    return true;
}

bool Texture::Load(Box& box, StringView name) {
    if (!box.HasResource(name)) return false;

    ResourceType type = box.GetResourceType(name);

    if (!(type == ResourceType::Binary || type == ResourceType::Texture)) {
        // We cannot Load this resource as a texture
        return false;
    }

    switch (type) {
        case ResourceType::Binary: {
            Array<byte> data;
            box.GetResourceData(name, data);
            if (!LoadPixelsFromMemory(data)) {
                return false;
            }

            CreateTextureData();
            Upload(false);

            return true;
        }

        case ResourceType::Texture: {
            Array<byte> data;
            box.GetResourceData(name, data);

            // Texture format is <filtering, byte> <wrapping, byte> <int, width> <int, height> <byte, rgba>
            MemoryStream memory(data);

            if (memory.Read(&filtering) != sizeof(filtering)) {
                return false;
            }
            if (memory.Read(&wrapping) != sizeof(wrapping)) {
                return false;
            }
            if (memory.Read(&size.x) != sizeof(size.x)) {
                return false;
            }
            if (memory.Read(&size.y) != sizeof(size.y)) {
                return false;
            }
            Create(data.Slice((int)memory.Position()), size.x);
            return true;
        }
    }

    return false;
}

void Texture::Create(Vec2i size, bool white) {
    CreateBlankPixels(size, (white) ? 0xFF : 0x00);
    CreateTextureData();
    Upload(false);
}

void Texture::Create(Slice<byte> pixels, int stride) {
    size.x = stride;
    size.y = pixels.Count() / (stride * 4);

    // Delete any old data
    Delete();

    u64 sizeInBytes = (u64)size.x * (u64)size.y * 4;
    this->pixels = (byte*)Alloc(sizeInBytes);

    // Initialize the pixels to white
    MemoryCopy(this->pixels, pixels.Data(), sizeInBytes);

    CreateTextureData();
    Upload(false);
}

void Texture::Create(Slice<Color> pixels, int stride) {
    if (pixels.SizeInBytes() == 0 || stride == 0) return;

    size.x = stride;
    size.y = pixels.Count() / stride;

    // Delete any old data
    Delete();

    u64 sizeInBytes = (u64)size.x * (u64)size.y * 4;
    this->pixels = (byte*)Alloc(sizeInBytes);

    for (int y = 0; y < size.y; y++) {
        for (int x = 0; x < size.x; x++) {
            SetPixel(Vec2i(x, y), pixels[y * size.x + x]);
        }
    }

    CreateTextureData();
    Upload(false);
}

void Texture::ReleasePixelData() {
    if (pixels) {
        Free(pixels);
        pixels = nullptr;
    }
}

void Texture::Export(StringView path, TextureExportFormat exportFormat) {
    if (!pixels) return;

    int stride = size.x * 4;

    stbi_flip_vertically_on_write(true);
    switch (exportFormat) {
        case TextureExportFormat::PNG:
            stbi_write_png(path.CStr(), size.x, size.y, 4, pixels, stride);
            break;

        case TextureExportFormat::JPG:
            stbi_write_jpg(path.CStr(), size.x, size.y, 4, pixels, 0);
            break;

        case TextureExportFormat::BMP:
            stbi_write_bmp(path.CStr(), size.x, size.y, 4, pixels);
            break;

        case TextureExportFormat::TGA:
            stbi_write_tga(path.CStr(), size.x, size.y, 4, pixels);
            break;
    }
}

Color Texture::GetPixel(Vec2i position) {
    if (!pixels || position.x < 0 || position.x >= size.x || position.y < 0 || position.y >= size.y) return Color();

    int index = (position.y * size.x + position.x) * 4;

    byte r = pixels[index];
    byte g = pixels[index + 1];
    byte b = pixels[index + 2];
    byte a = pixels[index + 3];

    return Color((f32)r / 255.0f, (f32)g / 255.0f, (f32)b / 255.0f, (f32)a / 255.0f);
}

void Texture::SetPixel(Vec2i position, Color color) {
    if (!pixels || position.x < 0 || position.x >= size.x || position.y < 0 || position.y >= size.y) return;

    int index = (position.y * size.x + position.x) * 4;

    byte r = (byte)(color.r * 255.0f);
    byte g = (byte)(color.g * 255.0f);
    byte b = (byte)(color.b * 255.0f);
    byte a = (byte)(color.a * 255.0f);

    pixels[index] = r;
    pixels[index + 1] = g;
    pixels[index + 2] = b;
    pixels[index + 3] = a;
}

void Texture::SetPixels(Vec2i position, const Ref<Texture>& texture) {
    // @TODO: switch this to something faster, like getting/setting rows
    // of pixels instead of per-pixel
    for (int y = 0; y < texture->GetSize().y; y++) {
        for (int x = 0; x < texture->GetSize().x; x++) {
            Color c = texture->GetPixel(Vec2i(x, y));
            SetPixel(position + Vec2i(x, y), c);
        }
    }
}

void Texture::SetPixels(Vec2i position, Slice<byte> pixels, int stride) {
    int width = stride;
    int height = pixels.Count() / (stride * 4);

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int index = (y * width + x) * 4;

            Color c = Color();
            c.r = (f32)pixels[index + 0] / 255.0f;
            c.g = (f32)pixels[index + 1] / 255.0f;
            c.b = (f32)pixels[index + 2] / 255.0f;
            c.a = (f32)pixels[index + 3] / 255.0f;

            SetPixel(position + Vec2i(x, y), c);
        }
    }
}

void Texture::SetPixels(Vec2i position, Slice<Color> pixels, int stride) {
    int width = stride;
    int height = pixels.Count() / stride;

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int index = (y * width + x);

            SetPixel(position + Vec2i(x, y), pixels[index]);
        }
    }
}

Vec2i Texture::GetSize() const {
    return size;
}

byte* Texture::Data() const {
    return pixels;
}

u64 Texture::GetHash() const {
    return hash;
}

bool Texture::ShouldFlipVertically() {
    return false;
}

bool Texture::LoadPixelsFromFile(StringView path) {
    if (!FileExists(path)) return false;

    // Delete any old data
    Delete();

    stbi_set_flip_vertically_on_load(true);

    int channels = 0;
    pixels = (byte*)stbi_load(path.CStr(), &size.x, &size.y, &channels, 4);

    return pixels != nullptr;
}

bool Texture::LoadPixelsFromMemory(Slice<byte> data) {
    // Delete any old data
    Delete();

    stbi_set_flip_vertically_on_load(true);

    int channels = 0;
    pixels = (byte*)stbi_load_from_memory(data.Data(), data.Count(), &size.x, &size.y, &channels, 4);

    return pixels != nullptr;
}

void Texture::CreateBlankPixels(Vec2i size, byte value) {
    // Delete any old data
    Delete();

    this->size = size;

    u64 sizeInBytes = (u64)size.x * (u64)size.y * 4;
    pixels = (byte*)Alloc(sizeInBytes);

    MemorySet(pixels, sizeInBytes, value);
}

}
