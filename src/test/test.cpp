#include <test/test.h>
#include <iostream>
#include <test/test_vector_io.h>
#include <test/test_frequency_generator.h>
#include <test/test_frequency_shift.h>
#include <test/test_amplitude_modulator.h>
#include <test/test_frequency_modulator.h>
#include <test/test_fir.h>



bool TestAll()
{
	bool succeeded = true;

	// Testing VectorInput and VectorOutput
	if (!TestVectorIOReal()) {
		succeeded = false;
		std::cout << "FAIL    : VectorInput<Real> or VectorOutput<Real>" << std::endl;
	}
	else
		std::cout << "SUCCESS : VectorInput<Real> and VectorOutput<Real>" << std::endl;
	if (!TestVectorIOComplex()) {
		succeeded = false;
		std::cout << "FAIL : VectorInput<Complex> or VectorOutput<Complex>" << std::endl;
	}
	else
		std::cout << "SUCCESS : VectorInput<Complex> and VectorOutput<Complex>" << std::endl;

	//Testing FrequencyGenerator
	if (!TestFrequencyGeneratorReal()) {
		succeeded = false;
		std::cout << "FAIL    : FrequencyGenerator<Real>" << std::endl;
	}
	else
		std::cout << "SUCCESS : FrequencyGenerator<Real>" << std::endl;
	if (!TestFrequencyGeneratorComplex()) {
		succeeded = false;
		std::cout << "FAIL    : FrequencyGenerator<Complex>" << std::endl;
	}
	else
		std::cout << "SUCCESS : FrequencyGenerator<Complex>" << std::endl;

	//Testing FrequencyShift
	if (!TestFrequencyShiftReal()) {
		succeeded = false;
		std::cout << "FAIL    : FrequencyShift<Real>" << std::endl;
	}
	else
		std::cout << "SUCCESS : FrequencyShift<Real>" << std::endl;
	if (!TestFrequencyShiftComplex()) {
		succeeded = false;
		std::cout << "FAIL    : FrequencyShift<Complex>" << std::endl;
	}
	else
		std::cout << "SUCCESS : FrequencyShift<Complex>" << std::endl;

	//Testing AmplitudeModulator
	if (!TestAmplitudeModulatorReal()) {
		succeeded = false;
		std::cout << "FAIL    : AmplitudeModulator<Real>" << std::endl;
	}
	else
		std::cout << "SUCCESS : AmplitudeModulator<Real>" << std::endl;
	if (!TestAmplitudeModulatorComplex()) {
		succeeded = false;
		std::cout << "FAIL    : AmplitudeModulator<Complex>" << std::endl;
	}
	else
		std::cout << "SUCCESS : AmplitudeModulator<Complex>" << std::endl;

	//Testing FrequencyModulator
	if (!TestFrequencyModulatorReal()) {
		succeeded = false;
		std::cout << "FAIL    : FrequencyModulator<Real>" << std::endl;
	}
	else
		std::cout << "SUCCESS : FrequencyModulator<Real>" << std::endl;
	if (!TestFrequencyModulatorComplex()) {
		succeeded = false;
		std::cout << "FAIL    : FrequencyModulator<Complex>" << std::endl;
	}
	else
		std::cout << "SUCCESS : FrequencyModulator<Complex>" << std::endl;

	//Testing Fir filters
	if (!TestFirNoSimdImplReal()) {
		succeeded = false;
		std::cout << "FAIL    : FirNoSimdImpl<Real>" << std::endl;
	}
	else
		std::cout << "SUCCESS : FirNoSimdImpl<Real>" << std::endl;
	if (!TestFirNoSimdImplComplex()) {
		succeeded = false;
		std::cout << "FAIL    : FirNoSimdImpl<Complex>" << std::endl;
	}
	else
		std::cout << "SUCCESS : FirNoSimdImpl<Complex>" << std::endl;
	if (!TestFirSimdImplReal()) {
		succeeded = false;
		std::cout << "FAIL    : FirSimdImpl<Real>" << std::endl;
	}
	else
		std::cout << "SUCCESS : FirSimdImpl<Real>" << std::endl;
	if (!TestFirSimdImplComplex()) {
		succeeded = false;
		std::cout << "FAIL    : FirSimdImpl<Complex>" << std::endl;
	}
	else
		std::cout << "SUCCESS : FirSimdImpl<Complex>" << std::endl;

	return succeeded;
}