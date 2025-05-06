
#ifndef QAM_MODULATOR_H
#define QAM_MODULATOR_H

#include <vector>       
#include <complex>      
#include <stdexcept>    


enum class ModulationType { QAM4, QAM16, QAM64};

class QamModulator {
public:
    std::vector<std::complex<double>> modulate(const std::vector<int>& bits, ModulationType type);

private:
     static double val2BitsQAM16(int b1, int b2);
     static double val3BitsQAM64(int b1, int b2, int b3);
};

#endif