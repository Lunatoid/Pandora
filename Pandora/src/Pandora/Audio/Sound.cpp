#include "Sound.h"

namespace pd {

Sound::Sound() : catalog(ResourceCatalog::Get()), audio(AudioAPI::Get()) {}

Sound::~Sound() {
    audio->DeleteHandle(handle);
}

void Sound::Load(StringView audioName) {
    buffer = catalog.Get<Audio>(audioName);
}

void Sound::Load(Ref<Audio> audio) {
    buffer = audio;
}

void Sound::Play() {
    audio->DeleteHandle(handle);

    handle = audio->CreateHandle(buffer);
    audio->Play(handle, volume);
    audio->SetLooping(handle, shouldLoop);
}

void Sound::Pause() {
    audio->Pause(handle);
}

void Sound::Resume() {
    audio->Resume(handle);
}

void Sound::TogglePause() {
    if (IsPaused()) {
        Resume();
    } else {
        Pause();
    }
}

void Sound::Stop() {
    audio->Stop(handle);
}

bool Sound::IsPaused() {
    return audio->IsPaused(handle);
}

void Sound::SetVolume(f32 newVolume) {
    if (newVolume >= 0.0f) {
        volume = newVolume;
    }
    audio->SetVolume(handle, volume);
}

void Sound::SetLooping(bool shouldLoop) {
    this->shouldLoop = shouldLoop;
}

void Sound::Seek(Duration time) {
    audio->Seek(handle, time);
}

Duration Sound::GetDuration() {
    return (buffer) ? buffer->GetDuration() : 0.0f;
}

}
