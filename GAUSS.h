#ifndef AWGN_GENERATOR_H
#define AWGN_GENERATOR_H

#include <vector>
#include <complex>
#include <random>

class AwgnGenerator {
public:
    AwgnGenerator();
    explicit AwgnGenerator(double noise_variance);
    void setNoiseVar(double noise_variance);
    double getNoiseVar() const;

    void applyToSignal(std::vector<std::complex<double>>& signal);
    std::complex<double> genNoiseSample();

private:
    std::mt19937 randEngine;                 
    std::normal_distribution<double> distribution; 

    double curNoiseVar;             
    double componentDev;                  
                                               
};

#endif