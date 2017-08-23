#ifndef FILTER_FIR_NO_SIMD_IMPL_H_
#define FILTER_FIR_NO_SIMD_IMPL_H_

#include <cstdint>
#include <filter/filter.h>
#include <types.h>
#include <vector>
#include <boost/circular_buffer.hpp>


///\cond IMPLEMENTATION_DETAILS
template<typename Tin>
class FirNoSimdImpl : public Filter
{
public:
	FirNoSimdImpl(uint32_t sample_rate, const std::vector<Real>& coeff);

	virtual void Update();

protected:
	void SetCoefficients(const std::vector<Real>& coeff);

private:
	std::vector<Real> coefficients;
	boost::circular_buffer<Real> buffer_real;
	boost::circular_buffer<Real> buffer_imag;
};
///\endcond


#endif //FILTER_FIR_NO_SIMD_IMPL_H_