#ifndef UTILS_H_
#define UTILS_H_

#include <cstdint>
#include <iostream>


/**
 * \file utils.h
 * \brief Some useful functions.
 */


/**
 * \brief Signextend `n` from `N` bits.
 *
 * \tparam T The type of integer to signextend.
 * \tparam N The number of bits in the representation of `n`.
 * \param n The number to signextend.
 * \returns The signextended `n`.
 */
template<typename T, size_t N>
inline T signextend(T n)
{
	struct S { T x : N; } s;
	return s.x = n;
}


/**
 * \brief Round `r` down to the nearest multiple of `n`.
 *
 * \tparam Tout The integer type this function returns.
 * \tparam Tin The arithmetic type this function rounds down.
 * \param n The number to which's multiples is rounded down.
 * \param r The number to round down.
 * \returns `r` rounded down to the nearest multiple of `n`.
 */
template<typename Tout, typename Tin>
inline constexpr Tout floorto(Tout n, Tin r)
{
	return r > 0
		? ((r / n) * n == r
			? static_cast<Tout>(r)
			: n * static_cast<Tout>(r / n))
		: ((r / n) * n == r
			? static_cast<Tout>(r)
			: n * static_cast<Tout>(r / n) - n);
}


/**
* \brief Round `r` up to the nearest multiple of `n`.
*
* \tparam Tout The integer type this function returns.
* \tparam Tin The arithmetic type this function rounds up.
* \param n The number to which's multiples is rounded up.
* \param r The number to round up.
* \returns `r` rounded up to the nearest multiple of `n`.
*/
template<typename Tout, typename Tin>
inline constexpr Tout ceilto(Tout n, Tin r)
{
	return r > 0
		? ((r / n) * n == r
			? static_cast<Tout>(r)
			: n * static_cast<Tout>(r / n) + n)
		: ((r / n) * n == r
			? static_cast<Tout>(r)
			: n * static_cast<Tout>(r / n));
}


/**
* \brief Round `r` to the nearest multiple of `n`.
*
* \tparam Tout The integer type this function returns.
* \tparam Tin The arithmetic type this function rounds.
* \param n The number to which's multiples is rounded.
* \param r The number to round.
* \returns `r` rounded to the nearest multiple of `n`.
*/
template<typename Tout, typename Tin>
inline constexpr Tout roundto(Tout n, Tin r) //Rounds to the next multiple of n
{
	return n * floorto<Tout, double>(static_cast<double>(r) / n + 0.5, Tout(1));
}


/**
 * \brief Print a range of objects to an output stream.
 * 
 * \tparam The type of iterator.
 * \param begin The iterator to the first element.
 * \param end The iterator to after the last element.
 * \param os The output stream to stream the values to.
 */
template<typename IteratorType>
inline void printRange(IteratorType begin, IteratorType end, std::ostream& os)
{
	os << *(begin++);
	for (auto p = begin; p != end; ++p)
		os << ", " << *p;
}


#endif //UTILS_H_