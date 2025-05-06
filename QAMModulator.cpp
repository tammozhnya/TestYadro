#include "QAMModulator.h"
#include <vector>
#include <complex>
#include <cmath>
#include <string>

    //QAM-16
    double QamModulator::val2BitsQAM16(int b1, int b2) {
        if (!b1 && !b2) return -3.0; // 00
        if (!b1 && b2) return -1.0; // 01
        if (b1 && b2) return  1.0; // 11
        if (b1 && !b2) return  3.0; // 10
        return 0.0;
    }

    //QAM-64
    double QamModulator::val3BitsQAM64(int b1, int b2, int b3) {
        if (!b1 && !b2 && !b3) return -7.0; // 000
        if (!b1 && !b2 && b3) return -5.0; // 001
        if (!b1 && b2 && b3) return -3.0; // 011
        if (!b1 && b2 && !b3) return -1.0; // 010
        if (b1 && b2 && !b3) return  1.0; // 110
        if (b1 && b2 && b3) return  3.0; // 111
        if (b1 && !b2 && b3) return  5.0; // 101
        if (b1 && !b2 && !b3) return  7.0; // 100
        return 0.0;
    }

    std::vector<std::complex<double>> QamModulator::modulate(const std::vector<int>& bits, ModulationType type) {
        int bitsPerSymbol = 0;

        switch (type) {
        case ModulationType::QAM4:
            bitsPerSymbol = 2;
            break;

        case ModulationType::QAM16:
            bitsPerSymbol = 4;
            break;

        case ModulationType::QAM64:
            bitsPerSymbol = 6;
            break;
        default:
            throw std::invalid_argument("Такая модуляции не поддерживается");
        }


        std::vector<std::complex<double>> symbols;
        symbols.reserve(bits.size() / bitsPerSymbol);

        for (size_t i = 0; (i + bitsPerSymbol) <= bits.size(); i += bitsPerSymbol) {
            double I = 0.0;
            double Q = 0.0;

            switch (type) {
            case ModulationType::QAM4: {
                int b1 = bits[i];     
                int b2 = bits[i + 1]; 
                if (!b1 && !b2) { I = -1.0; Q = -1.0; } 
                else if (!b1 && b2) { I = -1.0; Q = 1.0; } 
                else if (b1 && b2) { I = 1.0; Q = 1.0; } 
                else if (b1 && !b2) { I = 1.0; Q = -1.0; } 
                break;
            }
            case ModulationType::QAM16: {
                I = val2BitsQAM16(bits[i], bits[i + 1]);
                Q = val2BitsQAM16(bits[i + 2], bits[i + 3]);
                break;
            }
            case ModulationType::QAM64: {
                I = val3BitsQAM64(bits[i], bits[i + 1], bits[i + 2]);
                Q = val3BitsQAM64(bits[i + 3], bits[i + 4], bits[i + 5]);
                break;
            }
            }
            symbols.emplace_back(I, Q);
        }

        return symbols;
    }
