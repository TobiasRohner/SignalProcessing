#include <filter/riff_wave_output.h>
#include <vector>


//Force the compiler to generate a Real and a Complex version of RiffWaveOutput
template class RiffWaveOutput<Real>;
template class RiffWaveOutput<Complex>;


///\cond IMPLEMENTATION_DETAILS
template<>
RiffWaveOutput<Real>::RiffWaveOutput(uint32_t sample_rate, const std::string& path, Real dampening_factor) :
	RIFFWaveOStream(path, sample_rate, 1),
	Filter(sample_rate),
	dampening_factor(dampening_factor)
{
	AddInput({ nullptr, 0 });
}
///\endcond


///\cond IMPLEMENTATION_DETAILS
template<>
RiffWaveOutput<Complex>::RiffWaveOutput(uint32_t sample_rate, const std::string& path, Real dampening_factor) :
	RIFFWaveOStream(path, sample_rate, 2),
	Filter(sample_rate),
	dampening_factor(dampening_factor)
{
	AddInput({ nullptr, 0 });
}
///\endcond


///\cond IMPLEMENTATION_DETAILS
template<>
void RiffWaveOutput<Real>::Update()
{
	int16_t samples[1];
	for (int sample = 0; sample < RealSimd::static_size; ++sample) {
		samples[0] = static_cast<int16_t>(MaxSampleValue() * GetRealInput(0)[sample] * dampening_factor);
		this->operator<<(samples);
	}
}
///\endcond


///\cond IMPLEMENTATION_DETAILS
template<>
void RiffWaveOutput<Complex>::Update()
{
	int16_t samples[2];
	for (int sample = 0; sample < RealSimd::static_size; ++sample) {
		auto in = GetComplexInput(0);
		samples[0] = static_cast<int16_t>(MaxSampleValue() * in.first[sample] * dampening_factor);
		samples[1] = static_cast<int16_t>(MaxSampleValue() * in.second[sample] * dampening_factor);
		this->operator<<(samples);
	}
}
///\endcond