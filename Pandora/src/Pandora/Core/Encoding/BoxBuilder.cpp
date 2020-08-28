#if defined(PD_BOX_BUILDER)
#include "BoxBuilder.h"

#include "Pandora/Core/IO/Console.h"
#include "Pandora/Core/IO/File.h"

#include "Pandora/Core/Encoding/Compression.h"
#include "Pandora/Core/Encoding/Encryption.h"
#include "Pandora/Core/Encoding/JSON.h"
#include "Pandora/Core/Encoding/Box.h"

#include "Pandora/Libs/stb/stb_image.h"

#if !defined(PD_NO_ASSIMP)
#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#endif

#include <SoLoud/soloud.h>
#include <SoLoud/soloud_wav.h>
#include <SoLoud/soloud_wavstream.h>


#if !defined(PD_NO_DIRECTX)
#include <d3dcompiler.h>
#pragma comment(lib, "d3dcompiler")
#endif

namespace pd {

#define BOXB_LOG(fmt, ...) if (logStatus) {\
    console.Log("[{}Box{}] ", ConColor::Red, ConColor::White);\
    console.Log(fmt, __VA_ARGS__);\
}

BoxBuilder::~BoxBuilder() {
    Delete();
}

void BoxBuilder::Delete() {
    stagedFiles.Delete();
}

void BoxBuilder::StageBinary(StringView name, StringView path) {
    StageGenericResource(name, path, ResourceType::Binary);
}

void BoxBuilder::StageTexture(StringView name, StringView path, TextureFiltering filtering, TextureWrapping wrapping) {
    StagedFile* file = GetOrCreateStagedFile(name, ResourceType::Texture);
    console.Log(" {}({}){}\n", ConColor::Grey, path, ConColor::White);

    file->texture.path.Set(path);
    file->texture.filtering = filtering;
    file->texture.wrapping = wrapping;
}

void BoxBuilder::StageMesh(StringView name, StringView path) {
    StageGenericResource(name, path, ResourceType::Mesh);
}

void BoxBuilder::StageShader(StringView name, VideoBackend backend, StringView vertexPath, StringView pixelPath) {
    StagedFile* file = GetOrCreateStagedFile(name, ResourceType::Shader);

    // Check if we have already added this backend
    for (int i = 0; i < file->shader.shaders.Count(); i++) {
        if (file->shader.shaders[i].backend == backend) {
            return;
        }
    }

    console.Log(" {}({}){}\n", ConColor::Grey, backend, ConColor::White);
    file->shader.shaders.Reserve(1);
    file->shader.shaders.Last().backend = backend;
    file->shader.shaders.Last().vertexPath.Set(vertexPath);
    file->shader.shaders.Last().pixelPath.Set(pixelPath);
}

void BoxBuilder::StageFont(StringView name, StringView path) {
    StageGenericResource(name, path, ResourceType::Font);
}

void BoxBuilder::StageAudio(StringView name, StringView path) {
    StageGenericResource(name, path, ResourceType::Audio);
}

void BoxBuilder::StageGenericResource(StringView name, StringView path, ResourceType type) {
    StagedFile* file = GetOrCreateStagedFile(name, type);
    console.Log(" {}({}){}\n", ConColor::Grey, path, ConColor::White);

    file->generic.path.Set(path);
}

// This function is apparently 139616 bytes, that's an awful lot
bool BoxBuilder::Build(StringView path) {
    BOXB_LOG("Starting build\n");

    FileStream file(path, FileMode::Write);

    if (!file.IsOpen()) return false;

    // We should probably wrap up these writes to check if we wrote successfully
    BOXB_LOG("Writing header\n");
    file.WriteBytes(BOX_FILE_MAGIC);
    file.Write(BOX_VERSION);

    if (exportEncrypted) {
        file.WriteBytes(iv);
    } else {
        for (int i = 0; i < ENCRYPTION_BLOCK_LENGTH; i++) {
            file.WriteByte(0);
        }
    }

    // Write the file table
    u32 fileCount = (u32)stagedFiles.Count();
    file.Write(fileCount);

    Array<i64> dataPositions;
    dataPositions.Reserve(fileCount);
    for (u32 i = 0; i < fileCount; i++) {
        file.Write(stagedFiles[i].type);

        // Write name including null-terminator
        file.Write((u16)stagedFiles[i].name.SizeInBytes());
        file.WriteText(stagedFiles[i].name);
        file.WriteByte('\0');

        dataPositions[i] = file.Position();

        // We fill this out later with the data position
        file.Write<u64>(0);
    }

    file.WriteByte('\n');

    // Now write the file data
    for (u32 i = 0; i < fileCount; i++) {
        // Fill in data position
        i64 dataPosition = file.Position();
        file.Seek(dataPositions[i], SeekOrigin::Start);
        file.Write(dataPosition);
        file.Seek(dataPosition, SeekOrigin::Start);

        // Write file data
        StagedFile* sf = &stagedFiles[i];
        BOXB_LOG("Writing {}{}\t{}{}{}\n",
            ConColor::Yellow, sf->type, ConColor::Cyan, sf->name, ConColor::White);
        switch (sf->type) {
            case ResourceType::Binary:
            case ResourceType::Font: {
                // Open the file (do we want to read the entire file or use a stream?)
                Array<byte> fileBytes;
                u64 fileSize = ReadEntireFile(sf->generic.path, fileBytes);

                if (fileSize == 0) return false;

                // Write uncompressed size
                file.Write(fileSize);

                Array<byte> compressedBytes;
                CompressData(fileBytes, compressedBytes);
                fileBytes.Delete();

                // Write compressed size and data
                file.Write(compressedBytes.SizeInBytes());
                file.WriteBytes(compressedBytes.SliceAs<byte>());
                break;
            }

            case ResourceType::Shader: {
                MemoryStream output;

                // Write how many backends we support
                output.Write((byte)sf->shader.shaders.Count());

                auto writeShaderFile = [](Stream& output, StringView path, VideoBackend backend, bool isVertex) {
                    if (backend == VideoBackend::DirectX) {
#if !defined(PD_NO_DIRECTX)
                        ID3DBlob* blob;
                        ID3DBlob* error;

                        const char* shaderModel = (isVertex) ? "vs_4_0_level_9_3" : "ps_4_0_level_9_3";
                        HRESULT result = D3DCompileFromFile(path.ToWide(), NULL, D3D_COMPILE_STANDARD_FILE_INCLUDE,
                            "main", shaderModel, 0, 0, &blob, &error);

                        bool success = !error && result == S_OK;
                        if (!success) {
                            console.Log("[{}Box Error{}] D3DCompile: {}",
                                ConColor::Red, ConColor::White, (char*)error->GetBufferPointer());
                        } else {
                            output.Write((u32)blob->GetBufferSize());
                            output.WriteBytes(Slice<byte>((byte*)blob->GetBufferPointer(), (int)blob->GetBufferSize()));
                        }

                        if (blob) {
                            blob->Release();
                        }

                        if (error) {
                            error->Release();
                        }

                        return success;
#else
                        console.Log("[{}Box Error{}] cannot compile DirectX shaders because PD_NO_DIRECTX is defined\n",
                            ConColor::Red, ConColor::White);
                        return false;
#endif
                    } else if (backend == VideoBackend::OpenGL) {
                        output.Write((u32)GetFileSize(path));

                        Array<byte> fileBytes;
                        u64 fileSize = ReadEntireFile(path, fileBytes);

                        if (fileSize == 0) return false;

                        output.WriteBytes(fileBytes);
                    }

                    return true;
                };

                for (int i = 0; i < sf->shader.shaders.Count(); i++) {
                    StagedShader* ss = &sf->shader.shaders[i];

                    // Write the backend type
                    output.Write(ss->backend);

                    if (!writeShaderFile(output, ss->vertexPath, ss->backend, true)) return false;
                    if (!writeShaderFile(output, ss->pixelPath, ss->backend, false)) return false;
                }

                // Write uncompressed and compressed file size
                u64 outputSize = output.Position();
                file.Write(outputSize);

                // Fill in later
                u64 compressedSizePosition = file.Position();
                file.Write<u64>(0);

                // Compress output and write to file
                int written = CompressData(output.AsSlice(0, (int)outputSize), &file);

                // Go back and write compressed size
                file.Seek(compressedSizePosition, SeekOrigin::Start);
                file.Write((u64)written);
                file.Seek(0, SeekOrigin::End);
                break;
            }

            case ResourceType::Texture: {
                int width, height, channels;
                stbi_set_flip_vertically_on_load(true);
                byte* pixels = stbi_load((char*)sf->texture.path.Data(), &width, &height, &channels, 4);

                if (!pixels) return false;

                // Write texture format
                MemoryStream output;
                output.Write(sf->texture.filtering);
                output.Write(sf->texture.wrapping);

                output.Write(width);
                output.Write(height);

                output.WriteBytes(Slice<byte>(pixels, width * height * 4));

                // Free the texture data
                Free(pixels);

                // Write uncompressed and compressed file size
                u64 outputSize = output.Position();
                file.Write(outputSize);

                // Fill in later
                u64 compressedSizePosition = file.Position();
                file.Write<u64>(0);

                // Compress output and write to file
                int written = CompressData(output.AsSlice(0, (int)outputSize), &file);

                // Go back and write compressed size
                file.Seek(compressedSizePosition, SeekOrigin::Start);
                file.Write((u64)written);
                file.Seek(0, SeekOrigin::End);
                break;
            }

            case ResourceType::Mesh: {
#if !defined(PD_NO_ASSIMP)
                const aiScene* scene = aiImportFile((char*)sf->generic.path.Data(), aiProcess_Triangulate | aiProcess_JoinIdenticalVertices);

                if (!scene) return false;

                // Make sure this is in-sync with the MeshVertex in Graphics/Mesh
                struct MeshVertex {
                    Vec3 position;
                    Vec2 uv;
                    Vec3 normal;
                };

                aiMesh* mesh = scene->mMeshes[0];

                // Load mesh and write data
                MemoryStream output;

                output.Write((u32)mesh->mNumVertices);

                // Fill in the index count later
                u64 indexPosition = output.Position();
                output.Write<u32>(0);

                for (u32 i = 0; i < mesh->mNumVertices; i++) {
                    MeshVertex v;
                    v.position = Vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);

                    if (mesh->mTextureCoords) {
                        v.uv = Vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
                    }

                    if (mesh->mNormals) {
                        v.normal = Vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
                    }

                    output.Write(v);
                }

                u32 indexCount = 0;
                for (u32 i = 0; i < mesh->mNumFaces; i++) {
                    for (u32 j = 0; j < mesh->mFaces[i].mNumIndices; j++) {
                        output.Write(mesh->mFaces[i].mIndices[j]);
                        indexCount += 1;
                    }
                }

                // Free import
                aiReleaseImport(scene);

                u64 endPosition = output.Position();
                output.Seek(indexPosition, SeekOrigin::Start);
                output.Write(indexCount);
                output.Seek(endPosition, SeekOrigin::Start);

                // Write uncompressed and compressed file size
                u64 outputSize = output.Position();
                file.Write(outputSize);

                // Fill in later
                u64 compressedSizePosition = file.Position();
                file.Write<u64>(0);

                // Compress output and write to file
                int written = CompressData(output.AsSlice(0, (int)outputSize), &file);

                // Go back and write compressed size
                file.Seek(compressedSizePosition, SeekOrigin::Start);
                file.Write((u64)written);
                file.Seek(0, SeekOrigin::End);
#else
                PD_ASSERT(false, "Assimp is not included, cannot build meshes.");
#endif
                break;
            }

            case ResourceType::Audio: {
                SoLoud::Soloud soloud;
                soloud.init();

                SoLoud::Wav wave;
                if (wave.load((char*)sf->generic.path.Data()) != 0) {
                    return false;
                }

                MemoryStream output;


                // Write the metadata and samples
                output.Write<f64>(wave.getLength());
                output.Write<u32>((u32)wave.mBaseSamplerate);
                output.Write<u16>((u16)wave.mChannels);
                output.Write<u32>(wave.mSampleCount);

                // Convert float to PCM
                const f32 SAMPLE_MAX = (f32)(INT16_MAX + 1);

                // There is apparently some hot debate for the case where the sample is 1.0
                // and it introduces some distortion.
                // I found this blog that goes into more detail http://blog.bjornroche.com/2009/12/int-float-int-its-jungle-out-there.html
                if (wave.mChannels == 1) {
                    for (u32 i = 0; i < wave.mSampleCount; i++) {
                        f32 monoConverted = wave.mData[i] * SAMPLE_MAX;
                        i16 mono = (i16)Clamp<int>((int)monoConverted, INT16_MIN, INT16_MAX);
                        output.Write<i16>(mono);
                    }
                } else {
                    for (u32 i = 0; i < wave.mSampleCount; i++) {
                        // Convert
                        f32 leftConverted = wave.mData[i] * SAMPLE_MAX;
                        f32 rightConverted = wave.mData[i + wave.mSampleCount] * SAMPLE_MAX;

                        // Clamp
                        i16 left = (i16)Clamp<int>((int)leftConverted, INT16_MIN, INT16_MAX);
                        i16 right = (i16)Clamp<int>((int)rightConverted, INT16_MIN, INT16_MAX);

                        // Write
                        output.Write<i16>(left);
                        output.Write<i16>(right);
                    }
                }

                soloud.deinit();

                // Write uncompressed and compressed file size
                u64 outputSize = output.Position();
                file.Write(outputSize);

                // Fill in later
                u64 compressedSizePosition = file.Position();
                file.Write<u64>(0);

                // Compress output and write to file
                int written = CompressData(output.AsSlice(0, (int)outputSize), &file); // Seems like float/pcm samples don't compress well at all

                // Go back and write compressed size
                file.Seek(compressedSizePosition, SeekOrigin::Start);
                file.Write((u64)written);
                file.Seek(0, SeekOrigin::End);
                break;
            }
        }
    }

    return true;
}

void BoxBuilder::BuildEncrypted(StringView path, Slice<byte> iv, Slice<byte> key) {
    PD_ASSERT(iv.SizeInBytes() == ENCRYPTION_BLOCK_LENGTH, "IV must be %d bytes long, %lld bytes given", ENCRYPTION_BLOCK_LENGTH, iv.SizeInBytes());
    PD_ASSERT(key.SizeInBytes() == ENCRYPTION_BLOCK_LENGTH, "key must be %d bytes long", ENCRYPTION_BLOCK_LENGTH);

    exportEncrypted = true;
    this->iv = iv;
    this->key = key;

    Build(path);
}

#define BOX_ASSERT(expr, fmt, ...) if (!(expr)) {\
    console.Log("[{}Box Error{}] ", ConColor::Red, ConColor::White);\
    console.Log(fmt, __VA_ARGS__);\
    console.Log("\n");\
    return false;\
}

bool BoxBuilder::AddFromConfig(StringView configPath) {
    JsonValue configFile;

    // Parse config file and so some sanity checks
    BOX_ASSERT(configFile.Parse(configPath), "couldn't open config file");
    BOX_ASSERT(configFile.Type() == JsonType::Object, "root of config is not an object");
    BOX_ASSERT(configFile.HasField("items"), "array field 'items' not found in root object");
    BOX_ASSERT(configFile["items"].Type() == JsonType::Array, "field 'items' in root object is not a string");

    // Stage all assets from the "items" array
    JsonValue& items = configFile["items"];

    for (int i = 0; i < items.Count(); i++) {
        if (items[i].Type() != JsonType::Object) return false;

        JsonValue& item = items[i];

        // Sanity check common fields
        BOX_ASSERT(item.HasField("name"), "string field 'name' not found in item {}", i);
        BOX_ASSERT(item["name"].Type() == JsonType::String, "field 'name' in item {} is not a string", i);
        BOX_ASSERT(item.HasField("type"), "string field 'type' not found in item {}", i);
        BOX_ASSERT(item["type"].Type() == JsonType::String, "field 'type' in item {} is not a string", i);

        // Get type
        ResourceType type = ResourceType::Unknown;

        for (int j = 0; j < (int)ResourceType::Count; j++) {
            if (item["type"].GetString() == RESOURCE_TYPE_NAMES[j]) {
                type = (ResourceType)j;
                break;
            }
        }

        BOX_ASSERT(type != ResourceType::Unknown, "could not recognize type of item {} ({})", i, item["type"]);

        // Parse type-specific fields
        switch (type) {
            case ResourceType::Binary:
            case ResourceType::Mesh:
            case ResourceType::Font:
            case ResourceType::Audio: {
                BOX_ASSERT(item.HasField("source"), "string field 'source' not found in item {}", i);
                BOX_ASSERT(item["source"].Type() == JsonType::String, "field 'source' in item {} is not a string", i);

                // If only we had access to AddGenericResource()...
                switch (type) {
                    case ResourceType::Binary:
                        StageBinary(item["name"].GetString(), item["source"].GetString());
                        break;
                    case ResourceType::Mesh:
                        StageMesh(item["name"].GetString(), item["source"].GetString());
                        break;
                    case ResourceType::Font:
                        StageFont(item["name"].GetString(), item["source"].GetString());
                        break;
                    case ResourceType::Audio:
                        StageAudio(item["name"].GetString(), item["source"].GetString());
                        break;
                }
                break;
            }

            case ResourceType::Texture: {
                BOX_ASSERT(item.HasField("source"), "string field 'source' not found in item {}", i);
                BOX_ASSERT(item["source"].Type() == JsonType::String, "field 'source' in item {} is not a string", i);
                BOX_ASSERT(item.HasField("filtering"), "string field 'filtering' not found in item {}", i);
                BOX_ASSERT(item["filtering"].Type() == JsonType::String, "field 'source' in item {} is not a string", i);
                BOX_ASSERT(item.HasField("wrapping"), "string field 'wrapping' not found in item {}", i);
                BOX_ASSERT(item["wrapping"].Type() == JsonType::String, "field 'wrapping' in item {} is not a string", i);

                TextureFiltering filtering = TextureFiltering::Count;
                TextureWrapping wrapping = TextureWrapping::Count;

                for (int i = 0; i < (int)TextureFiltering::Count; i++) {
                    if (item["filtering"].GetString() == TEXTURE_FILTERING_NAMES[i]) {
                        filtering = (TextureFiltering)i;
                        break;
                    }
                }

                for (int i = 0; i < (int)TextureWrapping::Count; i++) {
                    if (item["wrapping"].GetString() == TEXTURE_WRAPPING_NAMES[i]) {
                        wrapping = (TextureWrapping)i;
                        break;
                    }
                }

                // Convert the options to a slice so we can pretty print it
                Slice<StringView> filteringOptions((StringView*)TEXTURE_FILTERING_NAMES, (int)TextureFiltering::Count);
                Slice<StringView> wrappingOptions((StringView*)TEXTURE_WRAPPING_NAMES, (int)TextureWrapping::Count);

                BOX_ASSERT(filtering != TextureFiltering::Count, "invalid filtering mode '{}', acceptable values are:\n{#}",
                    item["filtering"].GetString(), filteringOptions);
                BOX_ASSERT(wrapping != TextureWrapping::Count, "invalid wrapping mode '{}', acceptable values are:\n{#}",
                    item["wrapping"].GetString(), wrappingOptions);

                StageTexture(item["name"].GetString(), item["source"].GetString(), filtering, wrapping);
                break;
            }

            case ResourceType::Shader: {
                BOX_ASSERT(item.HasField("sources"), "array field 'sources' not found in item {}", i);
                BOX_ASSERT(item["sources"].Type() == JsonType::Array, "field 'sources' in item {} is not an array", i);

                JsonValue& sources = item["sources"];
                for (int j = 0; j < sources.Count(); j++) {
                    // Sanity check aaaaall the fields
                    BOX_ASSERT(sources[j].Type() == JsonType::Object, "shader source {} in item {} is not an object", j, i);
                    BOX_ASSERT(sources[j].HasField("backend"), "string field 'backend' not found in shader source {} in item {}", j, i);
                    BOX_ASSERT(sources[j]["backend"].Type() == JsonType::String, "field 'backend' is not a string in shader source {} in item {}", j, i);
                    BOX_ASSERT(sources[j].HasField("vertex"), "string field 'vertex' not found in shader source {} in item {}", j, i);
                    BOX_ASSERT(sources[j]["vertex"].Type() == JsonType::String, "field 'vertex' is not a string in shader source {} in item {}", j, i);
                    BOX_ASSERT(sources[j].HasField("pixel"), "string field 'pixel' not found in shader source {} in item {}", j, i);
                    BOX_ASSERT(sources[j]["pixel"].Type() == JsonType::String, "field 'pixel' is not a string in shader source {} in item {}", j, i);

                    // Get backend
                    VideoBackend backend = VideoBackend::Count;
                    for (int b = 0; b < (int)VideoBackend::Count; b++) {
                        if (sources[j]["backend"].GetString() == VIDEO_BACKEND_NAMES[b]) {
                            backend = (VideoBackend)b;
                            break;
                        }
                    }

                    BOX_ASSERT(backend != VideoBackend::Count, "could not recognize backend of shader source {} in item {} ({})", j, i, sources[j]["backend"]);

                    StageShader(item["name"].GetString(), backend, sources[j]["vertex"].GetString(), sources[j]["pixel"].GetString());
                }

                break;
            }
        }
    }

    // @TODO: encrypted builds
    return true;
}

#undef BOX_ASSERT

BoxBuilder::StagedFile* BoxBuilder::GetOrCreateStagedFile(StringView name, ResourceType type) {
    BOXB_LOG("Staging {}{}\t{}{}{}", ConColor::Yellow, type, ConColor::Cyan, name, ConColor::White);

    int index = FindStagedFile(name);

    BoxBuilder::StagedFile* file = nullptr;

    if (index >= 0) {
        file = &stagedFiles[index];
    } else {
        stagedFiles.Reserve(1);
        file = &stagedFiles.Last();
        file->name.Set(name);
        file->SetType(type);
    }

    return file;
}

int BoxBuilder::FindStagedFile(StringView name) {
    return stagedFiles.Find([&](StagedFile& file) {
        return file.name == name;
    });
}

BoxBuilder::StagedFile::StagedFile() {
    type = ResourceType::Unknown;
}

BoxBuilder::StagedFile::~StagedFile() {
    Delete();
}

void BoxBuilder::StagedFile::SetType(ResourceType type) {
    Delete();

    this->type = type;

    switch (type) {
        case ResourceType::Binary:
        case ResourceType::Mesh:
        case ResourceType::Font:
        case ResourceType::Audio: {
            new (&generic.path) BoxBuilder::StagedFile::GenericData();
            break;
        }

        case ResourceType::Texture: {
            new (&generic.path) BoxBuilder::StagedFile::TextureData();
            break;
        }

        case ResourceType::Shader: {
            new (&shader.shaders) BoxBuilder::StagedFile::ShaderData();
            break;
        }

        default:
            PD_ASSERT_D(false, "Unhandled case [%d] in StagedFile::SetType()");
    }
}

void BoxBuilder::StagedFile::Delete() {
    switch (type) {
        case ResourceType::Binary:
        case ResourceType::Mesh:
        case ResourceType::Font:
        case ResourceType::Audio:
            generic.path.Delete();
            break;

        case ResourceType::Texture:
            texture.path.Delete();
            break;

        case ResourceType::Shader:
            shader.shaders.Delete();
            break;

        case ResourceType::Unknown:
            break;

        default:
            PD_ASSERT_D(false, "Unhandled case [%d] in StagedFile::Delete()", (int)type);
    }
}

bool BuildBoxFromConfig(StringView path, StringView configPath, bool logStatus) {
    BoxBuilder builder;
    builder.logStatus = logStatus;
    if (!builder.AddFromConfig(configPath)) return false;

    return builder.Build(path);
}

#undef BOXB_LOG
}

#endif
