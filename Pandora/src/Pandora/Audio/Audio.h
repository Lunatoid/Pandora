#pragma once

#include "Pandora/Core/Resources/Resource.h"
#include "Pandora/Core/Time/Stopwatch.h"

#include "Pandora/Audio/AudioHandle.h"

namespace pd {

class Audio : public Resource {
public:
    static ResourceType GetType() { return ResourceType::Audio; }

    /**
     * \brief Loads the audio from a file.
     * 
     * \param path The path of the file.
     * \return Whether or not it loaded successfully.
     */
    virtual bool Load(StringView path) = 0;

    /**
     * \brief Loads the audio from a box.
     * 
     * \param box The box
     * \param name The audio resource name.
     * \return Whether or not it loaded successfully.
     */
    virtual bool Load(Box& box, StringView name) = 0;

    /**
     * \return The duration of the audio.
     */
    virtual Duration GetDuration() = 0;

    /**
     * \brief The size threshold for exporting and streaming this audio.
     */
    u64 streamSizeThreshold = (u64)Megabytes(16.0);

    /**
     * \brief The duration threshold for exporting and streaming this audio.
     */
    f32 streamLengthThreshold = 15.0f;
};

}
