#include <filter/amplitude_modulator.h>


//Force the compiler to generate a Real and a Complex version of AmplitudeModulator
template class AmplitudeModulator<Real>;
template class AmplitudeModulator<Complex>;



template<typename Tout>
AmplitudeModulator<Tout>::AmplitudeModulator(double sample_rate, Real frequency, Real amplitude, Real phaseshift) :
	FrequencyGenerator<Tout>(sample_rate, frequency, amplitude, phaseshift)
{
	AddInput({ nullptr, 0 });
}


template<typename Tout>
void AmplitudeModulator<Tout>::Update()
{
	CalculateOutput();
	UpdatePhase();
}