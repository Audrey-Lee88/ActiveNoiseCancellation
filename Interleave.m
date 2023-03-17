function C = Interleave(A,B)
% """ Take Vector A and B, concatenate into Vector C" + ...
%     
%     """"
%     C = [A;B];
%     C = C(:)';

% for i=1:length(A)
%     C(2*i) = A(i);
%     C(2*i+1) = B(i);
% end

counter = 1;
for i = 1:2:2*length(A)-1
    C(i) = A(counter);
    counter = counter + 1;
end
counter = 1;
for i = 2:2:2*length(A)
     C(i) = B(counter);
     counter = counter + 1;
end
