#ifndef FILTER_H_
#define FILTER_H_


/**
 * \file filters.h
 * \brief The collection of all avaliable filters.
 */


/**
 * \defgroup group_filters Filter
 * \brief This module contains various filters for signal processing.
 *
 * There currently are 8 signal processing filters and 4 IO filters supported by the library:
 * + IO
 *    - \ref VectorInput
 *    - \ref RiffWaveInput
 *    - \ref VectorOutput
 *    - \ref RiffWaveOutput
 * + Signal Processing
 *    - \ref FrequencyGenerator
 *    - \ref FrequencyShift
 *    - \ref AmplitudeModulator
 *    - \ref FrequencyModulator
 *    - \ref Fir
 *    - \ref Differentiator
 *    - \ref Cic
 *    - \ref LowPass
 */


#include <filter/vector_input.h>
#include <filter/riff_wave_input.h>
#include <filter/frequency_generator.h>
#include <filter/frequency_shift.h>
#include <filter/amplitude_modulator.h>
#include <filter/frequency_modulator.h>
#include <filter/fir.h>
#include <filter/differentiator.h>
#include <filter/low_pass.h>
#include <filter/cic.h>
#include <filter/vector_output.h>
#include <filter/riff_wave_output.h>


#endif //FILTER_H_