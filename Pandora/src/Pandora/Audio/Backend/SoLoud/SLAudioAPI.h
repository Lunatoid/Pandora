#if !defined(PD_NO_SOLOUD)
#pragma once

#include "Pandora/Audio/AudioAPI.h"

#include <SoLoud/soloud.h>

namespace pd {

class SLAudioAPI final : public AudioAPI {
public:
    SLAudioAPI();
    virtual ~SLAudioAPI();

    SoLoud::Soloud* GetSoloud();

    virtual AudioHandle CreateHandle(Ref<Audio> audio) override;
    virtual void DeleteHandle(AudioHandle handle) override;

    virtual void SetGlobalVolume(f32 volume) override;
    virtual void Play(AudioHandle handle, f32 volume) override;
    virtual void Pause(AudioHandle handle) override;
    virtual void Resume(AudioHandle handle) override;
    virtual bool IsPaused(AudioHandle handle) override;
    virtual void Stop(AudioHandle handle) override;
    virtual void SetLooping(AudioHandle handle, bool shouldLoop) override;
    virtual void SetVolume(AudioHandle handle, f32 volume) override;
    virtual void Seek(AudioHandle handle, Duration time) override;

    virtual void SetAudioRequestHandler(ResourceCatalog& catalog) override;

private:
    SoLoud::Soloud soloud;
};

}

#endif
