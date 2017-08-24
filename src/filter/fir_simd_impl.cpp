#include <filter/fir_simd_impl.h>
#include <utils.h>
#include <type_traits>


//Force the compiler to generate a Real and a Complex version of the class
template class FirSimdImpl<Real>;
template class FirSimdImpl<Complex>;


template<>
FirSimdImpl<Real>::FirSimdImpl(uint32_t sample_rate, const std::vector<Real>& coeff) :
	Filter(sample_rate, std::vector<Filter::FilterOutputType>(1, Filter::FilterOutputType::REAL))
{
	AddInput({ nullptr, 0 });
	SetCoefficients(coeff);
}


template<>
FirSimdImpl<Complex>::FirSimdImpl(uint32_t sample_rate, const std::vector<Real>& coeff) :
	Filter(sample_rate, std::vector<Filter::FilterOutputType>(1, Filter::FilterOutputType::COMPLEX))
{
	AddInput({ nullptr, 0 });
	SetCoefficients(coeff);
}


template<typename Tin>
void FirSimdImpl<Tin>::Update()
{
	UpdateInputBuffers();
	CalculateOutputs();
}


template<typename Tin>
void FirSimdImpl<Tin>::SetCoefficients(const std::vector<Real>& coeff)
{
	//The length of the buffer and coefficient arrays
	const int arraylength = ceilto<int>(RealSimd::static_size, coeff.size());
	//Resize the coefficient vectors
	for (auto& cv : coefficients) {
		cv.clear();
		cv.resize(arraylength, Real(0));
	}
	//Resize the input buffers
	buffer_real = BufferType(arraylength, Real(0));
	if (std::is_same<Tin, Complex>::value)	//Only allocate space for the imaginary buffer if the input signal is indeed complex
		buffer_imag = BufferType(arraylength, Real(0));
	//Set the coefficients
	for (int i = 0; i < (int)coefficients.size(); ++i) {
		for (int j = 0; j < (int)coeff.size(); ++j)
			coefficients[i][(i + j) % coefficients[i].size()] = coeff[coeff.size() - j - 1];	//Save the coefficients in reversed order
	}
	//Update the deltaoffset variable
	deltaoffset = arraylength - (uint32_t)coeff.size();
}