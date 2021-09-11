#ifndef Parameters_HPP
#define Parameters_HPP

struct Parameters
{
    char *SourcePath, *TargetPath;
    bool Concurrent;
    int Width; // 160  * 1, 2, 4, 5, 8
    int Height; // 120 * 1, 2, 4, 5, 6
    char *VSPath, *FSPath;

    Parameters(int argc, char **argv);
};

#endif // Parameters_HPP
