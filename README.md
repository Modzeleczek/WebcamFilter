# WebcamFilter
## Description
The program fetches a frame from the source then processes it by itself and then processes it using GPU controlled with OpenGL. Finally, the frame is uploaded to the target or displayed in an OpenGL window.

WebcamFilter's purpose is to process images in real time using filters which are implemented as pairs of vertex and fragment shaders. Calculations of such filters are often independent of each other so they can be performed in parallel. Therefore we use a GPU to hardware accelerate the calculations.

#### Supported sources
- software generated (GenericSource)
- V4L2 webcam (https://www.cnblogs.com/kevin-heyongyuan/articles/11070935.html) (Webcam)

#### Supported targets
- OpenGL window (Display)
- virtual video device (V4L2 loopback device) (VideoLoopback)

#### Implemented filters
- binarization
- box blur
- edge detection
- horizontal flip
- Gaussian blur with 3x3 convolution kernel
- Gaussian blur with 5x5 convolution kernel
- identity (no effect)
- negative
- sharpening
- solid color (input image is ignored and the output is a particular solid color)

## Running
WebcamFilter has been made for Linux and tested on Ubuntu 20.04 LTS. In order to run the program you need a GPU supporting OpenGL 3.3 or newer.
```
WebcamFilter [options] frame-width frame-height vertex-shader-path fragment-shader-path
Arguments:
 frame-width             Width of frames used by source and target. Frame width must be equal for source and target.
 frame-height            Height of frames used by source and target. Frame height must be equal for source and target.
 vertex-shader-path      Path to a text file containing code of the vertex shader executed by GPU.
 fragment-shader-path    Path to a text file containing code of the fragment shader executed by GPU.

Options:
 -s <path>    Download image frames from the webcam device in <path>.
 -t <path>    Upload processed image frames to the V4L2 loopback device in <path>.
 -c           Download next frame from source while (default: after) processing the current one.
 ```

When -s is not used, WebcamFilter uses software generated source, i.e. it generates pseudo-random frames by itself.  
When -t is not used, WebcamFilter displays output frames in an OpenGL window.

Before using your webcam as a source, make sure that it supports YUYV 4:2:2 format. You can do it using v4l2-ctl tool from v4l-utils apt package.
```
v4l2-ctl --list-formats-ext
```

You need to install v4l2loopback (https://github.com/umlaeute/v4l2loopback) kernel module which allows you to create a virtual video device if you want to use one as a target.

Shaders for use with OpenGL window target are located in display_shaders directory.  
Shaders for use with virtual video device are located in camera_shaders directory.  
In both directories there is one vertex shader rectangle.vert and several fragment shaders with .frag extension which implement the effects listed above.

#### Usage example
```
build/apps/WebcamFilter -c -s /dev/video0 640 480 display_shaders/rectangle.vert display_shaders/edges.frag
```
TODO

## Building
WebcamFilter creates an OpenGL window using GLFW and then calls GLEW functions to control the GPU. The program has been checked to be built correctly using g++ 9.3.0 and with the following apt packages installed:
- libglfw3 3.3.2-1
- libglfw3-dev 3.3.2-1
- libglew2.1 2.1.0-4
- libglew-dev 2.1.0-4

If you already have g++ and the required packages, you can build WebcamFilter using make.
```
make release
```
The executable will be placed in ```build/apps``` directory. Other possible make rules are ```debug``` ```clean``` ```info```
