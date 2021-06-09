#include "Wave.h"

namespace pd {

enum class WaveFormat {
    PCM = 1,
    Float = 3
};

bool WriteWavFile(Stream& stream, u16 channels, u32 sampleRate, u16 bitsPerSample, WaveFormat format, Slice<byte> sampleData) {
    stream.WriteText("RIFF");                                     // RIFF descriptor
    stream.Write<u32>((u32)sampleData.SizeInBytes() + 36);        // File size
    stream.WriteText("WAVEfmt ");                                 // Format + subchunk ID
    stream.Write<u32>(16);                                        // Header size
    stream.Write<u16>((u16)format);                               // Format (1 = PCM, 3 = float)
    stream.Write<u16>(channels);                                  // Channel count
    stream.Write<u32>(sampleRate);                                // Sample rate
    stream.Write<u32>(sampleRate * channels * bitsPerSample / 8); // Byte rate
    stream.Write<u16>(channels * bitsPerSample / 8);              // Block align
    stream.Write<u16>(bitsPerSample);                             // Bits per sample
    stream.WriteText("data");                                     // Data subchunk
    stream.Write<u32>((u32)sampleData.SizeInBytes());             // Subchunk size
    stream.WriteBytes(sampleData);                                // Actual data
    return true;
}

bool WriteWave(Stream& stream, u16 channels, u32 sampleRate, Slice<f32> sampleData) {
    return WriteWavFile(stream, channels, sampleRate, sizeof(f32) * 8, WaveFormat::Float, sampleData.As<byte>());
}

bool WriteWave(Stream& stream, u16 channels, u32 sampleRate, Slice<i16> sampleData) {
    return WriteWavFile(stream, channels, sampleRate, sizeof(u16) * 8, WaveFormat::PCM, sampleData.As<byte>());
}

}
