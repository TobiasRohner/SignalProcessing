#ifndef TEST_TEST_AMPLITUDE_MODULATOR_H_
#define TEST_TEST_AMPLITUDE_MODULATOR_H_

#include <types.h>
#include <filter/amplitude_modulator.h>
#include <filter/vector_input.h>
#include <filter/vector_output.h>
#include <vector>


/**
* \file test/test_amplitude_modulator.h
* \brief This file contains tests for the \ref AmplitudeModulator.
*/


/**
* \brief Test \ref AmplitudeModulator<Real>
* \returns A bool wether the test was successful.
*/
inline bool TestAmplitudeModulatorReal()
{
	std::vector<Real> test_input({ -0.75, -0.75, -0.75, -0.75, -0.25, -0.25, -0.25, -0.25, 0.25, 0.25, 0.25, 0.25, 0.75, 0.75, 0.75, 0.75 });
	std::vector<Real> expected_output({ -0.75, 0, 0.75, 0, -0.25, 0, 0.25, 0, 0.25, 0, -0.25, 0, 0.75, 0, -0.75, 0 });
	VectorInput<Real> input(4, test_input);
	AmplitudeModulator<Real> modulator(4, 1);
	VectorOutput<Real> output(4);
	modulator.SetInput(0, { static_cast<const Filter*>(&input), 0 });
	output.SetInput(0, { static_cast<const Filter*>(&modulator), 0 });
	for (int i = 0; i < (int)test_input.size() / RealSimd::static_size; ++i) {
		input.Update();
		modulator.Update();
		output.Update();
	}
	return output.GetStored() == expected_output;
}


/**
* \brief Test \ref AmplitudeModulator<Complex>
* \returns A bool wether the test was successful.
*/
inline bool TestAmplitudeModulatorComplex()
{
	std::vector<Real> test_input({ -0.75, -0.75, -0.75, -0.75, -0.25, -0.25, -0.25, -0.25, 0.25, 0.25, 0.25, 0.25, 0.75, 0.75, 0.75, 0.75 });
	std::vector<Complex> expected_output({ Complex(-0.75, 0), Complex(0, -0.75), Complex(0.75, 0), Complex(0, 0.75),
		Complex(-0.25, 0), Complex(0, -0.25), Complex(0.25, 0), Complex(0, 0.25),
		Complex(0.25, 0), Complex(0, 0.25), Complex(-0.25, 0), Complex(0, -0.25),
		Complex(0.75, 0), Complex(0, 0.75), Complex(-0.75, 0), Complex(0, -0.75) });
	VectorInput<Real> input(4, test_input);
	AmplitudeModulator<Complex> modulator(4, 1);
	VectorOutput<Complex> output(4);
	modulator.SetInput(0, { static_cast<const Filter*>(&input), 0 });
	output.SetInput(0, { static_cast<const Filter*>(&modulator), 0 });
	for (int i = 0; i < (int)test_input.size() / RealSimd::static_size; ++i) {
		input.Update();
		modulator.Update();
		output.Update();
	}
	return output.GetStored() == expected_output;
}


#endif //TEST_TEST_AMPLITUDE_MODULATOR