#ifndef FILTER_AMPLITUDE_MODULATOR_H_
#define FILTER_AMPLITUDE_MODULATOR_H_

#include <filter/frequency_generator.h>
#include <types.h>


/**\ingroup group_filters
 * \brief An Amplitude Modulator.
 *
 * This Filter generates an amplitude modulated sinusodial \ref Real or \ref Complex valued output signal.
 * The input to this filter must be of type \ref Real.
 *
 * \tparam Tout The type of signal this filter should generate. Either has to be \ref Real or \ref Complex.
 */
template<typename Tout>
class AmplitudeModulator : public FrequencyGenerator<Tout>
{
public:
	/**
	 * \brief Constructor
	 * \param sample_rate The number of samples per second this filter takes as an input.
	 * \param frequency The frequency of the amplitude modulated signal carrier. This defaults to 1Hz.
	 * \param amplitude The maximal amplitude of the carrier wave assuming the input's amplitude
	 *                  never leaves the range [-1, 1]. This defaults to 1.
	 * \param phaseshift The phase shift of the carrier wave in the interval `[0, 2pi)`.
	 */
	AmplitudeModulator(double sample_rate, Real frequency = Real(1), Real amplitude = Real(1), Real phaseshift = Real(0));


	/**
	 * \brief Update the output of this filter.
	 */
	virtual void Update();

	using Filter::SetInput;

protected:
	/**
	 * \brief Update the output of this filter.
	 */
	void CalculateOutput();

private:
	using Filter::AddInput;
	using FrequencyGenerator<Tout>::UpdatePhase;
};


//---------- inlined / templated functions implementation ----------//

///\cond IMPLEMENTATION_DETAILS
template<>
inline void AmplitudeModulator<Real>::CalculateOutput()
{
	RealSimd ampl = RealSimd(amplitude) * boost::simd::aligned_load<RealSimd, const Real*>(GetRealInput(0));
	auto result = boost::simd::cospi(phase_div_pi);
	boost::simd::aligned_store(ampl * result, real());
}
///\endcond


///\cond IMPLEMENTATION_DETAILS
template<>
inline void AmplitudeModulator<Complex>::CalculateOutput()
{
	RealSimd ampl = RealSimd(amplitude) * boost::simd::aligned_load<RealSimd, const Real*>(GetRealInput(0));
	auto result = boost::simd::sincospi(phase_div_pi);
	boost::simd::aligned_store(ampl * result.second, real());
	boost::simd::aligned_store(ampl * result.first, imag());
}
///\endcond


#endif //FILTER_AMPLITUDE_MODULATOR_H_