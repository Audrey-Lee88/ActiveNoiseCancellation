function C = Interleave(A,B)
    C = [A;B];
    C = C(:)';

