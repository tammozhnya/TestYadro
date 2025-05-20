clc; clear; close all;

EbN0_dB = 0:10; 
N_bits = 120000;      
testType = 64;                        
Eb = 1;               

ber_matlab = zeros(1, length(EbN0_dB));
dispers = zeros(1, length(EbN0_dB));

for i = 1:length(EbN0_dB)
    current_EbN0_dB = EbN0_dB(i);    
    EbN0_linear = 10^(current_EbN0_dB / 10);
    N0 = 1 / EbN0_linear;
    dispers(i) = N0/2;
    bits = randi([0,1], N_bits, 1);
    
    symbols= qammod(bits, testType, "InputType", "bit");
    noise_symbols = awgn(symbols, EbN0_dB(i));
    res_bits = qamdemod(noise_symbols, testType, "OutputType", "bit");

    [num_errors, ber_matlab(i)] = biterr(bits, res_bits);
end


fileName = 'Output.txt'; 
dataTable = readtable(fileName);
x_data = dataTable{:, 1}; 
y_data = dataTable{:, 2};    
EbN0_linear_cpp = 10.^(x_data / 10);
N0_cpp = 1 ./ EbN0_linear_cpp; 
dispers_cpp = N0_cpp/2;

semilogy(dispers, ber_matlab, dispers_cpp, y_data);
grid on
legend('Matlab','C++')
title('QAM64');
xlabel('σ²'); 
ylabel('BER');
