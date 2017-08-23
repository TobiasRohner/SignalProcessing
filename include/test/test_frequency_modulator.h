#ifndef TEST_TEST_FREQUENCY_MODULATOR_H_
#define TEST_TEST_FREQUENCY_MODULATOR_H_

#include <types.h>
#include <filter/frequency_modulator.h>
#include <filter/vector_input.h>
#include <filter/vector_output.h>
#include <vector>
#include <utils.h>


inline bool TestFrequencyModulatorReal()
{
	std::vector<Real> test_input({ -1, -1, -1, -1, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0 });
	std::vector<Real> expected_output({ 1, 0, -1, 0, 1, 1, 1, 1, 1, 0, -1, 0, 0, 0, 0, 0 });
	VectorInput<Real> input(4, test_input);
	FrequencyModulator<Real> modulator(4, 0, 2);
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


inline bool TestFrequencyModulatorComplex()
{
	std::vector<Real> test_input({ -1, -1, -1, -1, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0 });
	std::vector<Complex> expected_output({ Complex(1, 0), Complex(0, -1), Complex(-1, 0), Complex(0, 1),
		Complex(1, 0), Complex(1, 0), Complex(1, 0), Complex(1, 0),
		Complex(1, 0), Complex(0, 1), Complex(-1, 0), Complex(0, -1),
		Complex(0, -1), Complex(0, -1), Complex(0, -1), Complex(0, -1) });
	VectorInput<Real> input(4, test_input);
	FrequencyModulator<Complex> modulator(4, 0, 2);
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


#endif //TEST_TEST_FREQUENCY_MODULATOR_H_