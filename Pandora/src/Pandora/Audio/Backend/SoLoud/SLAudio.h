#if !defined(PD_NO_SOLOUD)
#pragma once

#include <SoLoud/soloud_wav.h>
#include <SoLoud/soloud_wavstream.h>

#include "Pandora/Audio/Audio.h"

#include "Pandora/Audio/Backend/SoLoud/SLAudioAPI.h"

namespace pd {

class SLAudio final : public Audio {
public:
    SLAudio();
    virtual ~SLAudio();

    virtual bool Load(StringView path) override;
    virtual bool Load(Box& box, StringView name) override;

    virtual Duration GetDuration() override;
    
    /// <summary>
    /// Returns the appropiate SoLoud instance.
    /// </summary>
    /// <returns></returns>
    SoLoud::AudioSource& GetSource();

private:    
    /// <summary>
    /// Configures the audio settings.
    /// </summary>
    void Configure();

    SoLoud::Soloud* soloud = nullptr;

    // Depending on the size/length thresholds we might export
    // the audio to the cache folder and then stream it from file.
    bool usingStream = true;

    SoLoud::WavStream waveStream;
    SoLoud::Wav wave;

    // This is either the binary wave data or the float samples
    Array<byte> waveData; 
};

}

#endif
