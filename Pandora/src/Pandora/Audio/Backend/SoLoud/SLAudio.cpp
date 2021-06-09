#if !defined(PD_NO_SOLOUD)
#include "SLAudio.h"

#include "Pandora/Core/IO/Storage.h"
#include "Pandora/Core/IO/File.h"

#include "Pandora/Audio/Wave.h"

namespace pd {

SLAudio::SLAudio() : soloud(((SLAudioAPI*)AudioAPI::Get())->GetSoloud()) {}

SLAudio::~SLAudio() {
    waveStream.stop();
    wave.stop();
}

bool SLAudio::Load(StringView path) {
    // Only check the size constraint here...
    u64 size = GetFileSize(path);

    usingStream = size > streamSizeThreshold;

    bool success = false;
    if (usingStream) {
        success = waveStream.load(path.CStr()) == 0;
    } else {
        success = wave.load(path.CStr()) == 0;
    }

    if (success) {
        Configure();
    }

    return success;
}

bool SLAudio::Load(Box& box, StringView name) {
    if (!box.HasResource(name)) return false;

    ResourceType type = box.GetResourceType(name);

    if (!(type == ResourceType::Binary || type == ResourceType::Audio)) {
        // We cannot Load this resource as an audio clip
        return false;
    }

    switch (type) {
        case ResourceType::Binary: {
            box.GetResourceData(name, waveData);

            // If we're loading binary, that will be our hash
            hash = DoHash(waveData.SliceAs<byte>());

            // Only check the size constraint here...
            usingStream = waveData.SizeInBytes() > streamSizeThreshold;
            if (usingStream) {
                // Write cached version
                if (!HasCacheStorageFile(name, hash)) {
                    FileStream cacheFile;
                    if (CreateCacheStorageFile(name, hash, cacheFile)) {
                        cacheFile.WriteBytes(waveData);
                    }
                }

                String path;
                path.Set(GetCacheStoragePath());
                path.Append(name);

                waveData.Delete();

                waveStream.load(path.CStr());
            } else {
                wave.loadMem(waveData.Data(), (u32)waveData.SizeInBytes(), false, false);
            }


            Configure();

            return true;
        }

        case ResourceType::Audio: {
            // First check if we have a cached version
            if (HasCacheStorageFile(name, 0)) {
                String path;
                path.Set(GetCacheStoragePath());
                path.Append(name);

                waveStream.load((char*)path.CStr());

                hash = GetCacheStorageHash(name);

                Configure();
                return true;
            }

            // No cache, Load from box
            Array<byte> data;
            box.GetResourceData(name, data);

            // Audio format is <f64, seconds length> <f32, sample rate>, <u32, channels>, <u32 sample count>, <i16, samples...>
            MemoryStream memory(data);

            f64 lengthInSeconds;
            memory.Read<f64>(&lengthInSeconds);

            u32 sampleRate;
            memory.Read<u32>(&sampleRate);

            u16 channels;
            memory.Read<u16>(&channels);

            u32 sampleCount;
            memory.Read<u32>(&sampleCount);

            const int BYTES_PER_SAMPLE = sizeof(i16);

            u32 totalSampleCount = sampleCount * channels;
            waveData.Reserve(totalSampleCount * BYTES_PER_SAMPLE);
            memory.ReadBytes((byte*)waveData.Data(), (u64)totalSampleCount * BYTES_PER_SAMPLE);

            // If we're loading audio, the samples will be our hash
            hash = DoHash(waveData.SliceAs<byte>());

            usingStream = lengthInSeconds > streamLengthThreshold || data.SizeInBytes() > streamSizeThreshold;
            if (usingStream) {
                // Write cached version
                FileStream cacheFile;
                if (CreateCacheStorageFile(name, hash, cacheFile)) {
                    WriteWave(cacheFile, channels, sampleRate, waveData.SliceAs<i16>());
                    cacheFile.Close();
                }

                String path;
                path.Set(GetCacheStoragePath());
                path.Append(name);

                waveData.Delete();

                waveStream.load((char*)path.CStr());
            } else {
                // SoLoud is very fun in that it expects [left samples][right samples] instead of interleaved samples
                if (channels == 2) {
                    Array<i16> alignedSamples;
                    alignedSamples.Reserve(totalSampleCount);

                    Slice<i16> samples = waveData.SliceAs<i16>();
                    for (u32 i = 0; i < totalSampleCount; i += 2) {
                        int index = (i / 2) + i % 2;

                        alignedSamples[index] = samples[i];
                        alignedSamples[sampleCount + index] = samples[i + 1];
                    }

                    waveData.Delete();
                    wave.loadRawWave16((i16*)alignedSamples.Data(), totalSampleCount, (f32)sampleRate, (u32)channels);

                    // SoLoud creates a copy so we can delete ours
                    alignedSamples.Delete();
                } else {
                    wave.loadRawWave16((i16*)waveData.Data(), totalSampleCount, (f32)sampleRate, (u32)channels);

                    // SoLoud creates a copy so we can delete ours
                    waveData.Delete();
                }
            }

            Configure();
            return true;
        }
    }

    return false;
}

Duration SLAudio::GetDuration() {
    SoLoud::time time;
    if (usingStream) {
        time = waveStream.getLength();
    } else {
        time = wave.getLength();
    }

    return Duration(time);
}

SoLoud::AudioSource& SLAudio::GetSource() {
    if (usingStream) {
        return waveStream;
    } else {
        return wave;
    }
}

void SLAudio::Configure() {
    SoLoud::AudioSource& source = GetSource();

    // These settings are not really relevant yet because 3D audio is still being worked out.
    source.set3dListenerRelative(true);
    source.set3dMinMaxDistance(1.0f, 100.0f);
    source.set3dAttenuation(SoLoud::AudioSource::EXPONENTIAL_DISTANCE, 0.5f);
}

}

#endif
