#include <filter/fir_no_simd_impl.h>


//Force the compiler to generate a Real and a Complex versiond of FirNoSimdImpl
template class FirNoSimdImpl<Real>;
template class FirNoSimdImpl<Complex>;


template<>
FirNoSimdImpl<Real>::FirNoSimdImpl(uint32_t sample_rate, const std::vector<Real>& coeff) :
	Filter(sample_rate, std::vector<Filter::FilterOutputType>(1, Filter::FilterOutputType::REAL)),
	coefficients(coeff.crbegin(), coeff.crend()),
	buffer_real(coeff.size(), Real(0))
{
	AddInput({ nullptr, 0 });
}


template<>
FirNoSimdImpl<Complex>::FirNoSimdImpl(uint32_t sample_rate, const std::vector<Real>& coeff) :
	Filter(sample_rate, std::vector<Filter::FilterOutputType>(1, Filter::FilterOutputType::COMPLEX)),
	coefficients(coeff.crbegin(), coeff.crend()),
	buffer_real(coeff.size(), Real(0)),
	buffer_imag(coeff.size(), Real(0))
{
	AddInput({ nullptr, 0 });
}


template<>
void FirNoSimdImpl<Real>::Update()
{
	const Real* in = GetRealInput(0);
	for (int sample = 0; sample < RealSimd::static_size; ++sample) {
		//Update the input buffers
		buffer_real.push_back(in[sample]);
		//Perform the convolution
		Real re = 0;
		for (int i = 0; i < static_cast<int>(coefficients.size()); ++i)
			re += buffer_real[i] * coefficients[i];
		//Store the result of the convolution to the output
		real()[sample] = re;
	}
}


template<>
void FirNoSimdImpl<Complex>::Update()
{
	auto in = GetComplexInput(0);
	for (int sample = 0; sample < RealSimd::static_size; ++sample) {
		//Update the input buffers
		buffer_real.push_back(in.first[sample]);
		buffer_imag.push_back(in.second[sample]);
		//Perform the convolution
		Real re = 0, im = 0;
		for (int i = 0; i < static_cast<int>(coefficients.size()); ++i) {
			re += buffer_real[i] * coefficients[i];
			im += buffer_imag[i] * coefficients[i];
		}
		//Store the result of the convolution to the output
		real()[sample] = re;
		imag()[sample] = im;
	}
}


template<>
void FirNoSimdImpl<Real>::SetCoefficients(const std::vector<Real>& coeff)
{
	//Update the coefficients
	coefficients.clear();
	coefficients.reserve(coeff.size());
	for (auto it = coeff.crbegin(); it != coeff.crend(); ++it)
		coefficients.push_back(*it);
	//Update the input buffer
	buffer_real = boost::circular_buffer<Real>(coeff.size(), Real(0));
}


template<>
void FirNoSimdImpl<Complex>::SetCoefficients(const std::vector<Real>& coeff)
{
	//Update the coefficients
	coefficients.clear();
	coefficients.reserve(coeff.size());
	for (auto it = coeff.crbegin(); it != coeff.crend(); ++it)
		coefficients.push_back(*it);
	//Update the input buffer
	buffer_real = boost::circular_buffer<Real>(coeff.size(), Real(0));
	buffer_imag = boost::circular_buffer<Real>(coeff.size(), Real(0));
}