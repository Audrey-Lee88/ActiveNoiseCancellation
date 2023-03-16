function [b_out, b_matlab] = quantizefir(b, B)
% Quantize FIR filter coefficients to use in C/C++.
% 
% Input(s)
%   b: filter coefficients
%      H(z) = (b0 + b1 z^-1 + b2 z^-2 + ... + bN z^-N)
%   B: desired Q.B fixed-point format (B+1) bits
% 
% Output(s)
% B_OUT: quantized (fixed point) FIR filter coefficients
%   H(z) = 2^-B * (b0' + b1' z^-1 + b2' z^-2 + ... + bN' z^-N)
%
% The primed coefficients (') are quantized to (B + 1) bits and represented
% in integer format.
%
% B_MATLAB: same as B_OUT but represented in fractional format.
%
% The function also displays C formatted strings for each of the filters.
%

% History:
% 01/31/2014, F. Mujica, created

numTaps = length(b);

assert(min(b) >=  -1, 'Coefficients must be in the range [-1, 1)');
assert(max(b) <=  (2^B - 1)/2^B, 'Coefficients must be in the range [-1, 1)');

b_out = b;

numTaps = length(b_out);

b_out = round(2^B * b_out);
b_matlab  = b_out / 2^B;

% Print C float array
str = sprintf('float h[%d] =   {%d, ', numTaps, b(1));
for n = 2:(numTaps - 1)
    str = [str, sprintf('%d, ', b(n))];
end
str = [str, sprintf('%d};', b(numTaps))];
disp(str);

% Print C float array quantized
str = sprintf('float h_q[%d] = {%d, ', numTaps, b_matlab(1));
for n = 2:(numTaps - 1)
    str = [str, sprintf('%d, ', b_matlab(n))];
end
str = [str, sprintf('%d};', b_matlab(numTaps))];
disp(str);

% Print C int array
str = sprintf('int h_q[%d] = {%d, ', numTaps, b_out(1));
for n = 2:(numTaps - 1)
    str = [str, sprintf('%d, ', b_out(n))];
end
str = [str, sprintf('%d};', b_out(numTaps))];
disp(str(1:3000));
disp(str(3001:6000));
disp(str(6001:9000));
disp(str(9001:11041));
%b_int = str;
