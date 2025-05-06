#include "QAMModulator.h" 
#include "QAMDemodulator.h" 
#include "GAUSS.h" 
#include <vector>
#include <complex>
#include <cmath>
#include <stdexcept>   
#include <string>
#include <iostream>   
#include <iomanip> 
#include <fstream>
#define NOMINMAX
#include <windows.h>


void printBits(const std::vector<int>& bits) {
    for (int bit : bits) {
        std::cout << bit;
    }
}

std::vector<int> generateRandomBits(int total_bits, unsigned int seed = std::random_device{}()) {

    std::vector<int> bits(total_bits);
    std::mt19937 gen(seed);
    std::uniform_int_distribution<> distrib(0, 1);

    for (int i = 0; i < total_bits; ++i) {
        bits[i] = distrib(gen);
    }
    return bits;
}


int main() {

    SetConsoleOutputCP(1251);
    int total_bits = 120000;

    ModulationType testType = ModulationType::QAM64;
    std::vector<int> inputBits = generateRandomBits(total_bits);

    std::string filename = "Output.txt";
    std::ofstream results_file(filename);
    results_file << "EbN0,BER\n";

    // Модуляция
    QamModulator modulator;

    // Демодуляция
    QamDemodulator demodulator(testType);

    std::vector<int> InputBits = generateRandomBits(total_bits);

    for (double i = 0; i < 11; ++i) {
        double SNR = i;
        double N0 = 1 / std::pow(10.0, SNR / 10.0);

        // Модуляция
        std::vector<std::complex<double>> modulatedSymbols = modulator.modulate(inputBits, testType);
        std::vector<std::complex<double>> noisy_signal = modulatedSymbols;

        //ШУМ
        AwgnGenerator noise_generator(N0);
        noise_generator.applyToSignal(noisy_signal);

        long long cur_bit_errors = 0;

        try {
            //Демодуляция
            std::vector<std::vector<int>> outputBitsSequence = demodulator.demodulateSequence(noisy_signal);
            std::vector<int> finalOutputBits;
            finalOutputBits.reserve(inputBits.size());

            for (const auto& bitsVec : outputBitsSequence) {
                finalOutputBits.insert(finalOutputBits.end(), bitsVec.begin(), bitsVec.end());
            }

            // Сравнение
            double BER = -1.0;
            if (inputBits.size() == finalOutputBits.size()) {
                for (size_t i = 0; i < total_bits; ++i) {
                    if (inputBits[i] != finalOutputBits[i]) {
                        cur_bit_errors++;
                    }
                }

                if (InputBits.empty()) {
                    BER = 0.0;
                }
                else {
                    BER = static_cast<double>(cur_bit_errors) / InputBits.size();
                }
            }
            //запись в файл
            results_file << std::fixed << std::setprecision(3) << SNR << ","
                << std::setprecision(7) << BER << "\n";
        }
        catch (const std::exception& e) {
            std::cerr << "\n!!! Ошибка выполнения" << e.what() << " !!!" << std::endl;
        }    
    }
    results_file.close();
    return 0;
}