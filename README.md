# Object Detection C++ Sample SSD

This topic demonstrates how to run the Object Detection sample application, which does inference using object detection
networks like SSD-VGG on Intel® Processors and Intel® HD Graphics.


## How It Works (main.cpp)

Upon the start-up the sample application reads command line parameters and loads a network and an image to the Inference
Engine device. When inference is done, the application creates an
output image and outputs data to the standard output stream.

> **NOTE**: By default, Inference Engine samples and demos expect input with BGR channels order. If you trained your model to work with RGB order, you need to manually rearrange the default channels order in the sample or demo application or reconvert your model using the Model Optimizer tool with `--reverse_input_channels` argument specified. For more information about the argument, refer to **When to Reverse Input Channels** section of [Converting a Model Using General Conversion Parameters](./docs/MO_DG/prepare_model/convert_model/Converting_Model_General.md).

## Running (main.cpp)
- Edit `CMakeLists.txt` to switch compilation between `main.cpp` and `main_dnn.cpp`
- Build docker using `docker build -t sample:ssd `
- Run docker `docker run -it sample:ssd /bin/bash` 
- `config.yaml` should have all the paths like this 
```
images_paths:
    - "/workspace/img.bmp"
input: "rtsp://admin:admin123@192.168.100.2:554/"
model: "/app/windows/D/workspace/CNN/imaginex/timeNattendance/models/FP32-INT1/face-detection-adas-binary-0001.bin"
config: "/app/windows/D/workspace/CNN/imaginex/timeNattendance/models/FP32-INT1/face-detection-adas-binary-0001.xml"
debug_log: false 
device: "CPU"
thr: 0.5
nms: 0.4
scale: 1.0
rgb: true
width: -1
height: -1
async: 0
backend: 2
framework: ""
target: 0

```
Running the application with the empty list of options yields the usage message given above and an error message.

To run the sample, you can use public or pre-trained models. To download the pre-trained models, use the OpenVINO [Model Downloader](https://github.com/opencv/open_model_zoo/tree/2018/model_downloader) or go to [https://download.01.org/opencv/](https://download.01.org/opencv/).