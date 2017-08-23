#ifndef FILTER_RIFF_WAVE_INPUT_H_
#define FILTER_RIFF_WAVE_INPUT_H_

#include <io/riff_wave.h>
#include <filter/filter.h>
#include <string>


/**\ingroup group_filters
 * \brief Reads data from a WAVE file and provides them as input to other filters.
 *
 * If `Tout` is defined to be \ref Real, the filter will provide an output for every channel.
 * If `Tout` is defined to be \ref Complex, the filter will take two channels and combine them to one \ref Complex output.
 *
 * \tparam Tout The type of output this filter provides. Either has to be \ref Real or \ref Complex.
 */
template<typename Tout>
class RiffWaveInput : public RIFFWaveIStream, public Filter	//This inheritance order is important!
{
public:
	/**
	 * \brief Constructor
	 */
	RiffWaveInput(const std::string& path);


	/**
	 * \brief Update the outputs of this filter
	 */
	virtual void Update();

private:

};


#endif //FILTER_RIFF_WAVE_INPUT_H_