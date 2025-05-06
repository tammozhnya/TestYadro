clc; clear;
EbN0 = 0:10;
N = 120000;
testType = 64;
for i=1:length(EbN0)
    bits = randi([0,1], N, 1);
    symbols = qammod(bits, testType, "InputType","bit");
    noise_symbols = awgn(symbols, EbN0(i));
    res_bits = qamdemod(noise_symbols, testType, "OutputType","bit");
    ber(i) = biterr(bits, res_bits)/N;
end


fileName = 'Output.txt'; 
dataTable = readtable(fileName);
x_data = dataTable{:, 1}; 
columnNames = dataTable.Properties.VariableNames; 
for i = 2:width(dataTable)
    y_data_candidate = dataTable{:, i};
end


semilogy(EbN0, ber, x_data, y_data_candidate);
grid on
legend('Matlab','C++')
title('QAM64');
xlabel('Eb/N0'); 
ylabel('BER');


