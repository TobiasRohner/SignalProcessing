#ifndef TEST_TEST_FREQUENCY_GENERATOR_H_
#define TEST_TEST_FREQUENCY_GENERATOR_H_

#include <vector>
#include <boost/math/constants/constants.hpp>
#include <types.h>
#include <filter/frequency_generator.h>
#include <filter/vector_output.h>


/**
 * \file test/test_frequency_generator.h
 * \brief This file contains tests for the \ref FrequencyGenerator.
 */


/**
 * \brief Test \ref FrequencyGenerator<Real>
 * \returns A bool wether the test was successful.
 */
inline bool TestFrequencyGeneratorReal()
{
	std::vector<Real> expected_output({ -10, 0, 10, 0, -10, 0, 10, 0, -10, 0, 10, 0, -10, 0, 10, 0 });
	FrequencyGenerator<Real> input(1, 0.25, 10, boost::math::constants::pi<Real>());
	VectorOutput<Real> output(1);
	output.SetInput(0, { static_cast<const Filter*>(&input), 0 });
	for (int i = 0; i < (int)expected_output.size() / RealSimd::static_size; ++i) {
		input.Update();
		output.Update();
	}
	return output.GetStored() == expected_output;
}


/**
 * \brief Test \ref FrequencyGenerator<Complex>
 * \returns A bool wether the test was successful.
 */
inline bool TestFrequencyGeneratorComplex()
{
	std::vector<Complex> expected_output({ Complex(-10, 0), Complex(0, -10), Complex(10, 0), Complex(0, 10),
		Complex(-10, 0), Complex(0, -10), Complex(10, 0), Complex(0, 10),
		Complex(-10, 0), Complex(0, -10), Complex(10, 0), Complex(0, 10),
		Complex(-10, 0), Complex(0, -10), Complex(10, 0), Complex(0, 10) });
	FrequencyGenerator<Complex> input(1, 0.25, 10, boost::math::constants::pi<Real>());
	VectorOutput<Complex> output(1);
	output.SetInput(0, { static_cast<const Filter*>(&input), 0 });
	for (int i = 0; i < (int)expected_output.size() / RealSimd::static_size; ++i) {
		input.Update();
		output.Update();
	}
	return output.GetStored() == expected_output;
}


#endif //TEST_TEST_FREQUENCY_GENERATOR_H_