#ifndef FILTER_FIR_SIMD_IMPL_H_
#define FILTER_FIR_SIMD_IMPL_H_

#include <cstdint>
#include <filter/filter.h>
#include <types.h>
#include <array>
#include <boost/circular_buffer.hpp>
#include <cstring>
#include <boost/simd/reduction.hpp>


///\cond IMPLEMENTATION_DETAILS
template<typename Tin>
class FirSimdImpl : public Filter		//FIXME: This filter produces the wrong results (Output samples are in the wrong order)
{
public:
	FirSimdImpl(double sample_rate, const std::vector<Real>& coeff);

	virtual void Update();

protected:
	void SetCoefficients(const std::vector<Real>& coefficients);

private:
	/**
	 * \brief An array of coefficient vectors.
	 *
	 * The coefficient vectors are organized in such a way, that the first coefficient
	 * of the i-th coefficient vector is offset for aligned loading into a SIMD register
	 * and it will be the i-th element in this register.
	 */
	std::array<AlignedStdVec<Real>, RealSimd::static_size> coefficients;

	/**
	 * \brief The type used as input buffer.
	 */
	using BufferType = AlignedCircBuf<Real, RealSimd::alignment>;

	BufferType buffer_real;		///< The input buffer for the real part of the input.
	BufferType buffer_imag;		///< The inout buffer for the complex part of the input.

	uint32_t offset = 0;		///< The offset the currently first sample has in the buffers.
	uint32_t deltaoffset;		///< The difference between the tapcount of the filter and the number of samples stored in a buffer


	void UpdateInputBuffers();
	void CalculateOutputs();
};


template<>
inline void FirSimdImpl<Real>::UpdateInputBuffers()
{
	for (int i = 0; i < RealSimd::static_size; ++i)
		buffer_real.push_back(GetRealInput(0)[i]);
}


template<>
inline void FirSimdImpl<Complex>::UpdateInputBuffers()
{
	for (int i = 0; i < RealSimd::static_size; ++i) {
		auto in = GetComplexInput(0);
		buffer_real.push_back(in.first[i]);
		buffer_imag.push_back(in.second[i]);
	}
}


template<>
inline void FirSimdImpl<Real>::CalculateOutputs()
{
	//Iterate over every element of the output register
	for (int i = 0; i < RealSimd::static_size; ++i) {
		Real sum = 0;
		for (int j = 0; j < (int)buffer_real.size() / RealSimd::static_size; ++j) {
			RealSimd samples = boost::simd::aligned_load<RealSimd>(&(buffer_real[j * RealSimd::static_size]));
			RealSimd coeff = boost::simd::aligned_load<RealSimd>(&(coefficients[(offset + i) % RealSimd::static_size][j * RealSimd::static_size]));
			sum += boost::simd::sum(samples * coeff);
		}
		real(0)[i] = sum;
	}
	//Update the offset
	offset = (offset + deltaoffset) % RealSimd::static_size;
}


template<>
inline void FirSimdImpl<Complex>::CalculateOutputs()
{
	//TODO
}
///\endcond


#endif //FILTER_FIR_SIMD_IMPL_H_