#ifndef FILTER_RIFF_WAVE_OUTPUT_H_
#define FILTER_RIFF_WAVE_OUTPUT_H_

#include <io/riff_wave.h>
#include <filter/filter.h>
#include <string>
#include <cstdint>
#include <types.h>


/**\ingroup group_filters
 * \brief Outputs all data to a WAVE file.
 *
 * If the filter is configured to take a real input, the input corresponds to one channel in the WAV file.
 * If it is configured to take a complex input, the input corresponds to two channels with the first
 * one representing the real and the second one representing the imaginary part.
 *
 * \tparam Tin The type of input this filter accepts. Either has to be \ref Real or \ref Complex.
 */
template<typename Tin>	//Defines the output type of the RiffWaveOutput (Either Real or Complex)
class RiffWaveOutput : public RIFFWaveOStream, public Filter
{
public:
	/**
	 * \brief Constructor
	 * \param sample_rate The number of samples per second at the input to this filter.
	 * \param path The path for the WAV file to be written to.
	 * \param dampening_factor A constant factor to multiply the input signal with. This defaults to 1.
	 */
	RiffWaveOutput(uint32_t sample_rate, const std::string& path, Real dampening_factor = 1);


	/**
	 * \brief Update the output file.
	 */
	virtual void Update();

private:
	Real dampening_factor;	///< The incoming signal is multiplied by this factor before it is written to the file.
};


#endif //FILTER_RIFF_WAVE_OUTPUT_H_