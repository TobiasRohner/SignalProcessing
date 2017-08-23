#ifndef TYPES_H_
#define TYPES_H_

#include <config.h>
#include <boost/simd/pack.hpp>
#include <boost/simd/complex.hpp>
#include <boost/simd/type/complex.hpp>
#include <boost/align/aligned_allocator.hpp>
#include <Eigen/Dense>
#include <complex>
#include <cstdint>
#include <vector>
#include <boost/circular_buffer.hpp>

/**
 * \file types.h
 * \brief Aliases for all frequently used types in this project.
 */


/**
 * \typedef Real
 * \brief The type of floating point arithmetic to use throughout this project.
 *
 * If `DOUBLE_PRECISION` is set to a truthy value, this is defined as `double`, else it will be of type `float`.
 */
#if DOUBLE_PRECISION
using Real = double;
#else
using Real = float;
#endif

/**
 * \brief The type of complex valued signals.
 */
using Complex = std::complex<Real>;

/**
 * \brief The type used for SIMD operations on real signals.
 */
using RealSimd = boost::simd::pack<Real>;

/**
 * \brief The type used for SIMD operations on complex valued signals.
 */
using ComplexSimd = boost::simd::complex<RealSimd>;


/**
 * \brief An Eigen vector of \ref Real type.
 */
using VecXReal = Eigen::Matrix<Real, Eigen::Dynamic, 1>;

/**
 * \brief An Eigen vector of \ref Complex type.
 */
using VecXComplex = Eigen::Matrix<Complex, Eigen::Dynamic, 1>;

/**
 * \brief An Eigen matrix of \ref Real type.
 */
using MatrixXReal = Eigen::Matrix<Real, Eigen::Dynamic, Eigen::Dynamic>;

/**
 * \brief An Eigen matrix of \ref Complex type.
 */
using MatrixXComplex = Eigen::Matrix<Complex, Eigen::Dynamic, Eigen::Dynamic>;

/**
 * \brief A 1-dimensional Eigen array of \ref Real type.
 */
using ArrayXReal = Eigen::Array<Real, Eigen::Dynamic, 1>;

/**
 * \brief A 1-dimensional Eigen array of \ref Complex type.
 */
using ArrayXComplex = Eigen::Array<Complex, Eigen::Dynamic, 1>;

/**
 * \brief A 2-dimensional Eigen array of \ref Real type.
 */
using ArrayXXReal = Eigen::Array<Real, Eigen::Dynamic, Eigen::Dynamic>;

/**
 * \brief A 2-dimensional Eigen array of \ref Complex type.
 */
using ArrayXXComplex = Eigen::Array<Complex, Eigen::Dynamic, Eigen::Dynamic>;


/**
 * \brief A `std::vector` with a custom memory allocator such that its contents can be memory aligned.
 *
 * \tparam T The type of the elements stores inside the vector.
 * \tparam alignment The memory alignment used for this vector. This parameter defaults to
 *                   the minimal alignment needed for aligned loading of the values into a SIMD Register of given type.
 */
template<typename T, uint64_t alignment = boost::simd::pack<T>::alignment>
using AlignedStdVec = std::vector<T, boost::alignment::aligned_allocator<T, alignment>>;

/**
 * \brief A `boost::circular_buffer` with a custom memory allocator such that its contents can be memory aligned.
 *
 * \tparam T The type of the elements stored inside the buffer.
 * \tparam alignment The memory alignment used for this buffer. This parameter defaults to
 *                   the minimal alignment needed for aligned loading of the values into a SIMD Register of given type.
 */
template<typename T, uint64_t alignment = boost::simd::pack<T>::alignment>
using AlignedCircBuf = boost::circular_buffer<T, boost::alignment::aligned_allocator<T, alignment>>;


/**
 * \typedef as_simd<T>
 * \brief Convert the scalar types \ref Real and \ref Complex to their SIMD Register counterparts.
 *
 * If `T` is equal to \ref Real, \ref RealSimd will be returned.
 * If `T` is equal to \ref Complex, \ref ComplexSimd will be returned.
 * Everything else will be left as is.
 *
 * \tparam T The type to convert to its SIMD Register counterpart.
 */
///\cond IMPLEMENTATION_DETAILS
template<typename T> struct as_simd_impl { using type = T; };
template<> struct as_simd_impl<Real> { using type = RealSimd; };
template<> struct as_simd_impl<Complex> { using type = ComplexSimd; };
///\endcond
template<typename T> using as_simd = typename as_simd_impl<T>::type;


/**
 * \typedef remove_simd<T>
 * \brief Convert the SIMD Register types \ref RealSimd and \ref ComplexSimd to their scalar counterparts.
 *
 * If `T` is equal to \ref RealSimd, \ref Real will be returned.
 * If `T` is equal to \ref ComplexSimd, \ref Complex will be returned.
 * Everything else will be left as is.
 *
 * \tparam T The type to convert to its scalar counterpart.
 */
///\cond IMPLEMENTATION_DETAILS
template<typename T> struct remove_simd_impl { using type = T; };
template<> struct remove_simd_impl<RealSimd> { using type = Real; };
template<> struct remove_simd_impl<ComplexSimd> { using type = Complex; };
///\endcond
template<typename T> using remove_simd = typename remove_simd_impl<T>::type;

#endif //TYPES_H_