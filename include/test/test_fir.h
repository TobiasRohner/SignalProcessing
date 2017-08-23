#ifndef TEST_TEST_FIR_H_
#define TEST_TEST_FIR_H_

#include <types.h>
#include <filter/vector_input.h>
#include <filter/fir_no_simd_impl.h>
#include <filter/fir_simd_impl.h>
#include <filter/vector_output.h>
#include <vector>


/**
* \file test/test_fir.h
* \brief This file contains tests for the \ref Fir or respectively \ref FirSimdImpl and \ref FirNoSimdImpl classes.
*/


/**
* \brief Test \ref FirNoSimdImpl<Real>
* \returns A bool wether the test was successful.
*/
inline bool TestFirNoSimdImplReal()
{
	std::vector<Real> coefficients({ 1, 0, -1 });
	std::vector<Real> test_input({ 0, 0, 0, 0, 0.5, 1, 1.5, 2, 1.75, 1.5, 1.25, 1, 0.75, 0.5, 0.25, 0 });
	std::vector<Real> expected_output({ 0, 0, 0, 0, 0.5, 1, 1, 1, 0.25, -0.5, -0.5, -0.5, -0.5, -0.5, -0.5, -0.5 });
	VectorInput<Real> input(1, test_input);
	FirNoSimdImpl<Real> filter(1, coefficients);
	VectorOutput<Real> output(1);
	filter.SetInput(0, { static_cast<const Filter*>(&input), 0 });
	output.SetInput(0, { static_cast<const Filter*>(&filter), 0 });
	for (int i = 0; i < (int)test_input.size() / RealSimd::static_size; ++i) {
		input.Update();
		filter.Update();
		output.Update();
	}
	return output.GetStored() == expected_output;
}


/**
* \brief Test \ref FirNoSimdImpl<Complex>
* \returns A bool wether the test was successful.
*/
inline bool TestFirNoSimdImplComplex()
{
	std::vector<Real> coefficients({ 1, 0, -1 });
	std::vector<Complex> test_input({ Complex(0, 0), Complex(0, 0), Complex(0, 0), Complex(0, 0),
		Complex(0.5, 0.5), Complex(1, 1), Complex(1.5, 1.5), Complex(2, 2),
		Complex(1.75, 1.75), Complex(1.5, 1.5), Complex(1.25, 1.25), Complex(1, 1),
		Complex(0.75, 0.75), Complex(0.5, 0.5), Complex(0.25, 0.25), Complex(0, 0) });
	std::vector<Complex> expected_output({ Complex(0, 0), Complex(0, 0), Complex(0, 0), Complex(0, 0),
		Complex(0.5, 0.5), Complex(1, 1), Complex(1, 1), Complex(1, 1),
		Complex(0.25, 0.25), Complex(-0.5, -0.5), Complex(-0.5, -0.5), Complex(-0.5, -0.5),
		Complex(-0.5, -0.5), Complex(-0.5, -0.5), Complex(-0.5, -0.5), Complex(-0.5, -0.5) });
	VectorInput<Complex> input(1, test_input);
	FirNoSimdImpl<Complex> filter(1, coefficients);
	VectorOutput<Complex> output(1);
	filter.SetInput(0, { static_cast<const Filter*>(&input), 0 });
	output.SetInput(0, { static_cast<const Filter*>(&filter), 0 });
	for (int i = 0 ; i < (int)test_input.size() / RealSimd::static_size ; ++i) {
		input.Update();
		filter.Update();
		output.Update();
	}
	return output.GetStored() == expected_output;
}


/**
* \brief Test \ref FirSimdImpl<Real>
* \returns A bool wether the test was successful.
*/
inline bool TestFirSimdImplReal()
{
	std::vector<Real> coefficients({ 1, 0, -1 });
	std::vector<Real> test_input({ 0, 0, 0, 0, 0.5, 1, 1.5, 2, 1.75, 1.5, 1.25, 1, 0.75, 0.5, 0.25, 0 });
	std::vector<Real> expected_output({ 0, 0, 0, 0, 0.5, 1, 1, 1, 0.25, -0.5, -0.5, -0.5, -0.5, -0.5, -0.5, -0.5 });
	VectorInput<Real> input(1, test_input);
	FirSimdImpl<Real> filter(1, coefficients);
	VectorOutput<Real> output(1);
	filter.SetInput(0, { static_cast<const Filter*>(&input), 0 });
	output.SetInput(0, { static_cast<const Filter*>(&filter), 0 });
	for (int i = 0; i < (int)test_input.size() / RealSimd::static_size; ++i) {
		input.Update();
		filter.Update();
		output.Update();
	}
	return output.GetStored() == expected_output;
}


/**
* \brief Test \ref FirSimdImpl<Complex>
* \returns A bool wether the test was successful.
*/
inline bool TestFirSimdImplComplex()
{
	//TODO
	return false;
}


#endif //TEST_TEST_FIR_H_