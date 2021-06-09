#if !defined(PD_NO_SOLOUD)
#include "SLAudioAPI.h"

#include "Pandora/Core/IO/Console.h"
#include "Pandora/Core/Input/Input.h"

#include "Pandora/Audio/Backend/SoLoud/SLAudio.h"

namespace pd {

SLAudioAPI::SLAudioAPI() {
    soloud.init();

    CONSOLE_LOG_DEBUG("  {}> SoLoud: {}, {}{}\n", ConColor::Grey, soloud.getVersion(), soloud.getBackendString(), ConColor::White);
}

SLAudioAPI::~SLAudioAPI() {
    soloud.deinit();
}

SoLoud::Soloud* SLAudioAPI::GetSoloud() {
    return &soloud;
}

AudioHandle SLAudioAPI::CreateHandle(Ref<Audio> audio) {
    SLAudio* sl = audio.As<SLAudio>();

    AudioHandle handle = (AudioHandle)(u64)soloud.play(sl->GetSource(), -1.0f, 0.0f, true);

    return handle;
}

void SLAudioAPI::DeleteHandle(AudioHandle handle) {
    // Not applicable
}

void SLAudioAPI::SetGlobalVolume(f32 volume) {
    soloud.setGlobalVolume(volume);
}

void SLAudioAPI::Play(AudioHandle handle, f32 volume) {
    SoLoud::handle audio = (SoLoud::handle)(u64)handle;
    if (!soloud.isValidVoiceHandle(audio)) return;

    soloud.setPause(audio, false);
    SetVolume(handle, volume);
}

void SLAudioAPI::Pause(AudioHandle handle) {
    SoLoud::handle audio = (SoLoud::handle)(u64)handle;
    if (!soloud.isValidVoiceHandle(audio)) return;

    soloud.setPause(audio, true);
}

void SLAudioAPI::Resume(AudioHandle handle) {
    SoLoud::handle audio = (SoLoud::handle)(u64)handle;
    if (!soloud.isValidVoiceHandle(audio)) return;

    soloud.setPause(audio, false);
}

bool SLAudioAPI::IsPaused(AudioHandle handle) {
    SoLoud::handle audio = (SoLoud::handle)(u64)handle;
    if (!soloud.isValidVoiceHandle(audio)) return false;

    return soloud.getPause(audio);
}

void SLAudioAPI::Stop(AudioHandle handle) {
    SoLoud::handle audio = (SoLoud::handle)(u64)handle;
    if (!soloud.isValidVoiceHandle(audio)) return;

    soloud.stop(audio);
}

void SLAudioAPI::SetLooping(AudioHandle handle, bool shouldLoop) {
    SoLoud::handle audio = (SoLoud::handle)(u64)handle;
    if (!soloud.isValidVoiceHandle(audio)) return;

    soloud.setLooping(audio, shouldLoop);
}

void SLAudioAPI::SetVolume(AudioHandle handle, f32 volume) {
    SoLoud::handle audio = (SoLoud::handle)(u64)handle;
    if (!soloud.isValidVoiceHandle(audio)) return;

    soloud.setVolume(audio, volume);
}

void SLAudioAPI::Seek(AudioHandle handle, Duration time) {
    SoLoud::handle audio = (SoLoud::handle)(u64)handle;
    if (!soloud.isValidVoiceHandle(audio)) return;
    
    soloud.seek(audio, time.seconds);
}

void SLAudioAPI::SetAudioRequestHandler(ResourceCatalog& catalog) {
    catalog.SetResourceRequestHandler(ResourceType::Audio, [](Box& box, ResourceType type, StringView name, void* data) {
        SLAudio* sound = New<SLAudio>();

        // Apply import options
        SLAudioAPI* audio = (SLAudioAPI*)data;
        sound->streamLengthThreshold = audio->audioOptions.streamLengthThreshold;
        sound->streamSizeThreshold = audio->audioOptions.streamSizeThreshold;

        if (!sound->Load(box, name)) {
            CONSOLE_LOG_DEBUG("[{}Resource Error{}] could not load audio {#} from box\n", ConColor::Red, ConColor::White, name);
        }

        return (Resource*)sound;
    }, this);
}

}

#endif
