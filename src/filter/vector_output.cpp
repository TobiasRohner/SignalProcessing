#include <filter/vector_output.h>


//Force the compiler to generate real and complex versions of VectorOutput
template class VectorOutput<Real>;
template class VectorOutput<Complex>;


template<typename Tin>
VectorOutput<Tin>::VectorOutput(double sample_rate) :
	Filter(sample_rate),
	output(0)
{
	AddInput({ nullptr, 0 });
}


///\cond IMPLEMENTATION_DETAILS
template<>
void VectorOutput<Real>::Update()
{
	auto in = GetRealInput(0);
	for (int i = 0; i < RealSimd::static_size; ++i)
		output.push_back(in[i]);
}
///\endcond


///\cond IMPLEMENTATION_DETAILS
template<>
void VectorOutput<Complex>::Update()
{
	auto in = GetComplexInput(0);
	for (int i = 0; i < RealSimd::static_size; ++i)
		output.push_back(Complex(in.first[i], in.second[i]));
}
///\endcond


template<typename Tin>
const std::vector<Tin>& VectorOutput<Tin>::GetStored() const
{
	return output;
}