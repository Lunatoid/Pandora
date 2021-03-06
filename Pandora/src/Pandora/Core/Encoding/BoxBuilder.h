#if defined(PD_BOX_BUILDER)
#pragma once

#include "Pandora/Graphics/VideoBackend.h"
#include "Pandora/Graphics/Texture.h"

namespace pd {

class BoxBuilder {
public:
    ~BoxBuilder();

    /// <summary>
    /// Frees all memory related to this builder. This is called on destruction.
    /// </summary>
    void Delete();

    /// <summary>
    /// Stages a generic resource.
    /// A generic resource is a resource that only needs a name and a path to export.
    /// </summary>
    /// <param name="name">The name that should be used in the archive.</param>
    /// <param name="path">The path to the file.</param>
    /// <param name="type">The type of the resource.</param>
    /// <param name="compressed">Whether or not to compress the resource data.</param>
    void StageGeneric(StringView name, StringView path, ResourceType type, bool compressed);

    /// <summary>
    /// Stages a binary resource for exporting.
    /// </summary>
    /// <param name="name">The name that should be used in the archive.</param>
    /// <param name="path">The path to the file.</param>
    /// <param name="compressed">Whether or not to compress the resource data.</param>
    void StageBinary(StringView name, StringView path, bool compressed);

    /// <summary>
    /// Stages a texture for exporting.
    /// </summary>
    /// <param name="name">The name that should be used in the archive.</param>
    /// <param name="path">The path to the file.</param>
    /// <param name="compressed">Whether or not to compress the resource data.</param>
    void StageTexture(StringView name, StringView path, TextureFiltering filtering, TextureWrapping wrapping, bool compressed);

    /// <summary>
    /// Stages a mesh for exporting.
    /// </summary>
    /// <param name="name">The name that should be used in the archive.</param>
    /// <param name="path">The path to the file.</param>
    /// <param name="compressed">Whether or not to compress the resource data.</param>
    void StageMesh(StringView name, StringView path, bool compressed);

    /// <summary>
    /// Stages a shader of a specific backend for exporting.
    /// Call this function multiple times for each backend with the same name to add more backends to a single shader resource.
    /// </summary>
    /// <param name="name">The name.</param>
    /// <param name="backend">The backend.</param>
    /// <param name="vertexPath">The vertex path.</param>
    /// <param name="pixelPath">The pixel path.</param>
    /// <param name="compressed">Whether or not to compress the resource data.</param>
    void StageShader(StringView name, VideoBackend backend, StringView vertexPath, StringView pixelPath, bool compressed);

    /// <summary>
    /// Stages a font resource for exporting.
    /// </summary>
    /// <param name="name">The name that should be used in the archive.</param>
    /// <param name="path">The path to the file.</param>
    void StageFont(StringView name, StringView path, bool compressed);

    /// <summary>
    /// Stages an audio resource for exporting.
    /// </summary>
    /// <param name="name">The name that should be used in the archive.</param>
    /// <param name="path">The path to the file.</param>
    /// <param name="compressed">Whether or not to compress the resource data.</param>
    void StageAudio(StringView name, StringView path, bool compressed);

    /// <summary>
    /// Exports all staged files.
    /// </summary>
    /// <param name="path">The path.</param>
    /// <returns>Whether or not it exported successfully.</returns>
    bool Build(StringView path);

    /// <summary>
    /// Exports all staged files.
    /// </summary>
    /// <param name="path">The path.</param>
    /// <param name="iv">The IV. Must be 16 bytes.</param>
    /// <param name="key">The key. Must be 16 bytes.</param>
    void BuildEncrypted(StringView path, Slice<byte> iv, Slice<byte> key);

    /// <summary>
    /// Adds items from a .JSON config file.
    /// </summary>
    /// <param name="configPath">Path to the .JSON file.</param>
    /// <returns>Whether or not is succeeded.</returns>
    bool AddFromConfig(StringView configPath);

    bool logStatus = false;

    struct StagedShader {
        VideoBackend backend;
        String vertexPath;
        String pixelPath;
    };

    struct StagedFile {
        StagedFile() {};
        ~StagedFile();

        /// <summary>
        /// Frees all memory related to this staged file. This is called on destruction.
        /// </summary>
        void Delete();

        /// <summary>
        /// Sets the type of the file and calls the constructor on the appropiate union members.
        /// </summary>
        /// <param name="type">The type.</param>
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

    /// <summary>
    /// Gets the staged file for the requested resource. If there is not it will add it.
    /// Also sets the name and type of the staged file.
    /// </summary>
    /// <param name="name">The name.</param>
    /// <param name="type">The type.</param>
    /// <param name="compressed">Whether or not the resource data should be compressed.</param>
    /// <returns>The staged file.</returns>
    BoxBuilder::StagedFile* GetOrCreateStagedFile(StringView name, ResourceType type, bool compressed);

    /// <summary>
    /// Finds a staged file.
    /// </summary>
    /// <param name="name">The name.</param>
    /// <returns>The index of the staged file. -1 if not found.</returns>
    int FindStagedFile(StringView name);

    Array<StagedFile> stagedFiles;

    // If we're exporting an encrypted archive
    bool exportEncrypted = false;
    Slice<byte> iv;
    Slice<byte> key;
};

/// <summary>
/// Builds a .box file from a JSON configuration.
/// Shorthand for declaring a <c>BoxBuilder</c> and then calling <c>AddFromConfig()</c> followed by <c>Build()</c>.
/// </summary>
/// <param name="path">The path.</param>
/// <param name="configPath">Path to the JSON file.</param>
/// <param name="logStatus">Whether or not to log the status of the builder to the console.</param>
/// <returns>Whether or not is succeeded.</returns>
bool BuildBoxFromConfig(StringView path, StringView configPath, bool logStatus = false);

}

#endif
