function Est = Block_NoiseCancel(A, wi, M, curr_block)
%UNTITLED Summary of this function goes here
%   Detailed explanation goes here
block_siz = 256; %always 256 (probably)
if (curr_block-1) == 0
    start_idx = M;
else
    start_idx = (curr_block-1)*block_siz+1;
end
if block_siz*curr_block > length(A)
    end_idx = length(A);
else
    end_idx = block_siz*curr_block;
end
% find the estimate
Est = zeros(block_siz, 1);
for i = start_idx:end_idx
    j = A(i:-1:i-M+1);
    Est(i-start_idx+1) = (wi)'*(j)';
end
