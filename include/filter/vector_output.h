#ifndef FILTER_VECTOR_OUTPUT_H_
#define FILTER_VECTOR_OUTPUT_H_

#include <types.h>
#include <filter/filter.h>

#include <vector>


/**\ingroup group_filters
 * \brief Stores all input it receives inside a `std::vector`.
 *
 * \tparam Tin The type of input this filter accepts. Either has to be \ref Real or \ref Complex.
 */
template<typename Tin>
class VectorOutput : public Filter
{
public:
	/**
	 * \brief Constructor
	 *
	 * \param sample_rate The number of samples per second produced by the input to this filter.
	 */
	VectorOutput(double sample_rate);


	/**
	 * \brief Update this filter.
	 *
	 * Stores the avaliable inputs to this filter inside a vector.
	 */
	virtual void Update();


	/**
	 * \brief Access the stored samples.
	 * \returns A const reference to the vector storing the samples.
	 */
	const std::vector<Tin>& GetStored() const;

private:
	std::vector<Tin> output;	///< All samples are stored in this vector.
};


#endif //FILTER_VECTOR_OUTPUT_H_