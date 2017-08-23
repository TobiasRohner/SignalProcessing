#ifndef CONFIG_H_
#define CONFIG_H_


/**
 * \file config.h
 * \brief The project configuration file.
 */


/**
 * \def DOUBLE_PRECISION
 * \brief Use single or double precision floating point arithmetic.
 *
 * If `DOUBLE_PRECISION` is set to a truthly value, all signals will be using double precision floating point arithmetic.
 */
#define DOUBLE_PRECISION 1


/**
 * \def ASSERT_FILTER_OUTPUT_TYPE
 * \brief Assert the output type of a filter when requesting a value.
 *
 * If `ASSERT_FILTER_OUTPUT_TYPE` is set to a truthy value, every time an output value is requested from a filter,
 * it asserts that the right type of output value is requested, e.g. If Filter::GetReal() is called on a filter
 * with complex output, the program will terminate.
 */
#define ASSERT_FILTER_OUTPUT_TYPE 1


#endif //CONFIG_H_