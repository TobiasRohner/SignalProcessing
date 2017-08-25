#include <filter/Filter.h>



Filter::Filter(double sample_rate) :
	sample_rate(sample_rate),
	decimation_factor(1)
{
}


Filter::Filter(double sample_rate, uint32_t decimation_factor) :
	sample_rate(sample_rate),
	decimation_factor(decimation_factor)
{
}


Filter::Filter(double sample_rate, const std::vector<Filter::FilterOutputType>& outputs) :
	sample_rate(sample_rate),
	decimation_factor(1)
{
	//Find out if the filter has a complex output in order to determine wether the output_imag vector has to be used
	bool has_imaginary_output = false;
	for (FilterOutputType type : outputs)
		if (type == FilterOutputType::COMPLEX) has_imaginary_output = true;
	//Set the output vectors to their corresponding sizes
	output_real.resize(outputs.size() * RealSimd::static_size, Real(0));
	if (has_imaginary_output)
		output_imag.resize(outputs.size() * RealSimd::static_size);

	#if ASSERT_FILTER_OUTPUT_TYPE
	//If ASSERT_FILTER_OUTPUT_TYPE is defined, also initialize the output_type vector
	output_type = outputs;
	#endif //ASSERT_FILTER_OUTPUT_TYPE
}


Filter::Filter(double sample_rate, uint32_t decimation_factor, const std::vector<Filter::FilterOutputType>& outputs) :
	sample_rate(sample_rate),
	decimation_factor(decimation_factor)
{
	//Find out if the filter has a complex output in order to determine wether the output_imag vector has to be used
	bool has_imaginary_output = false;
	for (FilterOutputType type : outputs)
		if (type == FilterOutputType::COMPLEX) has_imaginary_output = true;
	//Set the output vectors to their corresponding sizes
	output_real.resize(outputs.size() * RealSimd::static_size, Real(0));
	if (has_imaginary_output)
		output_imag.resize(outputs.size() * RealSimd::static_size);

	#if ASSERT_FILTER_OUTPUT_TYPE
	//If ASSERT_FILTER_OUTPUT_TYPE is defined, also initialize the output_type vector
	output_type = outputs;
	#endif //ASSERT_FILTER_OUTPUT_TYPE
}


void Filter::AddOutput(Filter::FilterOutputType type)
{
	//Add a further element to the output vectors
	output_real.resize(output_real.size() + RealSimd::static_size, Real(0));
	output_imag.resize(output_imag.size() + RealSimd::static_size, Real(0));
	#if ASSERT_FILTER_OUTPUT_TYPE
	output_type.push_back(type);
	#endif //ASSERT_FILTER_OUTPUT_TYPE
}


void Filter::InsertOutput(size_t index, Filter::FilterOutputType type)
{
	//Insert an element into the output vectors
	output_real.insert(output_real.begin() + index, RealSimd::static_size, Real(0));
	output_imag.insert(output_imag.begin() + index, RealSimd::static_size, Real(0));
	#if ASSERT_FILTER_OUTPUT_TYPE
	output_type.insert(output_type.begin() + index, type);
	#endif //ASSERT_FILTER_OUTPUT_TYPE
}


void Filter::RemoveOutput()
{
	RemoveOutput(output_real.size() - 1);
}


void Filter::RemoveOutput(size_t index)
{
	output_real.erase(output_real.begin() + index, output_real.begin() + index + RealSimd::static_size);
	output_imag.erase(output_imag.begin() + index, output_imag.begin() + index + RealSimd::static_size);
	#if ASSERT_FILTER_OUTPUT_TYPE
	output_type.erase(output_type.begin() + index);
	#endif //ASSERT_FILTER_OUTPUT_TYPE
}


void Filter::AddInput(const std::pair<const Filter*, size_t>& in)
{
	input.push_back(in);
}


void Filter::InsertInput(size_t index, const std::pair<const Filter*, size_t>& in)
{
	input.insert(input.begin() + index, in);
}


void Filter::SetInput(size_t index, const std::pair<const Filter*, size_t>& in)
{
	input[index] = in;
}


void Filter::RemoveInput()
{
	input.pop_back();
}


void Filter::RemoveInput(size_t index)
{
	input.erase(input.begin() + index);
}