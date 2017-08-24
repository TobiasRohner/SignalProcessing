#include <filter/frequency_modulator.h>


//Force the compiler to generate a Real and a Complex version of FrequencyModulator
template class FrequencyModulator<Real>;
template class FrequencyModulator<Complex>;


///\cond IMPLEMENTATION_DETAILS
template<>
FrequencyModulator<Real>::FrequencyModulator(uint32_t sample_rate, Real frequency, Real bandwidth, Real amplitude) :
	Filter(sample_rate, std::vector<Filter::FilterOutputType>(1, Filter::FilterOutputType::REAL)),
	frequency(frequency),
	bandwidth(bandwidth),
	amplitude(amplitude),
	phase_div_pi(0)
{
	AddInput({ nullptr, 0 });
	InitPhase();
	CalculateOutput();
}
///\endcond


///\cond IMPLEMENTATION_DETAILS
template<>
FrequencyModulator<Complex>::FrequencyModulator(uint32_t sample_rate, Real frequency, Real bandwidth, Real amplitude) :
	Filter(sample_rate, std::vector<Filter::FilterOutputType>(1, Filter::FilterOutputType::COMPLEX)),
	frequency(frequency),
	bandwidth(bandwidth),
	amplitude(amplitude),
	phase_div_pi(0)
{
	AddInput({ nullptr, 0 });
	InitPhase();
	CalculateOutput();
}
///\endcond


template<typename Tout>
void FrequencyModulator<Tout>::Update()
{
	UpdatePhase();
	CalculateOutput();
}


template<typename Tout>
void FrequencyModulator<Tout>::InitPhase()
{
	for (int i = 0; i < RealSimd::static_size; ++i)
		phase_div_pi[i] = frequency * static_cast<Real>(2 * i * DeltatimeOut());
}