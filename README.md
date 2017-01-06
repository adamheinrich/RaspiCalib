# RaspiCalib

RapiCalib is a camera calibration app from an [OpenCV tutorial][1] modified to
work directly with the Raspberry Pi camera.

A modified [RaspiVid][2] application is used to provide the image data (in a way
similar to the [RaspiCV][3] project).

The project works with OpenCV 3.1.0 release. The [camera_calibration][4] code
and the XML configuration file come from the OpenCV library. The original
`camera_calibration.cpp` is modified (see [e2bbc46][5]) to support the Raspberry
Pi camera.

The original `RaspiVid.c` is modified (see [d019d18][6]) to pass image and
motion vector buffers to callbacks defined in `cv.h`. File `cv.cpp` implements
these callbacks and provides the latest captured frame to class
`RaspiVideoCapture`.

The exact camera configuration (parametrs passed to the RaspiVid) is in `cv.cpp`
(function `process_thread`). Currently the configuraton is limited to grayscale
format. This can be easily modified (see function `cv_init()`).

## Building

First [build][7] the OpenCV library (version 3.1.0) and make sure it is
registered by `pkg-config`:

```
pkg-config --libs opencv
```

Then make the `USERLAND_DIR` variable in `Makefile` point to Raspberry Pi's
[userland][8] repository. `Makefile` references some files required by
`RaspiCV.c`.

Finally, build the application by `make` and run it by `make run` or:

```
 raspicalib default.xml
```

The application works exactly as described in the [tutorial][1]. The Raspberry
Pi camera is used if you set the `Input` element in the XML configuration file
to any number in range 0 to 9, e.g. `<Input>"1"</Input>`.

## Acknowledgements

The camera calibration code and tutorial was written by Bernát Gábor as a part
of the OpenCV library ([license][9]).

[1]: http://docs.opencv.org/master/d4/d94/tutorial_camera_calibration.html
[2]: https://www.raspberrypi.org/documentation/usage/camera/raspicam/raspivid.md
[3]: https://github.com/adamheinrich/RaspiCV
[4]: https://github.com/opencv/opencv/tree/3.1.0/samples/cpp/tutorial_code/calib3d/camera_calibration
[5]: https://github.com/adamheinrich/RaspiCalib/commit/e2bbc467641e89555b867b9b4cb59742282f29e8
[6]: https://github.com/adamheinrich/RaspiCalib/commit/d019d183a658283c54735ce9715eaa56c91efd50
[7]: http://www.pyimagesearch.com/2016/04/18/install-guide-raspberry-pi-3-raspbian-jessie-opencv-3/
[8]: https://github.com/raspberrypi/userland/
[9]: http://opencv.org/license.html
