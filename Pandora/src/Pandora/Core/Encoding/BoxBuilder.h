#if defined(PD_BOX_BUILDER)
#pragma once

#include "Pandora/Core/VideoBackend.h"

#include "Pandora/Graphics/Texture.h"

namespace pd {

class BoxBuilder {
public:
    ~BoxBuilder();

    /**
     * \brief Frees the box builder. Gets called on destruction.
     */
    void Delete();

    /**
     * \brief Stages a generic resource.
     * A generic resource is a resource that only requires a name and a path.
     * 
     * \param name The name that should be used in the archive.
     * \param path The path to the file.
     * \param type The type of the resource.
     * \param compressed Whether or not it should compress the resource data.
     */
    void StageGeneric(StringView name, StringView path, ResourceType type, bool compressed);

    /**
     * \brief Stages a binary resource.
     * 
     * \param name The name that should be used in the archive.
     * \param path The path to the file.
     * \param compressed Whether or not it should compress the resource data.
     */
    void StageBinary(StringView name, StringView path, bool compressed);

    /**
     * \brief Stages a texture resource.
     * 
     * \param name The name that should be used in the archive.
     * \param path The path to the file.
     * \param filtering The filtering that should be used with this texture.
     * \param wrapping The wrapping that should be used with this texture.
     * \param compressed Whether or not it should compress the resource data.
     */
    void StageTexture(StringView name, StringView path, TextureFiltering filtering, 
                      TextureWrapping wrapping, bool compressed);

    /**
     * \brief Stages a mesh resource.
     * 
     * \param name The name that should be used in the archive.
     * \param path The path to the file.
     * \param compressed Whether or not it should compress the resource data.
     */
    void StageMesh(StringView name, StringView path, bool compressed);

    /**
     * \brief Stages a shader resource.
     * Call this multiple times with the same resource name to add different backends.
     * 
     * \param name The name that should be used in the archive.
     * \param backend The backend that the shader belongs to.
     * \param vertexPath The path to the vertex shader file.
     * \param pixelPath The path to the pixel shader file.
     * \param compressed Whether or not it should compress the resource data.
     */
    void StageShader(StringView name, VideoBackend backend, StringView vertexPath,
                     StringView pixelPath, bool compressed);

    /**
     * \brief Stages a font resource.
     * 
     * \param name The name that should be used in the archive.
     * \param path The path to the file.
     * \param compressed Whether or not it should compress the resource data.
     */
    void StageFont(StringView name, StringView path, bool compressed);

    /**
     * \brief Stages an audio resource.
     * 
     * \param name The name that should be used in the archive.
     * \param path The path to the file.
     * \param compressed Whether or not it should compress the resource data.
     */
    void StageAudio(StringView name, StringView path, bool compressed);

    /**
     * \brief Builds the .box file.
     * 
     * \param path The output path.
     * \return Whether or not it exported successfully.
     */
    bool Build(StringView path);

    /**
     * \brief Builds the .box file, encrypts all the resources.
     * Currently not implemented.
     * 
     * \param path The output path.
     * \param iv The IV. Must be 16 bytes long.
     * Consider using `GenerateRandomIV()`.
     * \param key The key. Must be 16 bytes long.
     * Consider using `GenerateRandomKey()`.
     */
    void BuildEncrypted(StringView path, Slice<byte> iv, Slice<byte> key);

    /**
     * \brief Stages all the resources as specified by a config file.
     * 
     * \param configPath The path to the config file.
     * \return Whether or not the config file was loaded successfully.
     */
    bool AddFromConfig(StringView configPath);

    /**
     * \brief Whether or not it shoud log the current status to the console.
     */
    bool logStatus = false;

    struct StagedShader {
        VideoBackend backend;
        String vertexPath;
        String pixelPath;
    };

    struct StagedFile {
        StagedFile() {};
        ~StagedFile();

        /**
         * \brief Frees all memory. Gets called on destruction.
         */
        void Delete();

        /**
         * \brief Sets the type of the file and calls the appropiate constructor..
         * 
         * \param type The type.
         */
        void SetType(ResourceType type);

        ResourceType type = ResourceType::Unknown;
        String name;
        bool compressed = true;

        // Data definitions
        struct GenericData {
            String path;
        };

        struct TextureData {
            String path;
            TextureFiltering filtering;
            TextureWrapping wrapping;
        };

        struct ShaderData {
            Array<StagedShader> shaders;
        };

        // Data union
        union {
            GenericData generic;
            TextureData texture;
            ShaderData shader;
        };
    };

    Slice<StagedFile> GetStagedFiles();
    bool EncodeResource(Stream& out, StagedFile& sf);

private:
    bool EncodeBinaryResource(Stream& out, StringView path, bool compressed);
    bool EncodeFontResource(Stream& out, StringView path, bool compressed);
    bool EncodeShaderResource(Stream& out, Slice<BoxBuilder::StagedShader> shaders, bool compressed);
    bool EncodeTextureResource(Stream& out, StringView path, TextureFiltering filtering, TextureWrapping wrapping, bool compressed);
    bool EncodeMeshResource(Stream& out, StringView path, bool compressed);
    bool EncodeAudioResource(Stream& out, StringView path, bool compressed);

    /**
     * \brief Gets a staged file. If it doesn't exist, it will add it.
     * Sets the name and type of the staged file.
     * 
     * \param name The resource name.
     * \param type The resource type.
     * \param compressed Whether or not it should compress the resource data.
     * \return A pointer to the staged file.
     */
    BoxBuilder::StagedFile* GetOrCreateStagedFile(StringView name, ResourceType type, bool compressed);

    /**
     * \brief Finds a staged file.
     * 
     * \param name The resource name.
     * \return The index of the staged file, if found.
     */
    Optional<int> FindStagedFile(StringView name);

    Array<StagedFile> stagedFiles;

    // If we're exporting an encrypted archive
    bool exportEncrypted = false;
    Slice<byte> iv;
    Slice<byte> key;
};

/**
 * \brief Builds the .box file from a config file.
 * Helper function for creating a BoxBuilder, calling `AddFromConfig()` followed by `Build()`.
 * 
 * \param path The output path.
 * \param configPath The path to the config file.
 * \param logStatus Whether or not it should log the status to the console.
 * \return Whether or not all operations succeeded.
 */
bool BuildBoxFromConfig(StringView path, StringView configPath, bool logStatus = false);

}

#endif
