#include <filter/differentiator.h>
#include <boost/simd/function/aligned_load.hpp>
#include <boost/simd/function/aligned_store.hpp>
#include <boost/math/constants/constants.hpp>


//Force the compiler to generate a Real and a Complex version of Differentiator
template class Differentiator<Real>;
template class Differentiator<Complex>;


///\cond IMPLEMENTATION_DETAILS
template<>
Differentiator<Real>::Differentiator(uint32_t sample_rate) :
	Filter(sample_rate, { Filter::FilterOutputType::REAL }),
	amplitude_correction_factor(sample_rate / boost::math::constants::two_pi<Real>()),
	last_sample_real(0)
{
	AddInput({ nullptr, 0 });
}
///\endcond


///\cond IMPLEMENTATION_DETAILS
template<>
Differentiator<Complex>::Differentiator(uint32_t sample_rate) :
	Filter(sample_rate, { Filter::FilterOutputType::COMPLEX }),
	amplitude_correction_factor(sample_rate / boost::math::constants::two_pi<Real>()),
	last_sample_real(0),
	last_sample_imag(0)
{
	AddInput({ nullptr, 0 });
}
///\endcond


///\cond IMPLEMENTATION_DETAILS
template<>
void Differentiator<Real>::Update()
{
	auto in = GetRealInput(0);
	for (int i = 0; i < RealSimd::static_size; ++i) {
		real()[i] = amplitude_correction_factor * (in[i] - last_sample_real);
		last_sample_real = in[i];
	}
}
///\endcond


///\cond IMPLEMENTATION_DETAILS
template<>
void Differentiator<Complex>::Update()
{
	auto in = GetComplexInput(0);
	for (int i = 0; i < RealSimd::static_size; ++i) {
		real()[i] = amplitude_correction_factor * (in.first[i] - last_sample_real);
		last_sample_real = in.first[i];
		imag()[i] = amplitude_correction_factor * (in.second[i] - last_sample_imag);
		last_sample_imag = in.second[i];
	}
}
///\endcond