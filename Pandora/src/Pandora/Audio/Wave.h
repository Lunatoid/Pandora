#pragma once

#include "Pandora/Core/IO/Stream.h"

namespace pd {

/// <summary>
/// Writes an IEEE Float RIFF/Wave file.
/// </summary>
/// <param name="stream">The output stream.</param>
/// <param name="channels">The channel count.</param>
/// <param name="sampleRate">The sample rate.</param>
/// <param name="sampleData">The IEEE Float sample data.</param>
/// <returns>Whether or not it was written successfully.</returns>
bool WriteWave(Stream& stream, u16 channels, u32 sampleRate, Slice<f32> sampleData);

/// <summary>
/// Writes a PCM RIFF/Wave file.
/// </summary>
/// <param name="stream">The output stream.</param>
/// <param name="channels">The channel count.</param>
/// <param name="sampleRate">The sample rate.</param>
/// <param name="sampleData">The PCM sample data.</param>
/// <returns>Whether or not it was written successfully.</returns>
bool WriteWave(Stream& stream, u16 channels, u32 sampleRate, Slice<i16> sampleData);

}
