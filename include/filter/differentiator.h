#ifndef FILTER_DIFFERENTIATOR_H_
#define FILTER_DIFFERENTIATOR_H_

#include <types.h>
#include <filter/filter.h>


/**\ingroup group_filters
 * \brief Differentiate the input signal.
 * \tparam Tin The type of input signal to this filter. Either has to be \ref Real or \ref Complex.
 */
template<typename Tin>
class Differentiator : public Filter
{
public:
	/**
	 * \brief Constructor
	 * \param sample_rate The number of samples per second that are provided as input to this filter.
	 */
	Differentiator(uint32_t sample_rate);


	/**
	 * \brief Update the output of this filter.
	 */
	virtual void Update();

private:
	/**
	 * \brief The factor needed to output the correct derivative of the input signal.
	 *
	 * This factor is needed to correct for the fact that the time difference
	 * between two samples is not equal to 1s, so the steepness of the signal
	 * is considerably larger than just the difference between the samples.
	 */
	const Real amplitude_correction_factor;

	Real last_sample_real;	///< The real part of the last sample
	Real last_sample_imag;	///< The imaginary part of the last sample
};


#endif //FILTER_DIFFERENTIATOR_H_