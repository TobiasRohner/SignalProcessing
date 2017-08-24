#ifndef FILTER_FILTER_H_
#define FILTER_FILTER_H_

#include <config.h>
#include <types.h>
#include <cstdint>
#include <vector>
#include <utility>

/**
 * \brief The base class all filters inherit from.
 *
 * This class provides an uniform interface used by all filter types.
 * It contains a vector of output values, that can be updated via the
 * virtual function \ref Update() and accessed by other filters using
 * either the function \ref GetReal() or \ref GetComplex().
 */
class Filter
{
public:
	/** \brief The type of output this filter generates. */
	enum FilterOutputType {
		REAL,		///< The filter output is real
		COMPLEX		///< The filter output is complex
	};

	/** \brief Only used for \ref Fir<Tout,tapcount> at the moment to describe a unknown number of taps at compile time. */
	enum FilterSize : int64_t {
		DYNAMIC = -1	///< The filter has a unknown size at compile time.
	};


	/**
	 * \brief Constructor
	 *
	 * \param sample_rate The number of samples per second of the input to this filter.
	 *                    Assumes the sample rate at the output is equal to that at the input.
	 */
	Filter(uint32_t sample_rate);

	/**
	 * \brief Constructor
	 *
	 * \param sample_rate_in The number of samples per second at the input to this filter.
	 * \param sample_rate out The number of samples per second at the outputs of this filter.
	 */
	Filter(uint32_t sample_rate_in, uint32_t sample_rate_out);

	/**
	 * \brief Constructor
	 *
	 * \param sample_rate The number of samples per second of the input to this filter.
	 *                    Assumes the sample rate at the output is equal to that at the input.
	 * \param outputs A vector describing the output types of the filter's outputs.
	 */
	Filter(uint32_t sample_rate, const std::vector<FilterOutputType>& outputs);

	/**
	 * \brief Constructor
	 *
	 * \param sample_rate_in The number of samples per second at the input of this filter.
	 * \param sample_rate_out The number of samples per second at the outputs of this filter.
	 * \param outputs A vector describing the output types of this filters outputs.
	 */
	Filter(uint32_t sample_rate_in, uint32_t sample_rate_out, const std::vector<FilterOutputType>& outputs);

	/**
	 * \brief Destructor
	 */
	virtual ~Filter() = default;


	/**
	 * \brief Updates the outputs of this Filter.
	 * 
	 * This function has to be overwritten by other filter implementations.
	 */
	virtual void Update() = 0;

	
	/**
	 * \brief Get a real output from this filter.
	 *
	 * \param index The index of the filter's output. If no index is provided, the function defaults to the first output.
	 * \returns A pointer to an array of Real types with size RealSimd::static_size.
	 */
	const Real* GetReal(size_t index = 0) const;

	/**
	 * \brief Get a complex output from this filter.
	 *
	 * \param index The index of the filter's output. If no index is provided, the function defaults to the first output.
	 * \returns A pair containing a pointer to the real and imaginary parts of the output. Each Field is of size RealSimd::static_size.
	 */
	std::pair<const Real*, const Real*> GetComplex(size_t index = 0) const;	


	/**
	 * \brief Sets the input specified by index to the n-th output of the given filter.
	 *
	 * \param index The index of the filters input.
	 * \param in A `std::pair` with the following contents:
	 *               - __in.first__:  A pointer to the filter which provides the input.
	 *               - __in.second__: The index of the filter's output to use as an input.
	 */
	void SetInput(size_t index, const std::pair<const Filter*, size_t>& in);

	
	/**
	 * \brief Get the sample rate of the input of this filter.
	 *
	 * \returns The number of samples per second this filter provides at its inputs.
	 */
	uint32_t SampleRateIn() const;

	/**
	 * \brief Get the sample rate at the output of this filter.
	 *
	 * \returns The number of samples per second this filter provides at its outputs
	 */
	uint32_t SampleRateOut() const;

	/**
	 * \brief Get the nyquist frequency of the input of this filter.
	 *
	 * \returns The nyquist frequency of the inputs of this filter.
	 */
	Real NyquistIn() const;

	/**
	* \brief Get the nyquist frequency of the output of this filter.
	*
	* \returns The nyquist frequency of the outputs of this filter.
	*/
	Real NyquistOut() const;


	/**
	 * \brief Get the time between two samples.
	 *
	 * \returns The time passing between two samples at the inputs of this filter.
	 *          This is equivalent to `1.0 / SampleRateIn()`.
	 */
	double DeltatimeIn() const;

	/**
	* \brief Get the time between two samples.
	*
	* \returns The time passing between two samples at the outputs of this filter.
	*          This is equivalent to `1.0 / SampleRateOut()`.
	*/
	double DeltatimeOut() const;

protected:
	/**
	 * \brief Add a new output to this filter.
	 *
	 * \param type The type returned by the new output.
	 */
	void AddOutput(FilterOutputType type);

	/**
	 * \brief Insert a new output at a specified position.
	 *
	 * \param index The position at which the new output should be inserted.
	 * \param type The type returned by the new output.
	 */
	void InsertOutput(size_t index, FilterOutputType type);

	/**
	 * \brief Remove the last output of this filter.
	 */
	void RemoveOutput();	//Removes the last output

	/**
	 * \brief Remove an output of this filter.
	 *
	 * \param index The index of the output to remove.
	 */
	void RemoveOutput(size_t index);

	
	/**
	 * \brief Get access to the output array which stores the real part of the outputs.
	 *
	 * \param index The index of the output to get access to. If no index is provided, the function defaults to the first output.
	 * \returns A pointer to a field of size RealSimd::static_size storing the output vector.
	 */
	Real* real(size_t index = 0);

	/**
	 * \brief Get access to the output array which stores the imaginary part of the outputs.
	 *
	 * \param index The index of the output to get access to. If no index is provided, the function defaults to the first output.
	 * \returns A pointer to a field of size RealSimd::static_size storing the output vector.
	 */
	Real* imag(size_t index = 0);

	
	/**
	 * \brief Add an input to this filter.
	 *
	 * \param in A `std::pair` with the following contents:
	 *               - __in.first__:  The filter to use as an input.
	 *               - __in.second__: The index of the filter's output to use as an input.
	 */
	void AddInput(const std::pair<const Filter*, size_t>& in);

	/**
	 * \brief Insert an input to this filter.
	 *
	 * \param index The index to insert this input into.
	 * \param in A `std::pair` with the following contents:
	 *               - __in.first__:  The filter to use as an input.
	 *               - __in.second__: The index of the filter's output to use as an input.
	 */
	void InsertInput(size_t index, const std::pair<const Filter*, size_t>& in);

	/**
	 * \brief Removes the last input to this filter.
	 */
	void RemoveInput();	

	/**
	 * \brief Removes an input to this filter.
	 *
	 * \param index The index of the filter's input to remove.
	 */
	void RemoveInput(size_t index);

	
	/**
	 * \brief Access a real input value.
	 *
	 * \param index The index of the input to access.
	 * \returns A pointer to an array of size RealSimd::static_size storing the input values.
	 */
	const Real* GetRealInput(size_t index) const;

	/**
	 * \brief Access a complex input value.
	 *
	 * \param index The index of the input to access.
	 * \returns A pair with `pair.first` storing a pointer to the real part and `pair.second` storing a pointer to the imaginary part of the input.
	 */
	std::pair<const Real*, const Real*> GetComplexInput(size_t index) const;

private:
	const uint32_t sample_rate_in;
	const uint32_t sample_rate_out;
	const Real nyquist_in;
	const Real nyquist_out;
	const double deltatime_in;
	const double deltatime_out;

	AlignedStdVec<Real> output_real;
	AlignedStdVec<Real> output_imag;
	#if ASSERT_FILTER_OUTPUT_TYPE
	std::vector<FilterOutputType> output_type;		// Stores the output type of every filter output
	#endif //ASSERT_FILTER_OUTPUT_TYPE

	//Stores a pointer to the filter which is an input to this filter. The size_t indicates the index of the output used as an input to this filter
	std::vector<std::pair<const Filter*, size_t>> input;
};


//---------- inlined / templated functions implementation ----------//

inline uint32_t Filter::SampleRateIn() const
{
	return sample_rate_in;
}


inline uint32_t Filter::SampleRateOut() const
{
	return sample_rate_out;
}


inline Real Filter::NyquistIn() const
{
	return nyquist_in;
}


inline Real Filter::NyquistOut() const
{
	return nyquist_out;
}


inline double Filter::DeltatimeIn() const
{
	return deltatime_in;
}


inline double Filter::DeltatimeOut() const
{
	return deltatime_out;
}


inline const Real* Filter::GetReal(size_t index) const
{
	#if ASSERT_FILTER_OUTPUT_TYPE
	// Assert the type of output if configured that way
	BOOST_ASSERT_MSG(output_type[index] == FilterOutputType::REAL, "This Filter output does not provide real types!");
	#endif //ASSERT_FILTER_OUTPUT_TYPE
	return &(output_real[index * RealSimd::static_size]);
}


inline std::pair<const Real*, const Real*> Filter::GetComplex(size_t index) const
{
	#if ASSERT_FILTER_OUTPUT_TYPE
	// Assert the type of output if configured that way
	BOOST_ASSERT_MSG(output_type[index] == FilterOutputType::COMPLEX, "This Filter output does not provide real types!");
	#endif //ASSERT_FILTER_OUTPUT_TYPE
	const Real* real = &(output_real[index * RealSimd::static_size]);
	const Real* imag = &(output_imag[index * RealSimd::static_size]);
	return std::pair<const Real*, const Real*>(real, imag);
}


inline Real* Filter::real(size_t index)
{
	return output_real.data() + index * RealSimd::static_size;
}


inline Real* Filter::imag(size_t index)
{
	return output_imag.data() + index * RealSimd::static_size;
}


inline const Real* Filter::GetRealInput(size_t index) const
{
	return input[index].first->GetReal(input[index].second);
}


inline std::pair<const Real*, const Real*> Filter::GetComplexInput(size_t index) const
{
	return input[index].first->GetComplex(input[index].second);
}


#endif //FILTER_FILTER_H_