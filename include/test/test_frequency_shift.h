#ifndef TEST_TEST_FREQUENCY_SHIFT_H_
#define TEST_TEST_FREQUENCY_SHIFT_H_

#include <types.h>
#include <filter/frequency_shift.h>
#include <filter/vector_input.h>
#include <filter/vector_output.h>


inline bool TestFrequencyShiftReal()
{
	std::vector<Real> test_input({ 1, -1, 1, -1, 1, -1, 1, -1, 1, -1, 1, -1, 1, -1, 1, -1 });
	std::vector<Complex> expected_output({ Complex(1, 0), Complex(1, 0), Complex(1, 0), Complex(1, 0),
										   Complex(1, 0), Complex(1, 0), Complex(1, 0), Complex(1, 0),
										   Complex(1, 0), Complex(1, 0), Complex(1, 0), Complex(1, 0),
										   Complex(1, 0), Complex(1, 0), Complex(1, 0), Complex(1, 0) });
	VectorInput<Real> input(2, test_input);
	FrequencyShift<Real> shift(2, -1);
	VectorOutput<Complex> output(1);
	shift.SetInput(0, { static_cast<const Filter*>(&input), 0 });
	output.SetInput(0, { static_cast<const Filter*>(&shift), 0 });
	for (int i = 0; i < (int)test_input.size() / RealSimd::static_size; ++i) {
		input.Update();
		shift.Update();
		output.Update();
	}
	return output.GetStored() == expected_output;
}


inline bool TestFrequencyShiftComplex()
{
	std::vector<Complex> test_input({ Complex(1, 0), Complex(0, 1), Complex(-1, 0), Complex(0, -1),
									  Complex(1, 0), Complex(0, 1), Complex(-1, 0), Complex(0, -1),
									  Complex(1, 0), Complex(0, 1), Complex(-1, 0), Complex(0, -1),
									  Complex(1, 0), Complex(0, 1), Complex(-1, 0), Complex(0, -1) });
	std::vector<Complex> expected_output({ Complex(1, 0), Complex(1, 0), Complex(1, 0), Complex(1, 0),
										   Complex(1, 0), Complex(1, 0), Complex(1, 0), Complex(1, 0),
										   Complex(1, 0), Complex(1, 0), Complex(1, 0), Complex(1, 0),
										   Complex(1, 0), Complex(1, 0), Complex(1, 0), Complex(1, 0) });
	VectorInput<Complex> input(4, test_input);
	FrequencyShift<Complex> shift(4, -1);
	VectorOutput<Complex> output(1);
	shift.SetInput(0, { static_cast<const Filter*>(&input), 0 });
	output.SetInput(0, { static_cast<const Filter*>(&shift), 0 });
	for (int i = 0; i < (int)test_input.size() / RealSimd::static_size; ++i) {
		input.Update();
		shift.Update();
		output.Update();
	}
	return output.GetStored() == expected_output;
}


#endif //TEST_TEST_FREQUENCY_SHIFT_H_