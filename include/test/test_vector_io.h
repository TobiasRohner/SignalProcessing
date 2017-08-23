#ifndef TEST_TEST_VECTOR_IO_H_
#define TEST_TEST_VECTOR_IO_H_

#include <filter/vector_input.h>
#include <filter/vector_output.h>
#include <types.h>


/**
 * \file test/test_vector_io.h
 * \brief This file includes tests for the \ref VectorInput and \ref VectorOutput filters.
 */


/**
 * \brief Tests the \ref VectorInput<Real> and \ref VectorOutput<Real> filters.
 * \returns A bool indicating if the test succeeded.
 */
inline bool TestVectorIOReal()
{
	std::vector<Real> test_input({ 0, 1, 2, 3, 4, 5, 6, 7 });
	std::vector<Real> expected_output({ 0, 1, 2, 3, 4, 5, 6, 7, 0, 1, 2, 3, 4, 5, 6, 7 });
	VectorInput<Real> input(1, test_input);
	VectorOutput<Real> output(1);
	output.SetInput(0, { static_cast<const Filter*>(&input), 0 });
	for (int i = 0; i < 2 * (int)test_input.size() / RealSimd::static_size; ++i) {
		input.Update();
		output.Update();
	}
	return output.GetStored() == expected_output;
}

/**
 * \brief Tests the \ref VectorInput<Complex> and \ref VectorOutput<Complex> filters.
 * \returns A bool indicating if the test succeeded.
 */
inline bool TestVectorIOComplex()
{
	std::vector<Complex> test_input({ Complex(0, 0), Complex(0, 1), Complex(0, 2), Complex(0, 3),
		Complex(1, 0), Complex(1, 1), Complex(1, 2), Complex(1, 3) });
	std::vector<Complex> expected_output({ Complex(0, 0), Complex(0, 1), Complex(0, 2), Complex(0, 3),
		Complex(1, 0), Complex(1, 1), Complex(1, 2), Complex(1, 3),
		Complex(0, 0), Complex(0, 1), Complex(0, 2), Complex(0, 3),
		Complex(1, 0), Complex(1, 1), Complex(1, 2), Complex(1, 3) });
	VectorInput<Complex> input(1, test_input);
	VectorOutput<Complex> output(1);
	output.SetInput(0, { static_cast<const Filter*>(&input), 0 });
	for (int i = 0; i < 2 * (int)test_input.size() / RealSimd::static_size; ++i) {
		input.Update();
		output.Update();
	}
	return output.GetStored() == expected_output;
}


#endif //TEST_TEST_VECTOR_IO_H_