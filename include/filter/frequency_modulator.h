#ifndef FILTER_FREQUENCY_MODULATOR_H_
#define FILTER_FREQUENCY_MODULATOR_H_

#include <filter/filter.h>
#include <types.h>
#include <boost/simd/function/aligned_store.hpp>
#include <boost/simd/trigonometric.hpp>


/**\ingroup group_filters
 * \brief A Frequency Modulator
 *
 * This filter generates a sinusodial \ref Real or \ref Complex typed output signal.
 * The input to this filter must be of type \ref Real.
 *
 * \tparam Tout The type of output signal this filter should generator. Either has to be \ref Real or \ref Complex.
 */
template<typename Tout>
class FrequencyModulator : public Filter
{
public:
	/**
	 * \brief Constructor
	 * \param sample_rate The nummber of samples per second this filter takes as an input.
	 * \param frequency The frequency of the carrier wave. This defaults to 0Hz.
	 * \param bandwidth The bandwidth of the signal ass uming the input is in the range `[-1, 1]`. This defaults to 1Hz.
	 * \param amplitude The amplitude of the carrier wave. This defaults to 1.
	 */
	FrequencyModulator(uint32_t sample_rate, Real frequency = Real(0), Real bandwidth = Real(1), Real amplitude = Real(1));


	/**
	 * \brief Update the output of this filter.
	 */
	virtual void Update();


	/**
	 * \brief Get the Frequency of the carrier wave.
	 * \returns The frequency of the carrier wave.
	 */
	Real Frequency() const;

	/**
	 * \brief Get the period of the carrier wave.
	 * \returns The period of the carrier wave.
	 */
	Real Period() const;

	/**
	 * \brief Get the bandwidth of the signal.
	 * \returns The bandwidth of the signal.
	 */
	Real Bandwidth() const;

	/**
	 * \brief Get the amplitude of the signal.
	 * \returns The amplitude of the signal.
	 */
	Real Amplitude() const;

private:
	using Filter::AddInput;

	const Real frequency;	///< The frequency of the carrier wave.
	const Real bandwidth;	///< The bandwidth of the signal.
	const Real amplitude;	///< The amplitude of the carrier wave.

	RealSimd phase_div_pi;
	Real next_phase_div_pi_0 = 0;

	/**
	 * \brief Initialize the \ref phase_div_pi register.
	 */
	void InitPhase();

	/**
	 * \brief Update the phase based on the current input.
	 */
	void UpdatePhase();


	/**
	 * \brief Calculate and store the output of the filter.
	 */
	void CalculateOutput();
};


//---------- inlined / templated functions implementation ----------//

template<typename Tout>
inline Real FrequencyModulator<Tout>::Frequency() const
{
	return frequency;
}


template<typename Tout>
inline Real FrequencyModulator<Tout>::Period() const
{
	return static_cast<Real>(1.0 / frequency);
}


template<typename Tout>
inline Real FrequencyModulator<Tout>::Bandwidth() const
{
	return bandwidth;
}


template<typename Tout>
inline Real FrequencyModulator<Tout>::Amplitude() const
{
	return amplitude;
}


template<typename Tout>
inline void FrequencyModulator<Tout>::UpdatePhase()
{
	auto in = GetRealInput(0);
	phase_div_pi[0] = next_phase_div_pi_0;
	for (int i = 1; i < RealSimd::static_size; ++i)
		phase_div_pi[i] = phase_div_pi[i - 1] + (frequency + in[i - 1] * bandwidth * DeltatimeOut());
	next_phase_div_pi_0 = phase_div_pi[RealSimd::static_size - 1] + (frequency + in[RealSimd::static_size - 1] * bandwidth * DeltatimeOut());
	//Subtract 2 (period of boost::simd::sincospi) as soon as the first value is bigger than 1
	if (phase_div_pi[0] > 1)
		phase_div_pi -= RealSimd(2);
}


///\cond IMPLEMENTATION_DETAILS
template<>
inline void FrequencyModulator<Real>::CalculateOutput()
{
	auto result = boost::simd::cospi(phase_div_pi);
	boost::simd::aligned_store(RealSimd(amplitude) * result, real());
}
///\endcond


///\cond IMPLEMENTATION_DETAILS
template<>
inline void FrequencyModulator<Complex>::CalculateOutput()
{
	auto result = boost::simd::sincospi(phase_div_pi);
	boost::simd::aligned_store(RealSimd(amplitude) * result.second, real());
	boost::simd::aligned_store(RealSimd(amplitude) * result.first, imag());
}
///\endcond


#endif //FILTER_FREQUENCY_MODULATOR_H_