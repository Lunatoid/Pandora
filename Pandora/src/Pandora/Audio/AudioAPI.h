#pragma once

#include "Pandora/Core/Resources/ResourceCatalog.h"

#include "Pandora/Audio/AudioBackend.h"
#include "Pandora/Audio/AudioHandle.h"
#include "Pandora/Audio/Audio.h"

namespace pd {


class AudioAPI {
public:
    virtual ~AudioAPI() = default;

    /**
     * \brief Creates the global audio API. This can only be called once.
     * 
     * \param backend The backend.
     * \return A pointer to the global audio API.
     */
    static AudioAPI* Create(AudioBackend backend);

    /**
     * \return A pointer to the global audio API.
     */
    static AudioAPI* Get();

    /**
     * \brief Deletes the global audio API.
     * If there is none, it does nothing.
     */
    static void Delete();

    /**
     * \brief Creates a new audio handle for the specified audio source.
     * 
     * \param audio The audio source.
     * \return The handle.
     */
    virtual AudioHandle CreateHandle(Ref<Audio> audio) = 0;

    /**
     * \brief Deletes the specified audio handle.
     * 
     * \param handle The handle.
     */
    virtual void DeleteHandle(AudioHandle handle) = 0;

    // @TODO: 3D audio

    /**
     * \brief Sets the global volume.
     * 
     * \param volume The ovlume.
     */
    virtual void SetGlobalVolume(f32 volume) = 0;

    /**
     * \brief Plays the audio of the specified handle.
     * 
     * \param handle The handle.
     * \param volume The volume.
     */
    virtual void Play(AudioHandle handle, f32 volume) = 0;

    /**
     * \brief Pauses the audio of the specified handle.
     * 
     * \param handle The handle.
     */
    virtual void Pause(AudioHandle handle) = 0;

    /**
     * \brief Resumes the audio of the specified handle.
     * 
     * \param handle The handle.
     */
    virtual void Resume(AudioHandle handle) = 0;

    /**
     * \param handle The handle.
     * \return Whether or not the audio of the specified handle is paused.
     */
    virtual bool IsPaused(AudioHandle handle) = 0;

    /**
     * \brief Stops the audio of the specified handle.
     * 
     * \param handle The handle.
     */
    virtual void Stop(AudioHandle handle) = 0;

    /**
     * \brief Sets whether or not the audio of the specified
     * handle shoud loop.
     * 
     * \param handle The handle.
     * \param shouldLoop Whether or not it should loop.
     */
    virtual void SetLooping(AudioHandle handle, bool shouldLoop) = 0;

    /**
     * \brief Sets the volume for the audio of the specified handle.
     * 
     * \param handle The handle.
     * \param volume The volume.
     */
    virtual void SetVolume(AudioHandle handle, f32 volume) = 0;

    /**
     * \brief Seeks to the specified time in the audio of the specified handle.
     * 
     * \param handle The handle.
     * \param time The time.
     */
    virtual void Seek(AudioHandle handle, Duration time) = 0;

    /**
     * \brief Installs the request handler for audio loading.
     * 
     * \param catalog The catalog.
     */
    virtual void SetAudioRequestHandler(ResourceCatalog& catalog) = 0;

    /**
     * \brief The default import options.
     */
    struct {
        u64 streamSizeThreshold = (u64)Megabytes(16.0);
        f32 streamLengthThreshold = 15.0f;
    } audioOptions;

};

}
