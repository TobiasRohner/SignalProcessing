#ifndef IO_RIFF_WAVE_H_
#define IO_RIFF_WAVE_H_

#include <cstdint>
#include <iostream>
#include <fstream>
#include <vector>
#include <cstring>
#include <boost/assert.hpp>
#include <utils.h>


/**
 * \brief The baseclass used for \ref RIFFWaveIStream and \ref RIFFWaveOStream classes.
 *
 * This class implements some basic concepts used to read and write WAVE files.
 */
class RIFFWaveStream
{
	#pragma pack(push, 1)
	/**
	 * \brief The format header of a WAVE file.
	 */
	struct FMTHeader
	{
		uint32_t _RIFF            = 0x46464952;		///< Filetype identification magic number (RIFF in ASCII)
		uint32_t FileSize         = 0x00000000;		///< The size of the file starting from now on
		uint32_t _WAVE            = 0x45564157;		///< Filetype indentification magic number (WAVE in ASCII)
		uint32_t _fmt             = 0x20746D66;		///< The official start of the format header
		uint32_t FormatDataLength = 0x00000010;		///< The length of the format header
		uint16_t FormatType       = 0x0001;			///< The format type (Only type 1 is currently supported)
		uint16_t ChannelCount     = 0x0002;			///< The number of channels stored in the file
		uint32_t FrameRate        = 0x0000AC44;		///< The number of frames per second (A frame is a collection of one sample for every channel)
		uint32_t ByteRate         = 0x0002B110;		///< The byte rate of the file
		uint16_t BytesPerFrame    = 0x0002;			///< The number of bytes per frame
		uint16_t BitsPerSample    = 0x0010;			///< The number of bits per sample (Only 8 and 16 are currently supported)


		/**
		 * \brief Load the header from a given file
		 * \param file The file to read the header from.
		 */
		void load(std::ifstream& file);

		/**
		 * \brief Write the header to a given file
		 * \param file The file to write the header to.
		 */
		void save(std::ofstream& file);

		/**
		 * \brief toggle the endianness of every number stored in the header
		 */
		void toggleEndianness();
	};
	#pragma pack(pop)

	#pragma pack(push, 1)
	/**
	 * \brief The data header of a WAVE file.
	 */
	struct DataHeader
	{
		uint32_t _data    = 0x61746164;		///< The start of the data header
		uint32_t DataSize = 0x00000000;		///< The size of the data header

		/**
		 * \brief Load the header from a given file
		 * \param file The file to read the header from.
		 */
		void load(std::ifstream& file);

		/**
		* \brief Write the header to a given file
		* \param file The file to write the header to.
		*/
		void save(std::ofstream& file);

		/**
		* \brief toggle the endianness of every number stored in the header
		*/
		void toggleEndianness();
	};
	#pragma pack(pop)

public:
	/**
	 * \brief Get the file size
	 * \returns The Size of the WAVE file
	 */
	uint32_t FileSize() const;

	/**
	 * \brief Get the format type
	 * \returns The format type of the WAVE file
	 */
	uint16_t FormatType() const;

	/**
	 * \brief Get the number of channels
	 * \returns The number of channels of the WAVE file
	 */
	uint16_t ChannelCount() const;

	/**
	 * \brief Get the frame rate
	 * \returns The frame rate of the WAVE file
	 */
	uint32_t FrameRate() const;

	/**
	 * \brief Get the byte rate
	 * \returns The byte rate of the WAVE file
	 */
	uint32_t ByteRate() const;

	/**
	 * \brief Get the number of bits per sample
	 * \returns The number of bits per sample of the WAVE file
	 */
	uint16_t BitsPerSample() const;

	/**
	 * \brief Get the maximum value of a sample
	 * \returns The maximal value of a sample using the current number of bits per sample
	 */
	uint16_t MaxSampleValue() const;

	/**
	 * \brief Get the number of frames
	 * \returns The number of frames of the WAVE file
	 */
	uint32_t FrameCount() const;

protected:
	/**
	 * \brief Constructor
	 */
	RIFFWaveStream();

	FMTHeader fmt_header;		///< The format header of this WAVE file
	DataHeader data_header;		///< The data header of this WAVE file
};


/**
 * \brief A WAVE output stream to a file
 */
class RIFFWaveOStream : public RIFFWaveStream
{
public:
	/**
	 * \brief Constructor
	 * \param path The path to the file to stream the data to
	 * \param channel_count The number of channels of the WAVE file
	 */
	RIFFWaveOStream(const std::string& path, uint16_t channel_count);

	/**
	 * \brief Constructor
	 * \param path The path to the file to stream the data to
	 * \param sample_rate The number of samples per second of the final file
	 * \param channel_count The number of channels of the WAVE file
	 */
	RIFFWaveOStream(const std::string& path, uint32_t sample_rate, uint16_t channel_count);

	/**
	 * \brief Destructor
	 *
	 * Flushes and closes the output filestream.
	 */
	~RIFFWaveOStream();


	/**
	 * \brief Output stream operator
	 * \param sample a pointer to an array of \ref ChannelCount() samples.
	 */
	RIFFWaveOStream& operator<<(int16_t* sample);

	/**
	 * \brief Close the file.
	 */
	void close();


	/**
	 * \brief Set the frame rate of the file
	 * \param rate The new frame rate
	 */
	void FrameRate(uint32_t rate);

	/**
	 * \brief Set the bit depth of a sample to 8
	 */
	void set8BitsPerSample();

	/**
	 * \brief Set the bit depth of a sample to 16
	 */
	void set16BitsPerSample();

private:
	std::ofstream file;		///< The file to stream all output to


	/**
	 * \brief Write the headers to the file.
	 */
	void writeHeaders();
};


/**
 * \brief A WAVE input stream from file.
 */
class RIFFWaveIStream : public RIFFWaveStream
{
public:
	/**
	 * \brief Constructor
	 * \param path The path to the file to stream the data from.
	 */
	RIFFWaveIStream(const std::string& path);

	/**
	 * \brief Destructor
	 *
	 * Closes the input filestream.
	 */
	~RIFFWaveIStream();


	/**
	 * \brief Output stream operator
	 * \param sample A pointer to an array of length ChannelCount()
	 */
	RIFFWaveIStream& operator>>(int16_t* sample);

	/**
	 * \brief Test if EOF is reached
	 */
	bool eof() const;

	/**
	 * \brief buffer the whole data in RAM
	 *
	 * This stores the data as a `std::vector` of `int16_t`. So if the original file
	 * uses 8 bits per sample, the size doubles when buffered with this method!
	 */
	void bufferAll();		//Buffer the whole file

private:
	std::ifstream file;		///< The input filestream to read the data from

	std::vector<int16_t> file_buffer;	///< This vector contains the buffered input data
	bool file_buffered = false;			///< Saves wether the file is currently buffered
	uint32_t index = 0;					///< The number of the next frame to output
};


//---------- inline /templated functions implementation ----------//

inline uint32_t RIFFWaveStream::FileSize() const
{
	return fmt_header.FileSize + 8;
}


inline uint16_t RIFFWaveStream::FormatType() const
{
	return fmt_header.FormatType;
}


inline uint16_t RIFFWaveStream::ChannelCount() const
{
	return fmt_header.ChannelCount;
}


inline uint32_t RIFFWaveStream::FrameRate() const
{
	return fmt_header.FrameRate;
}


inline uint32_t RIFFWaveStream::ByteRate() const
{
	return fmt_header.ByteRate;
}


inline uint16_t RIFFWaveStream::BitsPerSample() const
{
	return fmt_header.BitsPerSample;
}


inline uint16_t RIFFWaveStream::MaxSampleValue() const
{
	return ((1 << fmt_header.BitsPerSample) - 1) / 2;
}


inline uint32_t RIFFWaveStream::FrameCount() const
{
	return data_header.DataSize / fmt_header.BytesPerFrame;
}



//---------- RIFFWaveOStream Implementation ----------//

inline RIFFWaveOStream& RIFFWaveOStream::operator<<(int16_t* sample)
{
	if (fmt_header.BitsPerSample == 8) {
		std::vector<uint8_t> data(fmt_header.ChannelCount, 0);
		for (uint16_t channel = 0; channel < fmt_header.ChannelCount; ++channel)
			data[channel] = (uint8_t)(sample[channel] & 0x00FF);
		file.write(reinterpret_cast<const char*>(data.data()), fmt_header.ChannelCount);
	}
	else {
		file.write(reinterpret_cast<const char*>(sample), fmt_header.ChannelCount * sizeof(*sample));
	}
	return *this;
}



//---------- RIFFWaveIStream Implementation ----------//

inline RIFFWaveIStream& RIFFWaveIStream::operator>>(int16_t* sample)
{
	BOOST_ASSERT_MSG(BitsPerSample() == 8 || BitsPerSample() == 16, "The file must have either 8 or 16 bits per sample!");
	if (file_buffered)
		memcpy(sample, &(file_buffer[index * ChannelCount()]), sizeof(*sample) * ChannelCount());
	else {
		//Read all bytes for this frame
		std::vector<uint8_t> bytes(fmt_header.BytesPerFrame);
		file.read((char*)bytes.data(), bytes.size());
		for (uint16_t channel = 0; channel < ChannelCount(); ++channel) {
			if (BitsPerSample() == 8)
				sample[channel] = signextend<uint16_t, 8>(bytes[channel]);
			else if (BitsPerSample() == 16)
				sample[channel] = *(int16_t*)&bytes[2 * channel];
		}
	}
	++index;
	return *this;
}


inline bool RIFFWaveIStream::eof() const
{
	return index >= FrameCount();
}


#endif //IO_RIFF_WAVE_H_