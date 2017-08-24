#include <filter/frequency_shift.h>


//Force the compiler to generate a Real and a Complex version of the filter
template class FrequencyShift<Real>;
template class FrequencyShift<Complex>;



template<typename Tin>
FrequencyShift<Tin>::FrequencyShift(uint32_t sample_rate, Real shift) :
	Filter(sample_rate, std::vector<Filter::FilterOutputType>(1, Filter::FilterOutputType::COMPLEX)),
	deltaphase_div_pi(2 * shift / sample_rate)
{
	AddInput({ nullptr, 0 });
	initPhase();
}


template<typename Tin>
void FrequencyShift<Tin>::Update()
{
	CalculateOutput();
	UpdatePhase();
}


template<typename Tin>
void FrequencyShift<Tin>::initPhase()
{
	for (int i = 0; i < RealSimd::static_size; ++i)
		phase_div_pi[i] = i * deltaphase_div_pi;
}