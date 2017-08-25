# SignalProcessing
This toolbox contains various signal processing filters and
provides an easy to use interface for further expansion.


# Contents
The library currently contains 4 IO and 7 signal processing filters:
+ IO
   - VectorInput
   - VectorOutput
   - RiffWaveInput
   - RiffWaveOutput
+ Signal Processing
   - FrequencyGenerator
   - FrequencyShift
   - AmplitudeModulator
   - FrequencyModulator
   - Fir
   - Differentiator
   - LowPass


## VectorInput
The VectorInput class provides input to the flow graph from
data stored in a standard C++ Vector. This data can either be of type `Real`
or of type `Complex`. The data is copied or moved inside the
VectorInput class itself.

VectorInput provides two constructors.
The following code is an example demonstrating both of them:
```c++
// The VectorInput should output the data at 44100Hz
constexpr double sample_rate = 44100;

// Set up the data that the VectorInput should output into the flow graph
std::vector<Real> real_data({1, 2, 3, 4, 5, 6, 7, 8});

// First constructor
VectorInput<Real> real_input_var1(sample_rate);
real_input_var1.SetInput(real_data);
// Second constructor
VectorInput<Real> real_input_var2(sample_rate, real_data);
```
For `Complex` valued inputs to the flow graph, the code is exactly the same
except the template argument to VectorInput should be changed to `Complex`
instead of `Real`.