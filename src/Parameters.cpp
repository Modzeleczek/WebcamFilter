#include "../include/Parameters.hpp"

#include <stdexcept>
#include <string.h>
#include <unistd.h>

#define HELP "WebcamFilter [options] source-frame-width source-frame-height target-frame-width target-frame-height vertex-shader-path fragment-shader-path\n\
Arguments:\n\
 source-frame-width      Width of frames used by source.\n\
 source-frame-height     Height of frames used by source.\n\
 target-frame-width      Width of frames used by target.\n\
 target-frame-height     Height of frames used by target.\n\
 vertex-shader-path      Path to a text file containing code of the vertex shader executed by GPU.\n\
 fragment-shader-path    Path to a text file containing code of the fragment shader executed by GPU.\n\
\n\
Options:\n\
 -s generic|webcam                   Select specified source type (default: generic).\n\
 -t display|v4l2loopback|terminal    Select specified target type (default: display).\n\
 -w <path>    Download image frames from the webcam device in <path>. Must be specified with generic source type.\n\
 -l <path>    Upload processed image frames to the V4L2 loopback device in <path>. Must be specified with v4l2loopback target type.\n\
 -c           Download next frame from source while (default: after) processing the current one.\n\
\n\
To close a WebcamFilter process send it a SIGINT signal."

Parameters::Parameters(int argc, char **argv) :
    SourceType(SourceType::NoSource),
    TargetType(TargetType::NoTarget),
    SourcePath(NULL),
    TargetPath(NULL),
    Concurrent(false),
    InputWidth(0),
    InputHeight(0),
    OutputWidth(0),
    OutputHeight(0),
    VSPath(NULL),
    FSPath(NULL)
{
    int option;
    char *sourceTypeString = NULL, *targetTypeString = NULL;
    while ((option = getopt(argc, argv, ":hs:t:w:l:c")) != -1)
    {
        switch (option)
        {
        case 'h':
            printf(HELP"\n");
            exit(0);
            break;
        case 's':
            if (strlen(optarg) < 1)
                throw std::invalid_argument("-s requires source type");
            sourceTypeString = optarg;
            break;
        case 't':
            if (strlen(optarg) < 1)
                throw std::invalid_argument("-t requires target type");
            targetTypeString = optarg;
            break;
        case 'w':
            if (strlen(optarg) < 1)
                throw std::invalid_argument("-w requires webcam device path");
            SourcePath = optarg;
            break;
        case 'l':
            if (strlen(optarg) < 1)
                throw std::invalid_argument("-l requires V4L2 loopback device path");
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
    if (sourceTypeString == NULL)
        throw std::invalid_argument("Specify source type");
    if (targetTypeString == NULL)
        throw std::invalid_argument("Specify target type");

    if (strcmp(sourceTypeString, "webcam") == 0)
    {
        if (SourcePath == NULL)
            throw std::invalid_argument("Specify source path");
        else
            this->SourceType = SourceType::Webcam;
    }
    else if (strcmp(sourceTypeString, "generic") == 0)
        this->SourceType = SourceType::Generic;
    else
        throw std::invalid_argument("Specify supported source type");

    if (strcmp(targetTypeString, "v4l2loopback") == 0)
    {
        if (TargetPath == NULL)
            throw std::invalid_argument("Specify target path");
        else
            this->TargetType = TargetType::V4L2Loopback;
    }
    else if (strcmp(targetTypeString, "terminal") == 0)
        this->TargetType = TargetType::Terminal;
    else if (strcmp(targetTypeString, "display") == 0)
        this->TargetType = TargetType::Display;
    else
        throw std::invalid_argument("Specify supported target type");

    int remainingArguments = argc - optind; // wyznaczamy liczbę obowiązkowych argumentów, które nie są opcjami; powinny być dokładnie 6
    if (remainingArguments != 6)
        throw std::invalid_argument(HELP);
    if (sscanf(argv[optind], "%i", &InputWidth) < 1 || InputWidth <= 0)
        throw std::invalid_argument("Specify a positive source frame width");
    if (sscanf(argv[optind + 1], "%i", &InputHeight) < 1 || InputHeight <= 0)
        throw std::invalid_argument("Specify a positive source frame height");
    if (sscanf(argv[optind + 2], "%i", &OutputWidth) < 1 || OutputWidth <= 0)
        throw std::invalid_argument("Specify a positive target frame width");
    if (sscanf(argv[optind + 3], "%i", &OutputHeight) < 1 || OutputHeight <= 0)
        throw std::invalid_argument("Specify a positive target frame height");
    VSPath = argv[optind + 4];
    FSPath = argv[optind + 5];
}