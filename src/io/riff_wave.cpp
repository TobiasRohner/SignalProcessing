#include <io/riff_wave.h>
#include <boost/endian/arithmetic.hpp>


//---------- RIFFWaveStream::FMTHeader Implementation ----------//

void RIFFWaveStream::FMTHeader::load(std::ifstream& file)
{
	//Read all values
	file.read(reinterpret_cast<char*>(this), sizeof(*this));

	//Correct Endianness if needed
	if (_RIFF == 0x52494646)	//Endianness is wrong
		toggleEndianness();

	//Throw away the padding bytes between the header and data chunk
	file.ignore(FormatDataLength - 0x10);
}


void RIFFWaveStream::FMTHeader::save(std::ofstream& file)
{
	//Correct for the endianness of the system
	FMTHeader to_write = *this;
	if (boost::endian::order::native != boost::endian::order::little)
		to_write.toggleEndianness();

	//Write the Header to file
	file.write(reinterpret_cast<char*>(&to_write), sizeof(to_write));
}


void RIFFWaveStream::FMTHeader::toggleEndianness()
{
	boost::endian::endian_reverse_inplace(_RIFF);
	boost::endian::endian_reverse_inplace(FileSize);
	boost::endian::endian_reverse_inplace(_WAVE);
	boost::endian::endian_reverse_inplace(FormatDataLength);
	boost::endian::endian_reverse_inplace(FormatType);
	boost::endian::endian_reverse_inplace(ChannelCount);
	boost::endian::endian_reverse_inplace(FrameRate);
	boost::endian::endian_reverse_inplace(ByteRate);
	boost::endian::endian_reverse_inplace(BytesPerFrame);
	boost::endian::endian_reverse_inplace(BitsPerSample);
}



//---------- RIFFWaveStream::DataHeader Implementation ----------//

void RIFFWaveStream::DataHeader::load(std::ifstream& file)
{
	//Read all values
	file.read(reinterpret_cast<char*>(this), sizeof(*this));

	//Correct Endianness if needed
	if (_data == 0x64617461)
		toggleEndianness();
}


void RIFFWaveStream::DataHeader::save(std::ofstream& file)
{
	//Correct for the endianness of the system
	DataHeader to_write = *this;
	if (boost::endian::order::native != boost::endian::order::little)
		to_write.toggleEndianness();

	//Write the Header to file
	file.write(reinterpret_cast<char*>(&to_write), sizeof(to_write));
}


void RIFFWaveStream::DataHeader::toggleEndianness()
{
	boost::endian::endian_reverse_inplace(_data);
	boost::endian::endian_reverse_inplace(DataSize);
}



//---------- RIFFWaveStream Implementation ----------//

RIFFWaveStream::RIFFWaveStream()
{
}



//---------- RIFFWaveOStream Implementation ----------//

RIFFWaveOStream::RIFFWaveOStream(const std::string& path, uint16_t channel_count) : file(path, std::ios::out | std::ios::binary)
{
	fmt_header.ChannelCount = channel_count;
	//These headers are just placeholders for the ones of the configured file
	fmt_header.save(file);
	data_header.save(file);
}


RIFFWaveOStream::RIFFWaveOStream(const std::string& path, uint32_t sample_rate, uint16_t channel_count) : file(path, std::ios::out | std::ios::binary)
{
	fmt_header.FrameRate = sample_rate;
	fmt_header.ChannelCount = channel_count;
	//These headers are just placeholders for the ones of the configured file
	fmt_header.save(file);
	data_header.save(file);
}


RIFFWaveOStream::~RIFFWaveOStream()
{
	//Make sure everything is written to the file
	writeHeaders();
	file.flush();
	file.close();
}


void RIFFWaveOStream::close()
{
	writeHeaders();
	file.flush();
	file.close();
}


void RIFFWaveOStream::FrameRate(uint32_t rate)
{
	fmt_header.FrameRate = rate;
	fmt_header.ByteRate = fmt_header.FrameRate * fmt_header.BytesPerFrame;
}


void RIFFWaveOStream::set8BitsPerSample()
{
	fmt_header.BitsPerSample = 8;
	fmt_header.BytesPerFrame = fmt_header.ChannelCount;
	fmt_header.ByteRate = fmt_header.BytesPerFrame * fmt_header.FrameRate;
}


void RIFFWaveOStream::set16BitsPerSample()
{
	fmt_header.BitsPerSample = 16;
	fmt_header.BytesPerFrame = 2 * fmt_header.ChannelCount;
	fmt_header.ByteRate = fmt_header.BytesPerFrame * fmt_header.FrameRate;
}


void RIFFWaveOStream::writeHeaders()
{
	fmt_header.FileSize = (uint32_t)file.tellp() - 8;
	data_header.DataSize = (uint32_t)file.tellp() - sizeof(fmt_header) - sizeof(data_header);
	file.seekp(0);
	fmt_header.save(file);
	data_header.save(file);
}



//---------- RIFFWaveIStream Implementation ----------//

RIFFWaveIStream::RIFFWaveIStream(const std::string& path) : file(path, std::ios::in | std::ios::binary)
{
	fmt_header.load(file);
	data_header.load(file);
}


RIFFWaveIStream::~RIFFWaveIStream()
{
	file.close();
}


void RIFFWaveIStream::bufferAll()
{
	if (!file_buffered) {
		file_buffer.resize(ChannelCount() * FrameCount());
		for (uint32_t sample = 0; sample < file_buffer.size(); sample += ChannelCount())
			this->operator >> (file_buffer.data() + sample);
	}
	file_buffered = true;
	index = 0;
}