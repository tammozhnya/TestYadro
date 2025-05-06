#ifndef QAM_DEMODULATOR_H
#define QAM_DEMODULATOR_H
#include "QAMModulator.h"
#include <vector>   
#include <complex>  
#include <stdexcept>


class QamDemodulator {
public:

    QamDemodulator(ModulationType type);
    std::vector<int> demodulate(const std::complex<double>& symbol) const;
    std::vector<std::vector<int>> demodulateSequence(const std::vector<std::complex<double>>& symbols) const;

    const std::vector<std::complex<double>>& getConstellation() const;
    const std::vector<std::vector<int>>& getBitMapping() const;

private:
    ModulationType type;
    int NumDots;
    int bitsPerSymbol;
    std::vector<std::complex<double>> constellation;
    std::vector<std::vector<int>> symbolToBitsMap;

    void generateConstellation();
    void generateMapping();
};
#endif