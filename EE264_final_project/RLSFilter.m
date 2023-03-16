function [y, Bout] = RLSFilter(f, d, FIR_M, delta_n )
persistent F B lambda delta  M Rinv
% The following is initialization, and is executed once
if (ischar(f) && strcmp(f,'initial'))
    lambda = d;
    M = FIR_M;
    delta = delta_n;
    F = zeros(M,1);
    Rinv = delta*eye(M);
    B = zeros(M,1);
    y = 0;
else
% Filtering:
    for J = M:-1:2
       F(J) = F(J-1);
    end
F(1) = f;
% Perform the convolution
    y= F'*B;
    error = d - y;
% Kalman gains
    K = Rinv*F/(lambda + F'*Rinv*F);
% Update Rinv
    Rinvn = (Rinv - K*F'*Rinv)/lambda;
% Update the filter coefficients
   B = B + K*error;
   Bout=B;
   Rinv = Rinvn;
end