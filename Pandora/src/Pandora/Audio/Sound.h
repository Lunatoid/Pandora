#pragma once

#include "Pandora/Core/Data/Reference.h"
#include "Pandora/Core/Resources/ResourceCatalog.h"

#include "Pandora/Audio/Audio.h"
#include "Pandora/Audio/AudioAPI.h"

namespace pd {


class Sound {
public:
    Sound();
    ~Sound();

    /**
     * \brief Loads the sound.
     * 
     * \param audioName The audio resource name.
     */
    void Load(StringView audioName);

    /**
     * \brief Loads the sound.
     * 
     * \param audio The audio.
     */
    void Load(Ref<Audio> audio);

    /**
     * \brief Plays the audio.
     */
    void Play();

    /**
     * \brief Pauses the audio.
     */
    void Pause();

    /**
     * \brief Resume the audio.
     */
    void Resume();

    /**
     * \brief Toggles between pause and play/resume.
     */
    void TogglePause();

    /**
     * \brief Stops the audio.
     */
    void Stop();

    /**
     * \return Whether or not the audio is paused.
     */
    bool IsPaused();

    /**
     * \brief Sets the audio volume.
     * 
     * \param newVolume The new volume. If negative it will reapply the current volume.
     */
    void SetVolume(f32 newVolume = -1.0f);

    /**
     * \brief Sets whether or not the audio should loop.
     * 
     * \param shouldLoop Whether or not it should loop.
     */
    void SetLooping(bool shouldLoop);

    /**
     * \brief Seeks to the specified time.
     * 
     * \param time The time.
     */
    void Seek(Duration time);

    /// <summary>
    /// Gets the duration of the audio buffer.
    /// </summary>
    /// <returns>The duration of the audio buffer.</returns>
    Duration GetDuration();

    f32 volume = 1.0f;

private:
    bool shouldLoop = false;

    ResourceCatalog& catalog;
    Ref<Audio> buffer;
    AudioAPI* audio = nullptr;
    AudioHandle handle = nullptr;

};

}
