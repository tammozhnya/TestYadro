#include "GAUSS.h"
#include <stdexcept> 
#include <cmath>     


AwgnGenerator::AwgnGenerator(double noise_variance): randEngine(std::random_device()()) {

    setNoiseVar(noise_variance);
}

void AwgnGenerator::setNoiseVar(double noise_variance) {

    curNoiseVar = noise_variance;
    if (curNoiseVar == 0.0) {
        double componentDev = 0.0;
    }
    else {
        componentDev = std::sqrt(curNoiseVar / 2.0);
    }
    distribution = std::normal_distribution<double>(0.0, componentDev);
}

double AwgnGenerator::getNoiseVar() const {
    return curNoiseVar;
}

void AwgnGenerator::applyToSignal(std::vector<std::complex<double>>& signal) {
    if (curNoiseVar == 0.0) {
        return;
    }

    for (auto& sample : signal) {
        double noise_real = distribution(randEngine);
        double noise_imag = distribution(randEngine);
        sample += std::complex<double>(noise_real, noise_imag);
    }
}

std::complex<double> AwgnGenerator::genNoiseSample() {
    double noise_real = distribution(randEngine);
    double noise_imag = distribution(randEngine);
    return { noise_real, noise_imag };
}