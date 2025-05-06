#include "QAMDemodulator.h" 
#include <cmath>      
#include <limits>     
#include <stdexcept>  
#include <iostream>   


QamDemodulator::QamDemodulator(ModulationType type) : type (type) {
    switch (type) {
    case ModulationType::QAM4:
        NumDots = 4;
        bitsPerSymbol = 2;
        break;
    case ModulationType::QAM16:
        NumDots = 16;
        bitsPerSymbol = 4;
        break;
    case ModulationType::QAM64:
        NumDots = 64;
        bitsPerSymbol = 6;
        break;
    default:
        throw std::invalid_argument("Такая модуляция не поддерживается");
    }

    generateConstellation();
    generateMapping(); 
}

std::vector<int> QamDemodulator::demodulate(const std::complex<double>& symbol) const {
    double min_dist_sq = std::numeric_limits<double>::max();
    int bestIndex = -1;

    for (int i = 0; i < NumDots; ++i) {
        double dist_sq = std::norm(symbol - constellation[i]); 
        if (dist_sq < min_dist_sq) {
            min_dist_sq = dist_sq;
            bestIndex = i;
        }
    }

    if (bestIndex == -1) {
        throw std::runtime_error("Не нашел точку созвездия");
    }
    return symbolToBitsMap[bestIndex];
}


std::vector<std::vector<int>> QamDemodulator::demodulateSequence(const std::vector<std::complex<double>>& symbols) const {
    std::vector<std::vector<int>> outputBits;
    outputBits.reserve(symbols.size());

    for (const auto& currentSymbol : symbols) {
        outputBits.emplace_back(demodulate(currentSymbol));
    }
    return outputBits;
}

const std::vector<std::complex<double>>& QamDemodulator::getConstellation() const {
    return constellation;
}
const std::vector<std::vector<int>>& QamDemodulator::getBitMapping() const {
    return symbolToBitsMap;
}


void QamDemodulator::generateConstellation() {
    constellation.resize(NumDots);
    int k = static_cast<int>(std::sqrt(NumDots));
    int index = 0;
    for (int i = 0; i < k; ++i) {
        double Q_level = (2.0 * i - k + 1.0);
        for (int j = 0; j < k; ++j) {
            double I_level = (2.0 * j - k + 1.0);
            constellation[index++] = std::complex<double>(I_level, Q_level);
        }
    }
}


void QamDemodulator::generateMapping() {
    symbolToBitsMap.resize(NumDots);
    int k = static_cast<int>(std::round(std::sqrt(NumDots)));
    auto intToGray = [](int n) -> int {
        return n ^ (n >> 1);
    };

    auto valueToBits = [&](int val, int numBits) -> std::vector<int> {
        std::vector<int> bits(numBits);
        for (int b = 0; b < numBits; ++b) {
            bits[numBits - 1 - b] = (val >> b) & 1;
        }
        return bits;
    };

    int index = 0;
    int bits_per_dim = bitsPerSymbol / 2;
    for (int i = 0; i < k; ++i) {
        int gray_i = intToGray(i);
        std::vector<int> bits_Q = valueToBits(gray_i, bits_per_dim);

        for (int j = 0; j < k; ++j) {
            int gray_j = intToGray(j);
            std::vector<int> bits_I = valueToBits(gray_j, bits_per_dim);
            std::vector<int> currentSymbolBits(bitsPerSymbol);
            std::copy(bits_I.begin(), bits_I.end(), currentSymbolBits.begin());
            std::copy(bits_Q.begin(), bits_Q.end(), currentSymbolBits.begin() + bits_per_dim);

            symbolToBitsMap[index++] = currentSymbolBits;
        }
    }
}