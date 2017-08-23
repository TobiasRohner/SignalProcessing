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
	 */
	Filter(uint32_t sample_rate);

	/**
	 * \brief Constructor
	 *
	 * \param sample_rate The number of samples per second of the input to this filter.
	 * \param outputs A vector describing the output types of the filter's outputs.
	 */
	Filter(uint32_t sample_rate, const std::vector<FilterOutputType>& outputs);

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
	 * \brief Get the sample rate of the output of this filter.
	 *
	 * \returns The number of samples per second this filter provides at its outputs.
	 */
	uint32_t SampleRate() const;

	/**
	 * \brief Get the nyquist frequency of the output of this filter.
	 *
	 * \returns The nyquist frequency of the outputs of this filter.
	 */
	Real Nyquist() const;


	/**
	 * \brief Get the time between two samples.
	 *
	 * \returns The time passing between two samples at the outputs of this filter.
	 *          This is equivalent to `1.0 / SampleRate()`.
	 */
	double Deltatime() const;

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
	const uint32_t sample_rate;
	const Real nyquist;
	const double deltatime;

	AlignedStdVec<Real> output_real;
	AlignedStdVec<Real> output_imag;
	#if ASSERT_FILTER_OUTPUT_TYPE
	std::vector<FilterOutputType> output_type;		// Stores the output type of every filter output
	#endif //ASSERT_FILTER_OUTPUT_TYPE

	//Stores a pointer to the filter which is an input to this filter. The size_t indicates the index of the output used as an input to this filter
	std::vector<std::pair<const Filter*, size_t>> input;
};


//---------- inlined / templated functions implementation ----------//

inline uint32_t Filter::SampleRate() const
{
	return sample_rate;
}


inline Real Filter::Nyquist() const
{
	return nyquist;
}


inline double Filter::Deltatime() const
{
	return deltatime;
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