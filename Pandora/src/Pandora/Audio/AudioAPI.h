#pragma once

#include "Pandora/Core/Resources/ResourceCatalog.h"

#include "Pandora/Audio/AudioBackend.h"
#include "Pandora/Audio/AudioHandle.h"
#include "Pandora/Audio/Audio.h"
namespace pd {

class AudioAPI {
public:
    virtual ~AudioAPI() = default;
    
    /// <summary>
    /// Creates the global audio API. This can only be called once.
    /// </summary>
    /// <param name="backend">The backend.</param>
    /// <returns>A pointer to the global audio API.</returns>
    static AudioAPI* Create(AudioBackend backend);
    
    /// <summary>
    /// Returns the global audio API.
    /// Will assert in debug mode if no audio API has been created.
    /// </summary>
    /// <returns>The global audio API.</returns>
    static AudioAPI* Get();

    /// <summary>
    /// Deletes the global audio API.
    /// If there is no global audio API it will do nothing.
    /// </summary>
    static void Delete();
    
    /// <summary>
    /// Creates a new handle from the specified audio buffer.
    /// </summary>
    /// <param name="audio">The audio.</param>
    /// <returns>The handle.</returns>
    virtual AudioHandle CreateHandle(Ref<Audio> audio) = 0;
    
    /// <summary>
    /// Deletes the handle.
    /// </summary>
    /// <param name="handle">The handle.</param>
    virtual void DeleteHandle(AudioHandle handle) = 0;

    // @TODO: 3D audio
    
    /// <summary>
    /// Sets the global volume.
    /// </summary>
    /// <param name="volume">The volume.</param>
    virtual void SetGlobalVolume(f32 volume) = 0;
    
    /// <summary>
    /// Plays the handle instance.
    /// </summary>
    /// <param name="handle">The handle.</param>
    /// <param name="volume">The volume.</param>
    virtual void Play(AudioHandle handle, f32 volume) = 0;
    
    /// <summary>
    /// Pauses the handle instance.
    /// </summary>
    /// <param name="handle">The handle.</param>
    virtual void Pause(AudioHandle handle) = 0;
    
    /// <summary>
    /// Resumes the handle instance.
    /// </summary>
    /// <param name="handle">The handle.</param>
    virtual void Resume(AudioHandle handle) = 0;
    
    /// <summary>
    /// Checks if the handle instance is paused.
    /// </summary>
    /// <param name="handle">The handle.</param>
    /// <returns>Whether or not the handle instance is paused.</returns>
    virtual bool IsPaused(AudioHandle handle) = 0;
    
    /// <summary>
    /// Stops the handle instance.
    /// </summary>
    /// <param name="handle">The handle.</param>
    virtual void Stop(AudioHandle handle) = 0;
    
    /// <summary>
    /// Sets whether or not the handle instance should loop.
    /// </summary>
    /// <param name="handle">The handle.</param>
    /// <param name="shouldLoop">If <c>true</c>, it will loop.</param>
    virtual void SetLooping(AudioHandle handle, bool shouldLoop) = 0;
        
    /// <summary>
    /// Sets the volume of the handle instance.
    /// </summary>
    /// <param name="handle">The handle.</param>
    /// <param name="volume">The volume.</param>
    virtual void SetVolume(AudioHandle handle, f32 volume) = 0;
        
    /// <summary>
    /// Seeks to the specified time.
    /// </summary>
    /// <param name="handle">The handle.</param>
    /// <param name="time">The time.</param>
    virtual void Seek(AudioHandle handle, Duration time) = 0;

    /// <summary>
    /// Installs the backend specific audio loader to the catalog.
    /// </summary>
    /// <param name="catalog">The catalog.</param>
    virtual void SetAudioRequestHandler(ResourceCatalog& catalog) = 0;

    struct {
        u64 streamSizeThreshold = (u64)Megabytes(16.0);
        f32 streamLengthThreshold = 15.0f;
    } audioOptions;

};

}
