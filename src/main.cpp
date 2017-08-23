#include <types.h>
#include <filters.h>
#include <iostream>
#include <boost/math/constants/constants.hpp>
#include <chrono>
#include <cstdint>
#include <test/test.h>
#include <string>
#include <filter/window_functions.h>



int main(int argc, char* argv[])
{
#ifdef _MSC_VER
	TestAll();
#endif
	
	if (argc >= 2 && (argv[1] == std::string("-t") || argv[1] == std::string("--test"))) {	//Run tests
		TestAll();
	}
	else {	//Run the main program
		constexpr uint32_t sample_rate = 44100;
		constexpr Real seconds = 300;

		constexpr uint64_t samplecount = seconds * sample_rate;
		constexpr uint64_t itercount = samplecount / RealSimd::static_size;

		//Initialize all filters
		FrequencyGenerator<Complex> generator(sample_rate, 100);
		FrequencyShift<Complex> filter(sample_rate, -100);
		RiffWaveOutput<Complex> modified_out(sample_rate, "modified.wav", 0.9);

		//Connect the filters into a network
		filter.SetInput(0, { static_cast<const Filter*>(&generator), 0 });
		modified_out.SetInput(0, { static_cast<const Filter*>(&filter), 0 });

		auto start = std::chrono::steady_clock::now();
		for (uint64_t i = 0; i < itercount; ++i) {
			generator.Update();
			filter.Update();
			modified_out.Update();
		}
		auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - start).count();
		std::cout << duration << "ns for " << samplecount << "Samples => " << ((double)duration / samplecount) << "ns per Sample" << std::endl;	

#ifdef _MSC_VER
		char c;
		std::cin >> c;
#endif
	}

	return 0;
}