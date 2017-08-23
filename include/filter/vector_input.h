#ifndef FILTER_VECTOR_INPUT_H_
#define FILTER_VECTOR_INPUT_H_

#include <types.h>
#include <filter/filter.h>
#include <vector>


/**\ingroup group_filters
 * \brief Outputs data stored in a `std::vector`.
 *
 * When the end of the data is reached, the output loops back to the beginning of the data.
 *
 * \tparam Tout The type of signal this filter outputs. Either has to be \ref Real or \ref Complex.
 */
template<typename Tout>
class VectorInput : public Filter
{
public:
	/**
	 * \brief Constructor
	 * \param sample_rate The number of samples per second this filter outputs.
	 */
	VectorInput(uint32_t sample_rate);

	/**
	 * \brief Constructor
	 * \param sample_rate The number of samples per second this filter outputs.
	 * \param out A vector storing the data to output.
	 */
	VectorInput(uint32_t sample_rate, const std::vector<Tout>& out);


	/**
	 * \brief Update the output of this filter.
	 *
	 * If the end of the data is reached, it loops around.
	 */
	virtual void Update();


	/**
	 * \brief Sets the data this filter outputs.
	 *
	 * \param out The data this filter outputs.
	 */
	void SetInput(const std::vector<Tout>& out);

private:
	std::vector<Tout> output_samples;	///< A copy of the data to output.

	uint32_t current_sample = 0;		///< The index of the current sample.


	/**
	 * \brief Update \ref current_sample to point to the sample that has to be returned next.
	 */
	void NextSample();
};


//---------- inlined / templated functions implementation ----------//

template<typename Tout>
inline void VectorInput<Tout>::NextSample()
{
	current_sample = (current_sample + 1) % output_samples.size();
}


#endif //FILTER_VECTOR_INPUT_H_