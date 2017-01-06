# RaspiCalib

RapiCalib is the camera calibration app from an [OpenCV tutorial][1] modified to
work directly with the Raspberry Pi camera.

A modified [RaspiVid][2] application is used to provide the image data (in a way
similar to the [RaspiCV][3] project).

## Building

First [build][6] the OpenCV library and make sure it is registered by
`pkg-config`:

```
pkg-config --libs opencv
```

Then make the `USERLAND_DIR` variable in `Makefile` point to Raspberry Pi's
[userland][5] repository. `Makefile` references some files required by
`RaspiCV.c`.

Finally, build the application by `make` and run it by `make run` or:

```
 raspicalib default.xml
```

[1]: http://docs.opencv.org/master/d4/d94/tutorial_camera_calibration.html
[2]: https://www.raspberrypi.org/documentation/usage/camera/raspicam/raspivid.md
[3]: https://github.com/adamheinrich/RaspiCV
[4]: http://www.pyimagesearch.com/2016/04/18/install-guide-raspberry-pi-3-raspbian-jessie-opencv-3/
[5]: https://github.com/raspberrypi/userland/
