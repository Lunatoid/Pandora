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
    
    /// <summary>
    /// Loads the audio resource by name.
    /// </summary>
    /// <param name="audioName">Name of the audio.</param>
    void Load(StringView audioName);
    
    /// <summary>
    /// Loads the audio resource by reference.
    /// </summary>
    /// <param name="audio">The audio.</param>
    void Load(Ref<Audio> audio);
    
    /// <summary>
    /// Plays this sound.
    /// </summary>
    void Play();
    
    /// <summary>
    /// Pauses this sound.
    /// </summary>
    void Pause();
    
    /// <summary>
    /// Resumes this sound.
    /// </summary>
    void Resume();
    
    /// <summary>
    /// Toggles between pause and resume on this sound.
    /// </summary>
    void TogglePause();
    
    /// <summary>
    /// Stops this sound.
    /// </summary>
    void Stop();
    
    /// <summary>
    /// Returns whether or not this sound is paused.
    /// </summary>
    /// <returns>Whether or not this sound is paused.</returns>
    bool IsPaused();
    
    /// <summary>
    /// Sets the volume of this sound.
    /// </summary>
    /// <param name="newVolume">The new volume. If this is <c>-1.0f</c> it will apply the currently set volume.</param>
    void SetVolume(f32 newVolume = -1.0f);
    
    /// <summary>
    /// Sets whether or not this sound should loop.
    /// </summary>
    /// <param name="shouldLoop">Whether or not this sound should loop.</param>
    void SetLooping(bool shouldLoop);
    
    /// <summary>
    /// Seeks to the specified time.
    /// </summary>
    /// <param name="time">The time.</param>
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
