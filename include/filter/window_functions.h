#ifndef FILTER_WINDOW_FUNCTIONS_H_
#define FILTER_WINDOW_FUNCTIONS_H_

#include <functional>
#include <cmath>
#include <cstdint>
#include <boost/math/constants/constants.hpp>
#include <boost/math/special_functions/factorials.hpp>
#include <types.h>
#include <algorithm>


/**
 * \file filter/window_functions.h
 * \brief Window Function definitions
 */


///\cond IMPLEMENTATION_DETAILS
inline Real BSpline_impl(uint64_t order, Real x)
{
	double result = 0;
	for (uint64_t k = 0; k < (order + 1) / 2; ++k) {
		double m1_pow_k = k % 2 == 0 ? 1 : -1;		//(-1)^k
		double m1_pow_np1mk = (order + 1 - k) % 2 == 0 ? 1 : -1;	//(-1)^(order + 1 - k)
		double summand_coefficient = (order + 1) / boost::math::factorial<double>((unsigned int)k) / boost::math::factorial<double>((unsigned int)(order + 1 - k));
		result += summand_coefficient * m1_pow_k * std::pow(std::max(0.0, (double)x - k), order);
		result += summand_coefficient * m1_pow_np1mk * std::pow(std::max(0.0, (double)x - order - 1 + k), order);
	}
	if (order % 2 == 1) {
		uint64_t k = (order + 1) / 2;
		double m1_pow_k = k % 2 == 0 ? 1 : -1;
		double summand_coefficient = (order + 1) / boost::math::factorial<double>((unsigned int)k) / boost::math::factorial<double>((unsigned int)(order + 1 - k));
		result += summand_coefficient * m1_pow_k * std::pow(std::max(0.0, (double)x - k), order);
	}
	return Real(result);
}

inline Real Sine_impl(uint64_t power, Real x)
{
	return static_cast<Real>(std::pow(std::sin(x * boost::math::constants::pi<Real>()), power));
}
///\endcond


/**
 * \brief Rectangular Window
 *
 * \param x A value in the interval [0, 1]
 * \returns The magnitude of the Rectangular window at `x`
 */
std::function<Real(Real)> Rectangular([&](Real x)->Real
{
	return x >= 0 && x <= 1 ? Real(1) : Real(0);
});


/**
 * \brief B-Spline Window
 *
 * This is a generator function for B-Splines of n-th order.
 *
 * \param order The order of the B-Spline window
 * \returns A `std::function` mapping values in the interval [0, 1] to a B-Spline of order `order`.
 */
std::function<Real(Real)> BSpline(uint64_t order)
{
	return std::bind(BSpline_impl, order, std::placeholders::_1);
}


/**
 * \brief Hamming Window
 *
 * \param x A value in the Interval [0, 1]
 * \returns The magnitude of the Hamming window at position `x`.
 */
std::function<Real(Real)> Hamming([&](Real x)->Real
{
	constexpr Real alpha = Real(25) / 46;
	constexpr Real beta = Real(1) - alpha;
	return static_cast<Real>(alpha - beta * std::cos(x * boost::math::constants::two_pi<Real>()));
});


/**
 * \brief Parzen Window
 *
 * \param x A value in the interval [0, 1]
 * \returns The magnitude of the Parzen window at position `x`.
 */
std::function<Real(Real)> Parzen(BSpline(3));


/**
 * \brief Sine Window
 *
 * This is a generator function for window functions of the form `sin(pi*x)^n`
 *
 * \param power The power to raise the sine to.
 * \returns A window function mapping the interval [0, 1] to the magnitudes of the Sind window.
 */
std::function<Real(Real)> Sine(uint64_t power)
{
	return std::bind(Sine_impl, power, std::placeholders::_1);
}


/**
 * \brief Hann Window
 *
 * \param x A value in the interval [0, 1]
 * \returns The magnitude of the Hann window at position `x`
 */
std::function<Real(Real)> Hann(Sine(2));


/**
 * \brief Triangular Window
 *
 * \param x A value in the interval [0, 1]
 * \returns The magnitude of the Triangular window at position `x`
 */
std::function<Real(Real)> Triangular(BSpline(1));


/**
 * \brief Welch Window
 *
 * \param x A value in the interval [0, 1]
 * \returns The magnitude of the Welch window at position `x`
 */
std::function<Real(Real)> Welch([&](Real x)->Real
{
	Real shifted = x - Real(0.5);
	return Real(1.0) - 4 * shifted*shifted;
});



#endif //FILTER_WINDOW_FUNCTIONS_H_