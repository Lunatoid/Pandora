#include "AudioAPI.h"

#include "Pandora/Core/Assert.h"
#include "Pandora/Core/Data/Allocator.h"
#include "Pandora/Core/IO/Console.h"

#include "Pandora/Audio/Backend/SoLoud/SLAudioAPI.h"

namespace pd {

// @GLOBAL
static AudioAPI* globalAudioAPI = nullptr;

AudioAPI* pd::AudioAPI::Create(AudioBackend backend) {
    PD_ASSERT_D(!globalAudioAPI, "creation of multiple audio APIs not allowed");

    CONSOLE_LOG_DEBUG("  {}> Creating {} audio backend{}\n", ConColor::Grey, backend, ConColor::White);

#if !defined(PD_NO_SOLOUD)
    if (backend == AudioBackend::SoLoud) {
        return globalAudioAPI = New<SLAudioAPI>();
    }
#endif

    return nullptr;
}

AudioAPI* AudioAPI::Get() {
    PD_ASSERT_D(globalAudioAPI, "no audio API created");
    return globalAudioAPI;
}

void AudioAPI::Delete() {
    if (globalAudioAPI) {
        pd::Delete(globalAudioAPI);
    }
}

}
