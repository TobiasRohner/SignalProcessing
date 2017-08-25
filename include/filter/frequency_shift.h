#ifndef FILTER_FREQUENCY_SHIFT_H_
#define FILTER_FREQUENCY_SHIFT_H_

#include <filter/filter.h>
#include <boost/simd/trigonometric.hpp>
#include <boost/simd/function/aligned_store.hpp>


/**
 * \brief Frequency Shift Filter
 *
 * This filter shifts the frequency spectrum
 *
 * \tparam Tin The type of input this filter accepts. Either has to be \ref Real or \ref Complex
 */
template<typename Tin>
class FrequencyShift : public Filter
{
public:
	/**
	 * \brief Constructor
	 * \param sample_rate The number of samples per second arriving at the input of this filter
	 * \param shift The number of Hertz to shift the frequency spectrum
	 */
	FrequencyShift(double sample_rate, Real shift);


	/**
	 * \brief Update the output of this filter
	 */
	virtual void Update();

private:
	const Real deltaphase_div_pi;	///< The phase difference between two samples divided by pi
	RealSimd phase_div_pi;			///< The current phase divided by pi

	/**
	 * \brief initialize the phase SIMD register
	 */
	void initPhase();

	/**
	 * \brief Update the phase and keep it in a range as close to 0 as possible to maximize accuracy
	 */
	void UpdatePhase();

	/**
	 * \brief Update the output of this filter
	 */
	void CalculateOutput();
};


//---------- FrequencyShift implementation of inlined functions ----------//

template<typename Tin>
inline void FrequencyShift<Tin>::UpdatePhase()
{
	phase_div_pi += RealSimd(RealSimd::static_size * deltaphase_div_pi);
	if (phase_div_pi[RealSimd::static_size / 2] > 2 || phase_div_pi[RealSimd::static_size / 2] < 2)
		phase_div_pi -= RealSimd(2 * ((int)phase_div_pi[RealSimd::static_size/2] / 2));
}


template<>
inline void FrequencyShift<Real>::CalculateOutput()
{
	auto exp = boost::simd::sincospi(phase_div_pi);
	auto input = boost::simd::aligned_load<RealSimd>(GetRealInput(0));
	boost::simd::aligned_store(exp.second * input, real());
	boost::simd::aligned_store(exp.first * input, imag());
}


template<>
inline void FrequencyShift<Complex>::CalculateOutput()
{
	auto exp = boost::simd::sincospi(phase_div_pi);
	auto in = GetComplexInput(0);
	auto re = boost::simd::aligned_load<RealSimd>(in.first);
	auto im = boost::simd::aligned_load<RealSimd>(in.second);
	boost::simd::aligned_store(exp.second*re - exp.first*im, real());
	boost::simd::aligned_store(exp.first*re + exp.second*im, imag());
}



#endif //FILTER_FREQUENCY_SHIFT_H_