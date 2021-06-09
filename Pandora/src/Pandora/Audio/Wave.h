#pragma once

#include "Pandora/Core/IO/Stream.h"

namespace pd {

/**
 * \brief Writes an IEEE Float RIFF/Wave file.
 * 
 * \param stream The output stream.
 * \param channels How many channels there are.
 * \param sampleRate The sample rate.
 * \param sampleData The IEEE Float sample data.
 * \return Whether or not it was written successfully.
 */
bool WriteWave(Stream& stream, u16 channels, u32 sampleRate, Slice<f32> sampleData);

/**
 * \brief Writes a PCM RIFF/Wave file.
 * 
 * \param stream The output stream.
 * \param channels How many channels there are.
 * \param sampleRate The sample rate.
 * \param sampleData The PCM sample data.
 * \return Whether or not it was written successfully.
 */
bool WriteWave(Stream& stream, u16 channels, u32 sampleRate, Slice<i16> sampleData);

}
