#include "../include/Parameters.hpp"

#include <stdexcept>
#include <string.h>
#include <unistd.h>

#define HELP "WebcamFilter [options] frame-width frame-height vertex-shader-path fragment-shader-path\n\
Arguments:\n\
 frame-width             Width of frames used by source and target. Frame width must be equal for source and target.\n\
 frame-height            Height of frames used by source and target. Frame height must be equal for source and target.\n\
 vertex-shader-path      Path to a text file containing code of the vertex shader executed by GPU.\n\
 fragment-shader-path    Path to a text file containing code of the fragment shader executed by GPU.\n\
\n\
Options:\n\
 -s <path>    Download image frames from the webcam device in <path>.\n\
 -t <path>    Upload processed image frames to the V4L2 loopback device in <path>.\n\
 -c           Download next frame from source while (default: after) processing the current one.\n\
\n\
To close a WebcamFilter process send it a SIGINT signal."

Parameters::Parameters(int argc, char **argv) :
    SourcePath(NULL),
    TargetPath(NULL),
    Concurrent(false),
    Width(0),
    Height(0),
    VSPath(NULL),
    FSPath(NULL)
{
    int option;
    while ((option = getopt(argc, argv, ":s:t:c")) != -1)
    {
        switch (option)
        {
        case 's':
            if (strlen(optarg) < 1)
                throw std::invalid_argument("-s requires source device path");
            SourcePath = optarg;
            break;
        case 't':
            if (strlen(optarg) < 1)
                throw std::invalid_argument("-t requires target device path");
            TargetPath = optarg;
            break;
        case 'c':
            Concurrent = true;
            break;
        case ':':
            throw std::invalid_argument("Option requires a value");
            break;
        case '?':
            throw std::invalid_argument("Unknown option");
            break;
        default:
            throw std::invalid_argument("Error");
            break;
        }
    }
    int remainingArguments = argc - optind; // wyznaczamy liczbę obowiązkowych argumentów, które nie są opcjami; powinny być dokładnie 4
    if (remainingArguments != 4)
        throw std::invalid_argument(HELP);
    if (sscanf(argv[optind], "%i", &Width) < 1 || Width <= 0)
        throw std::invalid_argument("Specify a positive frame width");
    if (sscanf(argv[optind + 1], "%i", &Height) < 1 || Height <= 0)
        throw std::invalid_argument("Specify a positive frame height");
    VSPath = argv[optind + 2];
    FSPath = argv[optind + 3];
}