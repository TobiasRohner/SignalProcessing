#ifndef FILTER_FIR_H_
#define FILTER_FIR_H_

#include <cstdint>
#include <filter/fir_no_simd_impl.h>
#include <filter/fir_simd_impl.h>
#include <type_traits>
#include <types.h>
#include <utility>


/**
 * \brief The number of taps a filter needs in order to use SIMD Instructions.
 */
constexpr int64_t TAPCOUNT_FOR_SIMD_FIR = std::numeric_limits<int64_t>::max();	//TODO: Replace by RealSimd::static_size as soon as the SIMD implementation works

/**
 * \brief The class a FIR filter with a specific tapcount inherits from.
 *
 * This is either \ref FirSimdImpl or \ref FirNoSimdImpl.
 */
template<typename Tin, int64_t tapcount>
using FirBaseType = std::conditional_t<tapcount < TAPCOUNT_FOR_SIMD_FIR, FirNoSimdImpl<Tin>, FirSimdImpl<Tin>>;


/**\ingroup group_filters
 * \brief A Finite Impulse Response filter.
 *
 * Convolves the input signal with the filter coefficients.
 * Has a `tapcount/2` samples delay to make it causal.
 *
 * \tparam Tin The type of input this filter accepts. Either has to be \ref Real or \ref Complex.
 * \tparam typcount The number of taps this filter has. This defaults to \ref FilterSize::DYNAMIC.
 */
template<typename Tin, int64_t tapcount = Filter::FilterSize::DYNAMIC>
class Fir : public FirBaseType<Tin, tapcount>
{
public:
	/**
	 * \brief Constructor
	 * \param sample_rate The number of samples per second ath the input of this filter.
	 */
	Fir(double sample_rate);

	/**
	 * \brief Constructor
	 * \param sample_rate The number of samples per second at the input to this filter.
	 * \param coeff A `std::vector<Real>` of filter coefficients.
	 */
	Fir(double sample_rate, const std::vector<Real>& coeff);

	/**
	 * \brief Updates the output of this filter
	 */
	virtual void Update() { FirBaseType<Tin, tapcount>::Update(); }

protected:
	using FirBaseType<Tin, tapcount>::SetCoefficients;
};


///\cond IMPLEMENTATION_DETAILS
template<typename Tin>
class Fir<Tin, static_cast<int64_t>(Filter::FilterSize::DYNAMIC)> : public Fir<Tin, 0>
{
public:
	Fir(double sample_rate);
	Fir(double sample_rate, const std::vector<Real>& coeff);

protected:
	using FirBaseType<Tin, Filter::FilterSize::DYNAMIC>::SetCoefficients;

private:

};
///\endcond


//---------- Fir Implementation ----------//

template<typename Tin, int64_t tapcount>
Fir<Tin, tapcount>::Fir(double sample_rate) :
	FirBaseType<Tin, tapcount>(sample_rate, std::vector<Real>(tapcount, 0))
{
}


template<typename Tin, int64_t tapcount>
Fir<Tin, tapcount>::Fir(double sample_rate, const std::vector<Real>& coeff) :
	FirBaseType<Tin, tapcount>(sample_rate, coeff)
{
	BOOST_ASSERT_MSG(coeff.size() == tapcount, "Wrong number of coefficients passed to the FIR Filter constructor!");
}


template<typename Tin>
Fir<Tin, Filter::FilterSize::DYNAMIC>::Fir(double sample_rate) :
	Fir<Tin, 0>(sample_rate)
{
}


template<typename Tin>
Fir<Tin, Filter::FilterSize::DYNAMIC>::Fir(double sample_rate, const std::vector<Real>& coeff) :
	Fir<Tin, 0>(sample_rate)
{
	this->SetCoefficients(coeff);
}


#endif //FILTER_FIR_H_