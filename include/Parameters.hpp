#ifndef Parameters_HPP
#define Parameters_HPP

struct Parameters
{
    enum SourceType { NoSource, Generic, Webcam };
    enum TargetType { NoTarget, Display, V4L2Loopback, Terminal };

    SourceType SourceType;
    TargetType TargetType;
    char *SourcePath, *TargetPath;
    bool Concurrent;
    int InputWidth; // 160  * 1, 2, 4, 5, 8
    int InputHeight; // 120 * 1, 2, 4, 5, 6
    int OutputWidth;
    int OutputHeight;
    char *VSPath, *FSPath;

    Parameters(int argc, char **argv);
};

#endif // Parameters_HPP
