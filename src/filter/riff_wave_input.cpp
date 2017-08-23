#include <filter/riff_wave_input.h>
#include <vector>
#include <string>


//Force the compiler to generate a real and a complex version of the filter
template class RiffWaveInput<Real>;
template class RiffWaveInput<Complex>;


template<>
RiffWaveInput<Real>::RiffWaveInput(const std::string& path) :
	RIFFWaveIStream(path),
	Filter(FrameRate())
{
	for (int i = 0; i < ChannelCount(); ++i)
		AddOutput(Filter::FilterOutputType::REAL);
}


template<>
RiffWaveInput<Complex>::RiffWaveInput(const std::string& path) :
	RIFFWaveIStream(path),
	Filter(FrameRate())
{
	BOOST_ASSERT_MSG(ChannelCount() % 2 == 0, "In order to provide complex output, the number of audio channels has to be even!");
	for (int i = 0; i < ChannelCount() / 2; ++i)
		AddOutput(Filter::FilterOutputType::COMPLEX);
}


template<>
void RiffWaveInput<Real>::Update()
{
	int16_t sample[1];
	for (int channel = 0; channel < ChannelCount(); ++channel) {
		for (int i = 0; i < RealSimd::static_size; ++i) {
			this->operator>>(sample);
			real(channel)[i] = static_cast<Real>(sample[0]) / MaxSampleValue();
		}
	}
}


template<>
void RiffWaveInput<Complex>::Update()
{
	int16_t sample[2];
	for (int channel = 0; channel < ChannelCount(); ++channel) {
		for (int i = 0; i < RealSimd::static_size; ++i) {
			this->operator >> (sample);
			real(channel)[i] = static_cast<Real>(sample[0]) / MaxSampleValue();
			imag(channel)[i] = static_cast<Real>(sample[1]) / MaxSampleValue();
		}
	}
}