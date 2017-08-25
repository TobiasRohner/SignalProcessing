#ifndef FILTER_FREQUENCY_GENERATOR_H_
#define FILTER_FREQUENCY_GENERATOR_H_

#include <filter/filter.h>
#include <types.h>
#include <boost/simd/pack.hpp>
#include <boost/simd/trigonometric.hpp>
#include <boost/simd/function/aligned_store.hpp>


/**\ingroup group_filters
 * \brief A sinusodial wave generator.
 *
 * \tparam Tout The type of output this filter generates. Either has to be \ref Real or \ref Complex.
 */
template<typename Tout>	//The type of output this filter generates. Either has to be Real or Complex
class FrequencyGenerator : public Filter
{
public:
	/**
	 * \brief Constructor
	 *
	 * \param sample_rate The number of samples this filter should generate per second.
	 * \param frequency The frequency of the generated waveform. This defaults to `1`Hz.
	 * \param amplitude The amplitude of the generated waveform. This defaults to `1`.
	 * \param phaseshift The phaseshift of the generated waveform in the interval `[0, 2pi)`. This defaults to `0`.
	 */
	FrequencyGenerator(double sample_rate, Real frequency = Real(1), Real amplitude = Real(1), Real phaseshift = Real(0));


	/**
	 * \brief Update the output of this filter.
	 *
	 * Generates the next \ref RealSimd::static_size samples and stores them in the output array.
	 */
	virtual void Update();


	/**
	 * \brief Get the frequency of the generated signal in Hertz.
	 * \returns The frequency of the generated signal in Hertz.
	 */
	Real Frequency() const;

	/**
	 * \brief Get the period of the generated signal.
	 * \returns The period of the generated signal.
	 */
	Real Period() const;

	/**
	 * \brief Get the amplitude of the generated signal.
	 * \returns The amplitude of the generated signal.
	 */
	Real Amplitude() const;

	/**
	 * \brief Get the phase shift of the generated signal.
	 * \returns The phaseshift of the generated signal.
	 */
	Real Phaseshift() const;

protected:
	/**
	 * \brief The frequency of the generated signal.
	 */
	const Real frequency;

	/**
	 * \brief The amplitude of the generated signal.
	 */
	const Real amplitude;

	/**
	 * \brief The phase shift of the generated signal.
	 */
	const Real phaseshift;

	
	/**
	 * \brief The current phase of the signal divided by `pi`.
	 */
	RealSimd phase_div_pi;

	/**
	 * \brief Initializes the phase of the signal.
	 */
	void InitPhase();

	/**
	 * \brief Updates the phase of the signal.
	 *
	 * This function has to be called before every generation of the next samples.
	 */
	void UpdatePhase();

	
	/**
	 * \brief Calculate the output using the current phase stores in \ref phase_div_pi.
	 */
	void CalculateOutput();
};


//---------- inlined / templated functions implementation ----------//

template<typename Tout>
inline Real FrequencyGenerator<Tout>::Frequency() const
{
	return frequency;
}


template<typename Tout>
inline Real FrequencyGenerator<Tout>::Period() const
{
	return static_cast<Real>(1.0 / frequency);
}


template<typename Tout>
inline Real FrequencyGenerator<Tout>::Amplitude() const
{
	return amplitude;
}


template<typename Tout>
inline Real FrequencyGenerator<Tout>::Phaseshift() const
{
	return phaseshift;
}


template<typename Tout>
inline void FrequencyGenerator<Tout>::UpdatePhase()
{
	phase_div_pi += RealSimd(2 * frequency * RealSimd::static_size * DeltatimeOut());
	//Subtract 2 (period of boost::simd::sincospi) as soon as the first value is bigger than 1
	if (phase_div_pi[RealSimd::static_size / 2] > 1)
		phase_div_pi -= RealSimd(2);
}


///\cond IMPLEMENTATION_DETAILS
template<>
inline void FrequencyGenerator<Real>::CalculateOutput()
{
	auto result = boost::simd::cospi(phase_div_pi);
	boost::simd::aligned_store(RealSimd(amplitude) * result, real());
}
///\endcond


///\cond IMPLEMENTATION_DETAILS
template<>
inline void FrequencyGenerator<Complex>::CalculateOutput()
{
	auto result = boost::simd::sincospi(phase_div_pi);
	boost::simd::aligned_store(RealSimd(amplitude) * result.second, real());
	boost::simd::aligned_store(RealSimd(amplitude) * result.first , imag());
}
///\endcond


#endif //FILTER_FREQUENCY_GENERATOR_H_