#include <filter/frequency_generator.h>
#include <iostream>
#include <utility>
#include <boost/math/constants/constants.hpp>


//Force the compiler to generate a Real and a Complex version of FrequencyGenerator
template class FrequencyGenerator<Real>;
template class FrequencyGenerator<Complex>;


///\cond IMPLEMENTATION_DETAILS
template<>
FrequencyGenerator<Real>::FrequencyGenerator(double sample_rate, Real frequency, Real amplitude, Real phaseshift) :
	Filter(sample_rate, std::vector<Filter::FilterOutputType>(1, Filter::FilterOutputType::REAL)),
	frequency(frequency),
	amplitude(amplitude),
	phaseshift(phaseshift)
{
	InitPhase();
	CalculateOutput();
}
///\endcond


///\cond IMPLEMENTATION_DETAILS
template<>
FrequencyGenerator<Complex>::FrequencyGenerator(double sample_rate, Real frequency, Real amplitude, Real phaseshift) :
	Filter(sample_rate, std::vector<Filter::FilterOutputType>(1, Filter::FilterOutputType::COMPLEX)),
	frequency(frequency),
	amplitude(amplitude),
	phaseshift(phaseshift)
{
	InitPhase();
	CalculateOutput();
}
///\endcond


template<typename Tout>
void FrequencyGenerator<Tout>::Update()
{
	CalculateOutput();
	UpdatePhase();
}


template<typename Tout>
void FrequencyGenerator<Tout>::InitPhase()
{
	Real phaseshift_time = phaseshift / boost::math::constants::pi<Real>();
	for (int i = 0; i < RealSimd::static_size; ++i)
		phase_div_pi[i] = frequency * static_cast<Real>(2 * i * DeltatimeOut()) + phaseshift_time;
}