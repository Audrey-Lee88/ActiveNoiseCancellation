function audiofilewrite(filename, y, fs)
%AUDIOFILEWRITE writes data Y to an audio file specified by the file name
% FILENAME with extension .wav (and .caf in macOS)  with a sample rate of
% FS Hz.
%
% See also AUDIOWRITE

audiowrite(char(filename + ".wav"), y, fs)

if ismac
    cmd = "afconvert -f caff -d LEI16@" + num2str(fs) + " -c 1 " + filename + ".wav " + filename + ".caf"
    system(char(cmd));
end