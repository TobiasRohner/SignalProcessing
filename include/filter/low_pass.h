#ifndef FILTER_LOW_PASS_H_
#define FILTER_LOW_PASS_H_

#include <filter/fir.h>
#include <filter/window_functions.h>
#include <boost/math/constants/constants.hpp>
#include <boost/math/special_functions/sinc.hpp>
#include <functional>
#include <utils.h>


/**
 * \brief A Low Pass Filter
 * \tparam Tin The type of input this filter accepts. Either has to be \ref Real or \ref Complex
 * \tparam tapcount The number of taps this filter uses. Can be \ref Filter::FilterSize::DYNAMIC or any positive integer.
 */
template<typename Tin, int64_t tapcount = Filter::FilterSize::DYNAMIC>
class LowPass : public Fir<Tin, tapcount>
{
public:
	/**
	 * \brief Constructor
	 * \param sample_rate The number of samples per second at the input of this filter
	 * \param cutoff_frequency The cutoff frequency of the low pass filter
	 * \param window_function The window function to weigh the coefficients with. This defaults to \ref Rectangular
	 */
	LowPass(double sample_rate, Real cutoff_frequency, const std::function<Real(Real)>& window_function = Rectangular);

	using Fir<Tin, tapcount>::Deltatime;
	using Fir<Tin, tapcount>::SetInput;
	using Fir<Tin, tapcount>::Update;

protected:
	const Real cutoff_frequency_norm;	///< The cutoff frequency of the filter normalized to the interval [0, 0.5] where 0.5 is the nyquist frequency

	/**
	 * \brief Get a filter coefficient
	 * \param x The position at which the filter coefficient should be generated
	 */
	Real coefficient_func(Real x) const;

	/**
	 * \brief Generate the coefficient vector
	 * \param window_function The window function to weigh the coefficients with
	 */
	std::vector<Real> coefficients(const std::function<Real(Real)>& window_function) const;
};


///\cond IMPLEMENTATION_DETAILS
template<typename Tin>
class LowPass<Tin, Filter::FilterSize::DYNAMIC> : public LowPass<Tin, 0>
{
public:
	LowPass(double sample_rate, Real cutoff_frequency, uint32_t tapcount, const std::function<Real(Real)>& window_function = Rectangular);

protected:
	std::vector<Real> coefficients(uint32_t count, const std::function<Real(Real)>& window_function) const;

	using LowPass<Tin, 0>::coefficient_func;
};


//---------- LowPass Implementation ----------//

template<typename Tin, int64_t tapcount>
LowPass<Tin, tapcount>::LowPass(double sample_rate, Real cutoff_frequency, const std::function<Real(Real)>& window_function) :
	Fir<Tin, tapcount>(sample_rate),
	cutoff_frequency_norm(cutoff_frequency / sample_rate)
{
	std::vector<Real> coeff(coefficients(window_function));
	this->SetCoefficients(coeff);
}


template<typename Tin>
LowPass<Tin, Filter::FilterSize::DYNAMIC>::LowPass(double sample_rate, Real cutoff_frequency, uint32_t tapcount, const std::function<Real(Real)>& window_function) :
	LowPass<Tin, 0>(sample_rate, cutoff_frequency, window_function)
{
	std::vector<Real> coeff(coefficients(tapcount, window_function));
	this->SetCoefficients(coeff);
}


template<typename Tin, int64_t tapcount>
inline Real LowPass<Tin, tapcount>::coefficient_func(Real x) const
{
	return 2 * cutoff_frequency_norm * boost::math::sinc_pi(2 * cutoff_frequency_norm * x);
}


template<typename Tin, int64_t tapcount>
inline std::vector<Real> LowPass<Tin, tapcount>::coefficients(const std::function<Real(Real)>& window_function) const
{
	std::vector<Real> coeff(tapcount);
	for (int i = 0; i < tapcount; ++i)
		coeff[i] = window_function(Real(i) / (tapcount - 1)) * coefficient_func(Real((i - Real(tapcount-1)/2)));
	return coeff;
}


template<typename Tin>
inline std::vector<Real> LowPass<Tin, Filter::FilterSize::DYNAMIC>::coefficients(uint32_t count, const std::function<Real(Real)>& window_function) const
{
	std::vector<Real> coeff(count);
	for (unsigned int i = 0; i < count; ++i)
		coeff[i] = window_function(Real(i) / (count - 1)) * coefficient_func(Real((i - Real(count - 1) / 2)));
	return coeff;
}


#endif //FILTER_LOW_PASS_H_