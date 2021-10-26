# WebcamFilter
## Description
The program fetches a frame from the source then processes it by itself and then processes it using GPU controlled with OpenGL. Finally, the frame is uploaded to the target or displayed in an OpenGL window.

WebcamFilter's purpose is to process images in real time using filters which are implemented as pairs of vertex and fragment shaders. Calculations of such filters are often independent of each other so they can be performed in parallel. Therefore we use a GPU to hardware accelerate the calculations.

### Supported sources
- software generated (GenericSource)
- V4L2 webcam (https://www.cnblogs.com/kevin-heyongyuan/articles/11070935.html) (Webcam)

### Supported targets
- OpenGL window (Display)
- virtual video device (V4L2 loopback device) (VideoLoopback)
- ncurses terminal (Terminal)

### Implemented filters
- for targets: OpenGL window, V4L2 loopback device
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
- for target: ncurses terminal
    - identity (no effect)
    - smooth downscaling (value of one output pixel is average value of (source width / target width)x(source height / target height) input pixels)

## Running
WebcamFilter has been tested on Ubuntu 20.04 LTS and Debian 10 with Intel HD 4600 and NVIDIA GeForce 820M. In order to run the program you need a GPU supporting OpenGL 3.3 or newer and the following shared libraries:
- ```libglfw3``` (tested on 3.3.2-1: libglfw.so.3.3)
- ```libglew2.1``` (tested on 2.1.0-4: libGLEW.so.2.1.0)
- ```libncurses6``` (tested on6.2-0ubuntu2: libncurses.so.6.2)

which you can get by executing
```
sudo apt install libglfw3 libglew2.1 libncurses6
```

Below is the program's usage description.
```
WebcamFilter [options] source-frame-width source-frame-height target-frame-width target-frame-height vertex-shader-path fragment-shader-path
Arguments:
 source-frame-width      Width of frames used by source.
 source-frame-height     Height of frames used by source.
 target-frame-width      Width of frames used by target.
 target-frame-height     Height of frames used by target.
 vertex-shader-path      Path to a text file containing code of the vertex shader executed by GPU.
 fragment-shader-path    Path to a text file containing code of the fragment shader executed by GPU.

Options:
 -s generic|webcam                   Select specified source type (default: generic).
 -t display|v4l2loopback|terminal    Select specified target type (default: display).
 -w <path>    Download image frames from the webcam device in <path>. Must be specified with generic source type.
 -l <path>    Upload processed image frames to the V4L2 loopback device in <path>. Must be specified with v4l2loopback target type.
 -c           Download next frame from source while (default: after) processing the current one.

To close a WebcamFilter process send it a SIGINT signal.
```

When target resolution is smaller or greater than source resolution, input frame is respectively downscaled by skipping pixels or upscaled by duplicating pixels. This is true for every filter except smooth downscaling for ncurses terminal target.  
When -s is not used, WebcamFilter uses software generated source, i.e. it generates pseudo-random frames by itself.  
When -t is not used, WebcamFilter displays output frames in an OpenGL window.

Before using your webcam as a source, make sure that it supports YUYV 4:2:2 format. You can do it using v4l2-ctl tool from v4l-utils apt package.
```
v4l2-ctl --list-formats-ext
```

You need to install v4l2loopback (https://github.com/umlaeute/v4l2loopback) kernel module which allows you to create a virtual video device if you want to use one as a target.

Shaders for use with OpenGL window target are located in shaders/display directory.  
Shaders for use with virtual video device target are located in shaders/v4l2loopback directory.  
Shaders for use with ncurses terminal target are located in shaders/terminal directory.  
In every of these directories there is one vertex shader rectangle.vert and several fragment shaders with .frag extension which implement the effects listed above.

### Usage examples
#### 1. Edge detection effect displayed in an OpenGL window
We start WebcamFilter passing only our webcam's path /dev/video0 and shaders from shaders/display directory.
```
./WebcamFilter -c -s webcam -w /dev/video0 -t display 640 480 640 480 shaders/display/rectangle.vert shaders/display/edges.frag
```
![imgur; window; edges](https://i.imgur.com/KuSvVRQ.png)

#### 2. Binarization effect for a virtual video device
Firstly, we create a virtual video device using v4l2loopback kernel module.
```
sudo modprobe v4l2loopback devices=1 card_label="VirtualCam" video_nr=5
```
Then we start WebcamFilter passing the created device's path /dev/video5 and shaders from shaders/v4l2loopback directory.
```
./WebcamFilter -c -s webcam -w /dev/video0 -t v4l2loopback -l /dev/video5 640 480 640 480 shaders/v4l2loopback/rectangle.vert shaders/v4l2loopback/binary.frag
```
We can select VirtualCam device as our webcam in an application e.g. Discord.  
![imgur; V4L2 device; binary](https://i.imgur.com/3AFU7ce.png)

#### 3. Smoothly downscaled input displayed in an ncurses terminal
Firstly, we set our terminal's font to a square font, i.e. one whose all characters are equally wide and high. Then we resize the terminal to 80 characters wide and 60 high. We start WebcamFilter passing our webcam's path /dev/video0 and shaders from shaders/terminal directory. Additionally, we specify target width as 80 and height as 60 (the size of our terminal) and source width and height greater than target's.
```
./WebcamFilter -c -s webcam -w /dev/video0 -t terminal 640 480 80 60 shaders/terminal/rectangle.vert shaders/terminal/smooth_downscaling.frag
```
![imgur; ncurses terminal; smooth downscaling](https://i.imgur.com/GG39Aci.png)

## Building
WebcamFilter creates an OpenGL window using GLFW and then calls GLEW functions to control the GPU. To build the program correctly you need both of the libraries. To get them you can install the following apt packages:
- ```libglfw3-dev``` (tested on 3.3.2-1)
- ```libglew-dev``` (tested on 2.1.0-4)

WebcamFilter also uses ncurses library to display output in the terminal, so you need to get it as well by installing the apt package ```libncurses-dev``` (tested on 6.2-0ubuntu2).

You can get all mentioned packages by executing
```
sudo apt install libglfw3-dev libglew-dev libncurses-dev
```

If you already have g++ and the required packages, you can build WebcamFilter using make.
```
make release
```
The executable will be placed in ```build/apps``` directory. Other possible make rules are ```debug``` ```clean``` ```info```
