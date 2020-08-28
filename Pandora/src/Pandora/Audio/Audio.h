#pragma once

#include "Pandora/Core/Resources/Resource.h"

#include "Pandora/Core/Time/Stopwatch.h"

#include "Pandora/Audio/AudioHandle.h"

namespace pd {

class Audio : public Resource {
public:
    static ResourceType GetType() { return ResourceType::Audio; }
    
    /// <summary>
    /// Loads the audio resource from a file.
    /// </summary>
    /// <param name="path">The path.</param>
    /// <returns>Whether or not it loaded successfully.</returns>
    virtual bool Load(StringView path) = 0;
        
    /// <summary>
    /// Loads an audio resource from a boc.
    /// </summary>
    /// <param name="box">The box.</param>
    /// <param name="name">The resource name.</param>
    /// <returns>Whether or not it loaded successfully.</returns>
    virtual bool Load(Box& box, StringView name) = 0;
    
    /// <summary>
    /// Returns the duration of the audio.
    /// </summary>
    /// <returns>The duration of the audio.</returns>
    virtual Duration GetDuration() = 0;
    
    /// <summary>
    /// The size threshold for exporting and streaming this audio.
    /// </summary>
    u64 streamSizeThreshold = (u64)Megabytes(16.0);

    /// <summary>
    /// The duration threshold for exporting and streaming this audio.
    /// </summary>
    f32 streamLengthThreshold = 15.0f;
};

}
